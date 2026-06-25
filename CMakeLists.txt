cmake_minimum_required(VERSION 4.3)
project(chess_engine)

set(CMAKE_CXX_STANDARD 20)

add_executable(chess_engine main.cpp
        ChessBoard.h
        ChessBoard.cpp
        Utils.cpp
        Utils.h
        GameManager.cpp
        GameManager.h
        MoveValidator.cpp
        MoveValidator.h
        Enums.cpp
        Enums.h
)