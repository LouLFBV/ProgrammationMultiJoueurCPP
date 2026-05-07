#pragma once
#include <winsock2.h>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "Dungeon.h"
#include "Player.h"

//Grosse classe qui va gérer les commandes du joueur 


// Struct pour stocker lese clients dans le réseau, avec leur nom et les messages en attente à envoyer
struct ClientInfo {
    std::string name;
    std::vector<std::string> pending;
    std::mutex pendingMutex;
};

class GameLogic {
private:
    std::map<SOCKET, ClientInfo*> m_activeClients;
    std::mutex m_clientsMutex;
    std::mutex m_dungeonMutex;
    Dungeon& m_dungeon;

    // Méthodes privées qui font servir à faciliter la communication entre serveur, client et utilisateur
    std::vector<std::string> Split(const std::string& sentence);
    std::string RecoverLine(SOCKET sock);
    void FlushAndSend(SOCKET clientSocket, ClientInfo& info, const std::string& response);

public:
    GameLogic(Dungeon& dungeon);
    ~GameLogic(); 

    void HandleClient(SOCKET clientSocket);
    std::string ProcessCommand(const std::string& input, Player& p);
    void Broadcast(const std::string& message); // Permet de communiquer avec tous les joueurs
    void Shutdown();
    std::string ToUpperCase(std::string sentence);

    std::string LookCommand(Player& player);
    std::string MoveCommand(Player& player, const std::vector<std::string>& tokens);
    std::string AttackCommand(Player& player, const std::vector<std::string>& tokens);
    std::string PickupCommand(Player& player, const std::vector<std::string>& tokens);
    std::string StatusCommand(Player& player);
    std::string SayCommand(Player& player, const std::vector<std::string>& tokens);
    std::string QuitCommand(Player& player);
};