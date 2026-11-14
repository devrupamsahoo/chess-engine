cmake_minimum_required(VERSION 4.3)

project(chess_engine LANGUAGES CXX)
include(CTest)

# enforces the c++20 standard without compiler-specific language extensions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# builds the shared chess engine library used by the applications and tests
add_library(chess_core
        src/core/ChessBoard.cpp
        src/core/Enums.cpp
        src/core/Structs.cpp
        src/core/Utils.cpp
        src/core/GameSession.cpp

        src/history/MoveHistory.cpp

        src/move/AttackValidator.cpp
        src/move/CastlingValidator.cpp
        src/move/KingValidator.cpp
        src/move/MoveValidator.cpp
        src/move/MovementPatterns.cpp
        src/move/PathValidator.cpp
        src/move/PawnValidator.cpp
        src/move/PieceOwnership.cpp
        src/move/RayTable.cpp

        src/movegen/MoveGeneration.cpp
        src/notation/FEN.cpp

        src/evaluation/PieceBonuses.cpp
        src/evaluation/PawnStructure.cpp
        src/evaluation/Mobility.cpp
        src/evaluation/KingSafety.cpp
        src/evaluation/KingAttackers.cpp
        src/evaluation/GamePhase.cpp
        src/evaluation/EvaluationConstants.cpp
        src/evaluation/Evaluation.cpp
)

# exposes the public header directory to targets that link against chess_core
target_include_directories(chess_core
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# includes the terminal application in the build
add_subdirectory(applications/terminal)

# controls whether the Qt desktop application is included in the build
option(BUILD_QT_GUI "Build the Qt desktop application" ON)

if(BUILD_QT_GUI)
    # finds the required Qt modules and configures Qt-specific build features
    find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)
    qt_standard_project_setup()
    set(CMAKE_AUTORCC ON)
    add_subdirectory(applications/desktop)
endif()

# includes the test targets when CTest testing is enabled
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
