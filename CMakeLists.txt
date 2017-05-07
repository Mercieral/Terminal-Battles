cmake_minimum_required(VERSION 3.7)
project(battleship)

set(SOURCE_FILES
        main.cpp
        user/user.cpp
        user/user.hpp
        user/host.cpp
        user/client.cpp
        game/game_board.cpp
        game/game_board.hpp
        game/game_piece.cpp
        game/game_piece.hpp)

add_executable(battleship ${SOURCE_FILES})