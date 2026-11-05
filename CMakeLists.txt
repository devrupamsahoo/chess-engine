cmake_minimum_required(VERSION 4.3)

project(chess_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# shared chess rules and board logic
add_library(chess_core
        src/core/ChessBoard.cpp
        src/core/Enums.cpp
        src/core/Structs.cpp
        src/core/Utils.cpp

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

        src/notation/FEN.cpp
        include/chess/core/GameSession.h
        src/core/GameSession.cpp

)

target_include_directories(chess_core
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# terminal application
add_executable(chess_engine
        applications/terminal/main.cpp
        applications/terminal/GameManager.cpp
        applications/terminal/DisplayBoard.cpp
)

target_link_libraries(chess_engine
        PRIVATE
        chess_core
)

# used by the terminal app to export match_fens.txt to the project directory
target_compile_definitions(chess_engine
        PRIVATE
        CHESS_ENGINE_PROJECT_DIR="${PROJECT_SOURCE_DIR}"
)

option(BUILD_QT_GUI "Build the Qt desktop application" ON)

if(BUILD_QT_GUI)
    find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)

    qt_standard_project_setup()

    set(CMAKE_AUTORCC ON)

    qt_add_executable(chess_desktop
            applications/desktop/main.cpp
            applications/desktop/MainWindow.cpp
            applications/desktop/ChessBoardWidget.cpp
            assets/resources.qrc
            applications/desktop/PieceAssets.h
            applications/desktop/PieceAssets.cpp
    )

    target_link_libraries(chess_desktop
            PRIVATE
                chess_core
                Qt6::Widgets
                Qt6::Svg
    )

    set_target_properties(chess_desktop PROPERTIES
            WIN32_EXECUTABLE ON
    )

endif()