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
        MovementPatterns.cpp
        MovementPatterns.h
        PathValidator.cpp
        PathValidator.h
        KingValidator.cpp
        KingValidator.h
        MoveHistory.cpp
        MoveHistory.h
        PieceOwnership.cpp
        PieceOwnership.h
        CastlingValidator.cpp
        CastlingValidator.h
        PawnValidator.cpp
        PawnValidator.h
        RayTable.cpp
        RayTable.h
        AttackValidator.cpp
        AttackValidator.h
        Structs.cpp
        Structs.h
)
