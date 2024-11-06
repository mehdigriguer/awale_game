//
// Created by felix on 06.11.2024.
//

#ifndef SERVER_H
#define SERVER_H


#define PORT 8080
#define MAX_CLIENTS 2

// Function prototypes
int setup_server_socket();
int accept_client(int server_fd);
void send_to_all_clients(const char *msg);
void handle_client_message(int client_socket);
void server_loop(int server_fd);


#endif //SERVER_H
