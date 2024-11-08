//
// Created by felix on 06.11.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "game_logic.h"
#include "message_handler.h"
#include <sys/select.h>


#define PORT 8080
#define MAX_CLIENTS 2

// Global game state
GameState game;

// Player sockets
int player_sockets[MAX_CLIENTS] = {0};
char *player_names[MAX_CLIENTS] = {};
int num_players = 0;

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Setup server socket, listen for connections, and accept clients
int setup_server_socket() {
    int server_fd;
    struct sockaddr_in address;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        error_exit("Socket creation failed");
    }

    int opt = 1;
    //Casting the integer value to a pointer to the option value for windows
    //FOR LINUX
    //if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt))) {
        error_exit("Set socket options failed");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        error_exit("Binding failed");
    }

    if (listen(server_fd, 3) < 0) {
        error_exit("Listening failed");
    }

    printf("Server listening on port %d...\n", PORT);
    return server_fd;
}

// Accept a new client connection and add to the player list
int accept_client(int server_fd) {
    int new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char player_name[50]; // Allocation pour le nom du joueur, ajustez la taille si nécessaire
    int name_taken;

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
        error_exit("Accepting connection failed");
    }

    if (num_players < MAX_CLIENTS) {
        player_sockets[num_players] = new_socket;

        do {
            name_taken = 0; // Reset flag for each attempt
            printf("Choose a name : ");
            scanf("%49s", player_name); // Limite la longueur du nom à 49 caractères pour éviter les débordements

            // Check if the name is already taken
            for (int i = 0; i < num_players; i++) {
                if (strcmp(player_names[i], player_name) == 0) {
                    printf("Name already taken, please choose another.\n");
                    name_taken = 1;
                    break;
                }
            }
        } while (name_taken); // Repeat until a unique name is provided

        // If the name is unique, add it to the list
        player_names[num_players] = strdup(player_name); // Alloue dynamiquement le nom pour l'ajouter à la liste
        printf("%s is connected (Player %d) !\n", player_name, num_players + 1);
        num_players++;
    } else {
        printf("Max players reached. Rejecting connection.\n");
        close(new_socket);
    }

    return new_socket;
}

// Broadcast a message to all clients
void send_to_all_clients(const char *msg) {
    for (int i = 0; i < num_players; i++) {
        send(player_sockets[i], msg, strlen(msg), 0);
    }
}

// Handle messages from clients
void handle_client_message(int client_socket) {
    char buffer[1024] = {0};
    int valread = read(client_socket, buffer, sizeof(buffer));

    if (valread <= 0) {
        printf("Player disconnected.\n");
        close(client_socket);
        return;
    }

    // Determine the message type and handle it
    if (strncmp(buffer, "MOVE", 4) == 0) {
        int player_id = (client_socket == player_sockets[0]) ? 0 : 1;
        handle_move(buffer + 5, client_socket, &game); // Send the move to game logic
    } else {
        printf("Unknown message: %s\n", buffer);
    }
}

// Main server loop to manage clients and handle their messages
void server_loop(int server_fd) {
    fd_set readfds;
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        int max_sd = server_fd;

        for (int i = 0; i < num_players; i++) {
            FD_SET(player_sockets[i], &readfds);
            if (player_sockets[i] > max_sd)
                max_sd = player_sockets[i];
        }

        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            printf("Select error\n");
            continue;
        }

        if (FD_ISSET(server_fd, &readfds)) {
            accept_client(server_fd);
            if (num_players == MAX_CLIENTS) {
                initialize_game(&game);
                send_to_all_clients("Game started!\n");
                broadcast_board(&game);
            }
        }

        for (int i = 0; i < num_players; i++) {
            int sock = player_sockets[i];
            if (FD_ISSET(sock, &readfds)) {
                handle_client_message(sock);
            }
        }
    }
}
//Windows socket code
int main() {

    //FROM HERE
    WSADATA wsaData;

    // Initialize WinSock
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }
    //TO HERE

    int server_fd = setup_server_socket();
    server_loop(server_fd);
    close(server_fd);

    //FROM HERE
    WSACleanup();
    //TO HERE


    return 0;
}
