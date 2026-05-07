#include "GameLogic.h"
#include "Sword.h"

#include <iostream>
#include <algorithm>


GameLogic::GameLogic(Dungeon& dungeon) : m_dungeon(dungeon) {}

GameLogic::~GameLogic() {
    Shutdown();
}


void GameLogic::HandleClient(SOCKET clientSocket) {
    std::string name;
    std::string prompt = "HELLO Enter your name:\nEND\n";

	// Envoie la demande de nom au client et attend la réponse
    send(clientSocket, prompt.c_str(), (int)prompt.size(), 0);

	// Recupere le nom du client
    name = RecoverLine(clientSocket);

    if (name.empty()) name = "Unknown";

	// Initialisation du joueur et de son info client
    Player player = Player(20, name, new Knife(5));
	ClientInfo* info = new ClientInfo(); // Pour stocker les messages en attente et le nom du client
    info->name = player.GetName();

    {
		std::lock_guard<std::mutex> lock(m_clientsMutex); // On lock m_clientsMutex pour le client soit bien ajouté sans crash, dans un bloc de code pour libérer le mutex à la sortie du bloc
		m_activeClients[clientSocket] = info; // On ajoute le client à la liste des clients actifs
    }

    Broadcast(player.GetName() + " joined the dungeon!\n");

    // Message de bienvenue + LOOK 
    std::string welcome = "WELCOME " + player.GetName() + "! Type HELP to see all commands.\n";
    std::string lookResult = ProcessCommand("LOOK", player);

    FlushAndSend(clientSocket, *info, welcome + lookResult);

    // Boucle principale
    while (true) {
        std::string input = RecoverLine(clientSocket);

        // Si le client ferme la fenêtre
        if (input.empty()) break;

        std::string response = ProcessCommand(input, player);
        FlushAndSend(clientSocket, *info, response);
    }


    // Nettoyage
    {
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        if (m_activeClients.count(clientSocket)) {
            delete m_activeClients[clientSocket];
            m_activeClients.erase(clientSocket);
        }
    }
    closesocket(clientSocket);
}

std::string GameLogic::ProcessCommand(const std::string& input, Player& player) {
    std::vector<std::string> tokens = Split(input);
    if (tokens.empty()) return "";

    std::string command = ToUpperCase(tokens[0]);

    if (!player.IsAlive() && command != "SAY" && command != "QUIT") {
        return "ERROR: You are DEAD. You can only use SAY, STATUS or QUIT.\n";
    }

    if (command == "LOOK")    return LookCommand(player);
    else if (command == "MOVE")    return MoveCommand(player, tokens);
    else if (command == "ATTACK")  return AttackCommand(player, tokens);
    else if (command == "PICKUP")  return PickupCommand(player, tokens);
    else if (command == "SAY")     return SayCommand(player, tokens);
    else if (command == "STATUS")  return StatusCommand(player);
    else if (command == "HELP")    return "Commands: LOOK, MOVE <room id>, ATTACK <enemy>, PICKUP <item>, SAY <text>, STATUS, QUIT\n";
    else if (command == "QUIT")    return QuitCommand(player);
    else return "ERROR: Wrong command\n";
}

std::string GameLogic::LookCommand(Player& player)
{
    std::lock_guard<std::mutex> lock(m_dungeonMutex);
    auto& rooms = m_dungeon.GetRooms();
    Room& current = *rooms[player.GetCurrentRoomId()];

    std::string response = "\nRoom " + std::to_string(current.GetId()) + ", description : " + current.GetDescription() + "\n";

    // Exits
    response += "Exits: ";
    for (int exitId : current.GetExits()) response += "[" + std::to_string(exitId) + "] ";
    response += "\n";


    // Items
    response += "Items: ";
    if (current.GetItems().empty())
        response += "no items";
    else{
        for (auto const& item : current.GetItems())
        {
            response += item->GetName() + " ";
			if (item != current.GetItems().back())
                response += " - ";
        }

    }
    response += "\n";

    // Enemies
    response += "Enemies: ";
    if (current.GetEnemies().empty())
        response += "no enemies";
    else {
        for (auto const& enemy : current.GetEnemies())
        {
            response += enemy->GetName() + " (HP:" + std::to_string(enemy->GetCurrentHealth()) + ") ";
            if (enemy != current.GetEnemies().back())
                response += " - ";
        }
    }
    response += "\n";

    return response;
}

