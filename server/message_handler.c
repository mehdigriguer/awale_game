#include <stdio.h>

#include "game_logic.h"
//
// Created by felix on 06.11.2024.
//
void broadcast_board(const GameState *game) {
    char board_msg[256];
    // Generate ASCII board representation
    snprintf(board_msg, sizeof(board_msg), "\nPlayer 2\n %d %d %d %d %d %d\n %d %d %d %d %d %d\nPlayer 1\n",
             game->pits[1][5], game->pits[1][4], game->pits[1][3], game->pits[1][2], game->pits[1][1], game->pits[1][0],
             game->pits[0][0], game->pits[0][1], game->pits[0][2], game->pits[0][3], game->pits[0][4], game->pits[0][5]);

    send_to_all_clients(board_msg); // Send to all connected clients
}

void handle_move(char *msg, int client_socket, GameState *game){
    
}