# defines the terminal chess application and registers its source files
add_executable(chess_engine
        main.cpp
        src/GameManager.cpp
        src/DisplayBoard.cpp
)

# links the shared chess engine library containing the core game logic
target_link_libraries(chess_engine PRIVATE chess_core)

# provides the project root path to the application at compile time
target_compile_definitions(chess_engine
        PRIVATE CHESS_ENGINE_PROJECT_DIR="${PROJECT_SOURCE_DIR}"
)