std::string GameLogic::MoveCommand(Player& player, const std::vector<std::string>& tokens) {
    if (tokens.size() < 2) return "ERROR: Usage: MOVE <room_id>\n";

    try {
        int targetId = std::stoi(tokens[1]);
        std::lock_guard<std::mutex> lock(m_dungeonMutex);

        Room& current = *m_dungeon.GetRooms()[player.GetCurrentRoomId()];
        bool canGo = false;

        for (int exitId : current.GetExits()) {
            if (exitId == targetId) { canGo = true; break; }
        }

        if (canGo && m_dungeon.GetRooms().count(targetId)) {
            player.SetCurrentRoomId(targetId);
            Broadcast("MOVE: " + player.GetName() + " moved to room " + std::to_string(targetId) + ".\n");
            return "You move to: " + m_dungeon.GetRooms()[targetId]->GetDescription() + "\n";
        }
        return "ERROR: You cannot go there.\n";
    }
    catch (...) {
        return "ERROR: Invalid Room ID.\n";
    }
}

std::string GameLogic::AttackCommand(Player& player, const std::vector<std::string>& tokens)
{
    std::string response = "";

    if (tokens.size() < 2) {
        response = "ERROR: Usage: ATTACK <enemy_name>\n";
    }
    else {
        std::string enemyName = "";
        for (size_t i = 1; i < tokens.size(); ++i) enemyName += (i > 1 ? " " : "") + tokens[i];

        std::lock_guard<std::mutex> lock(m_dungeonMutex);
        Room& current = *m_dungeon.GetRooms()[player.GetCurrentRoomId()];
        auto& ennemies = current.GetEnemies();

        Enemy* target = nullptr;
        size_t targetIdx = 0;

        

        for (size_t i = 0; i < ennemies.size(); ++i) {
            if (ToUpperCase(ennemies[i]->GetName()) == ToUpperCase(enemyName)) {
                target = ennemies[i];
                targetIdx = i;
                break;
            }
        }

        if (target) {
            
            if (!player.GetCurrentWeapon()) {
                return "ERROR: You have no weapon to attack with!\n";
            }
            response += player.UseWeapon(player.GetCurrentWeapon(), *target);

            if (target->GetCurrentHealth() <= 0) {
                Broadcast("KILL: " + player.GetName() + " slew " + target->GetName() + "!\n");

                ennemies.erase(ennemies.begin() + targetIdx);
            }
            else {
                // Variable pour calculer les dégâts subis par le joueur car les dégâts ont une part d'aléatoire
                int playerHealthBeforeAttack = player.GetCurrentHealth();
                response += target->UseWeapon(target->GetWeapon(), player);
                Broadcast("COMBAT: " + player.GetName() + " suffered " + std::to_string(playerHealthBeforeAttack - player.GetCurrentHealth()) + " attack points by " + target->GetName() + "!\n");
            }
        }
        else {
            response = "ERROR: Monster not found.\n";
        }
    }
    return response+"\n";
}

