#include <stdio.h>
#include "game_logic.h"
#include <string.h>
#include "message_handler.h"

#include <stdlib.h>
#include <winsock2.h>

#include "server.h" // For send_to_all_clients
extern int player_sockets[MAX_CLIENTS];

// Send an error message to a specific client
void send_error_message(int client_socket, const char *error_msg) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "ERROR: %s\n", error_msg);
    send(client_socket, buffer, strlen(buffer), 0);
}
// Handle a move from the client
void handle_move(const char *msg, int client_socket, GameState *game) {
    int player_id = (client_socket == player_sockets[0]) ? 0 : 1;

    // Parse the move - expecting a pit index as an integer after "MOVE"
    int pit_index = atoi(msg); // Convert the string to an integer pit index

    // Check if it's the correct player's turn
    if (player_id != game->current_turn) {
        send_error_message(client_socket, "Not your turn!");
        return;
    }

    // Attempt to make the move
    int move_result = make_move(game, player_id, pit_index);

    if (move_result == 0) {
        // Successful move, broadcast the updated board to all players
        broadcast_board(game);

        // Check if the game has ended
        if (check_game_end(game)) {
            int winner_id = determine_winner(game); // Function to determine the winner
            send_game_end(winner_id); // Notify clients of the game outcome
        }
    } else if (move_result == -2) {
        send_error_message(client_socket, "Move would starve opponent!");
    } else {
        send_error_message(client_socket, "Invalid move!");
    }
}

void broadcast_board(const GameState *game) {
    char board_msg[256];
    // Generate ASCII board representation
    snprintf(board_msg, sizeof(board_msg), "\nPlayer 2\n %d %d %d %d %d %d\n %d %d %d %d %d %d\nPlayer 1\n",
             game->pits[1][5], game->pits[1][4], game->pits[1][3], game->pits[1][2], game->pits[1][1], game->pits[1][0],
             game->pits[0][0], game->pits[0][1], game->pits[0][2], game->pits[0][3], game->pits[0][4], game->pits[0][5]);

    send_to_all_clients(board_msg); // Send to all connected clients
}


// Send a game end message to all clients
void send_game_end(int winner_id) {
    char end_msg[256];
    snprintf(end_msg, sizeof(end_msg), "GAME_END: Player %d wins!\n", winner_id + 1);
    send_to_all_clients(end_msg);
}