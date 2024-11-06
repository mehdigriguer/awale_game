cmake_minimum_required(VERSION 3.29)
project(awale_game C)

set(CMAKE_C_STANDARD 11)

add_executable(awale_game
        server/game.c
        server/game_logic.c
        server/game_logic.h
        server/message_handler.h
        server/message_handler.c
        server/server.c
        server/server.h)
#FOR WINDOWS
target_link_libraries(awale_game ws2_32)
