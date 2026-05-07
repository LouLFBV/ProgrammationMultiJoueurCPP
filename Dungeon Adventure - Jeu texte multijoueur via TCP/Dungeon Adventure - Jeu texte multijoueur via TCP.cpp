#include <iostream>
#include <winsock2.h>
#include <thread>
#include <ctime>

#include "Dungeon.h"
#include "GameLogic.h"

#pragma comment(lib, "ws2_32.lib") 

int main() {
    // Initialisation Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    //  Initialisation du Donjon et de la Logique de Jeu
    Dungeon dungeon;
    GameLogic gameLogic(dungeon); // Permet d'utiliser toutes les méthodes du GameLogic

    std::cout << "--- SERVER DUNGEON MULTIPLAYER  ---" << std::endl;

    // Configuration du Socket d'écoute
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) { // Si problème lors de la création du socket
        std::cerr << "socket() failed." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000); // Port 54000
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&hint, sizeof(hint));
    listen(listening, SOMAXCONN);



    std::cout << "Server waiting on port 54000..." << std::endl;
    std::cout << "Type 'EXIT' here to shut down the server properly.." << std::endl;

    // Thread pour le serveur 
    std::thread consoleThread([&gameLogic]() {
        std::string command;
        while (std::cin >> command){
            if (gameLogic.ToUpperCase(command) == "EXIT") { // Si on veut couper le serveur.
                gameLogic.Shutdown();
				std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Pour être sur que le message de shutdown soit envoyé avant de fermer les sockets.
                exit(0);
            }
        }
        });
    consoleThread.detach();

    // Boucle d'acceptation des clients.
    while (true) {
        sockaddr_in client{};
        int clientSize = sizeof(client);
        SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

        if (clientSocket != INVALID_SOCKET) {
            std::cout << "New customer connected." << std::endl;

            //Affiche les info pour le joueur.
            std::thread playerThread(&GameLogic::HandleClient, &gameLogic, clientSocket);
			playerThread.detach(); // Détache le thread pour qu'il puisse s'exécuter indépendamment et que le serveur puisse continuer à accepter d'autres clients.
        }
    }

    closesocket(listening);
    WSACleanup();
    return 0;
}