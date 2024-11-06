cmake_minimum_required(VERSION 3.29)
project(awale_game C)

set(CMAKE_C_STANDARD 11)

add_executable(awale_game
        server/game.c)