std::string GameLogic::PickupCommand(Player& player, const std::vector<std::string>& tokens)
{
    if (tokens.size() < 2) {
        return "ERROR: Usage: PICKUP <item_name>\n";
    }
    else {
        std::string itemName = "";
        for (size_t i = 1; i < tokens.size(); ++i) {
            itemName += tokens[i];
            if (i < tokens.size() - 1) itemName += " "; 
        }

        std::lock_guard<std::mutex> lock(m_dungeonMutex);
        Room& current = *m_dungeon.GetRooms()[player.GetCurrentRoomId()];

        auto& items = current.GetItems();

        for (size_t i = 0; i < items.size(); ++i) {
            if (ToUpperCase(items[i]->GetName()) == ToUpperCase(itemName)) {

                std::string nameFound = items[i]->GetName();
                std::string itemResponse = player.TakeItem(items[i]);
                items.erase(items.begin() + i);

                return "You picked up: " + nameFound + "\n" + itemResponse ;
            }
        }

        return "ERROR: Item '" + itemName + "' not found here.\n";
    }
}

std::string GameLogic::StatusCommand(Player& player)
{
    return "Your stats : Name : " + player.GetName() + "; HP :  " + std::to_string(player.GetCurrentHealth()) + " ; Location: Room " + std::to_string(player.GetCurrentRoomId()) + " ; Current Weapon : " + player.GetCurrentWeapon()->GetName() +"\n";
}

std::string GameLogic::SayCommand(Player& player, const std::vector<std::string>& tokens)
{
    if (tokens.size() < 2) {
        return "ERROR: Usage: SAY <message>\n";
    }
    else {
        std::string message;
        for (size_t i = 1; i < tokens.size(); ++i)
            message += tokens[i] + " ";
        Broadcast("[CHAT] " + player.GetName() + ": " + message + "\n");
        return "Message sent.\n";
    }
}

std::string GameLogic::QuitCommand(Player& player)
{
    Broadcast(player.GetName() + " left the dungeon.\n");
    return "Goodbye, " + player.GetName() + "!\n";
}



// Envoie un message à tous les clients connectés, en le mettant dans leur file d'attente de messages à envoyer (pending) pour qu'il soit envoyé au prochain flush du client, on lock les mutex.
void GameLogic::Broadcast(const std::string& message) {
    std::lock_guard<std::mutex> lock(m_clientsMutex);
    for (auto& [sock, info] : m_activeClients) {
        std::lock_guard<std::mutex> pl(info->pendingMutex);
        info->pending.push_back(message);
    }
}


void GameLogic::Shutdown() {
    Broadcast("Error: Serveur shutting down...\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Pour être sur que le Broadcast soit envoyé avant de fermer les sockets.

    std::lock_guard<std::mutex> lock(m_clientsMutex);
    for (auto const& [sock, info] : m_activeClients) {
        closesocket(sock);
    }
}

// Transforme les données reçues du client en une ligne de texte, en gérant les retours à la ligne et les caractères de fin de ligne.
std::string GameLogic::RecoverLine(SOCKET sock) {
    std::string line;
    char c;
    while (recv(sock, &c, 1, 0) > 0) {
        if (c == '\n') 
            break;
        if (c != '\r') 
            line += c;
    }
    return line;
}


// Cette méthode va envoyer au client le message de réponse à sa commande, mais aussi tous les messages en attente (comme les messages de broadcast).
void GameLogic::FlushAndSend(SOCKET clientSocket, ClientInfo& info, const std::string& response) {
    std::string packet;
    {
        std::lock_guard<std::mutex> pl(info.pendingMutex);
        for (auto& msg : info.pending) 
            packet += msg;
        info.pending.clear();
    }
	packet += response + "END\n"; // On ajoute "END\n" à la fin du paquet pour indiquer au client que c'est la fin du message.
    send(clientSocket, packet.c_str(), (int)packet.size(), 0);
}

std::vector<std::string> GameLogic::Split(const std::string& sentence) {
    std::vector<std::string> words;
    std::string word;
    for (char c : sentence) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else word += c;
    }
    if (!word.empty()) 
        words.push_back(word);
    return words;
}

std::string GameLogic::ToUpperCase(std::string sentence) {
    for (auto& c : sentence)
        c = (char)toupper((unsigned char)c);
    return sentence;
}