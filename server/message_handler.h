//
// Created by felix on 06.11.2024.
//

#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H


typedef enum { CHAT, MOVE, BOARD, GAME_END } MessageType;

// Function prototypes
void handle_chat(const char *msg, int client_socket);
void handle_move(const char *msg, int client_socket, GameState *game);
void broadcast_board(const GameState *game);
void send_game_end(int winner_id);


#endif //MESSAGE_HANDLER_H
