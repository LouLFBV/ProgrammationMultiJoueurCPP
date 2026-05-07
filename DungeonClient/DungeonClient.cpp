#include <iostream>
#include <string>
#include <winsock2.h> 
#include <ws2tcpip.h>  

#pragma comment(lib, "ws2_32.lib")


std::string RecoverResponse(SOCKET s) {
    std::string words; 
    char c; 
    std::string word; 
    while (true) {
        int r = recv(s, &c, 1, 0); // Lit les données char par char grâce à recv, 1 pour un octet/un char
        if (r <= 0) 
            return "SHUTDOWN";
        if (c == '\r') 
            continue; 
        if (c == '\n') {
			if (word == "END") break; // END indique la fin du message, on sort de la boucle pour que le client reprenne la main sur le client
            words += word + "\n";      
            word = "";
        }
        else {
            word += c;
        }
    }
    return words;
}

std::string ToUpperCase(std::string sentence) {
    for (auto& c : sentence)
        c = (char)toupper((unsigned char)c);
    return sentence;
}

int main() {
    WSADATA wsaData; // Un bloc mémoire qui va recevoir les détails techniques de l'implémentation réseau
    WSAStartup(MAKEWORD(2, 2), &wsaData); // Fonction qui prépare le réseau, "MAKEWORD(2, 2)" pour utiliser la version 2, 2 de Winsock


    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // on créé un Socket qui permet la communication avec  IPv4 et SOCK_STREAM pour le protocol TCP

	// On configure les infos du serveur auquel on veut se connecter
    sockaddr_in hint; 
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000); 
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr); 



    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint)); // On tente d'établir la connexion avec le serveur
    if (connResult == SOCKET_ERROR) { // Si cela renvoie une erreur
        std::cerr << " ERROR : Connection failed." << std::endl;
        closesocket(sock);  
        WSACleanup(); 
        return 1;
    }


    std::cout << "# CLIENT DUNGEON ADVENTURE #" << std::endl;

    std::string userInput;
    std::string welcome = RecoverResponse(sock); // Recoit le message de bienvenu du serveur
    std::cout << welcome; 

    std::string nameInput;
    std::getline(std::cin, nameInput); 
	send(sock, (nameInput + "\n").c_str(), (int)(nameInput.size() + 1), 0); // Envoie notre nom au serveur en ajoutant \n pour que le serveur puisse lire la ligne correctement. +1 pour inclure le \n dans la taille du message

    std::string confirm = RecoverResponse(sock); // Attends une réponse du serveur pour continuer
    std::cout << confirm;

    
    while (true) { // BOUCLE DE JEU
        std::cout << "> ";
        std::getline(std::cin, userInput); 
        if (userInput.empty()) continue; 

        //  Envoyer la commande
        std::string command = userInput + "\n"; // On ajoute \n pour que le serveur lise la commande correctement
        send(sock, command.c_str(), (int)command.size(), 0); // Envoie les infos dans le socket pour que le serveur lise les infos

        if (ToUpperCase(userInput) == "QUIT") break; // Si on écrit QUIT, on quitte la boucle, c'est la fin de la communication avec le serveur

        // Attendre la réponse du serveur
        std::string response = RecoverResponse(sock);        
        if (response == "SHUTDOWN") { // Si le serveur s'arrete
            std::cout << "\nERROR: Serveur shutting down..." << std::endl;
            break; 
        }
        std::cout << response << std::endl; 

    }

    closesocket(sock); 

    WSACleanup();
    return 0;
}

// Pour ajouter un nouveau client, je démarre une nouvelle instance de cette solution (la solution du client)