# defines the desktop application and registers its source, header, and resource files
qt_add_executable(chess_desktop
        main.cpp
        include/MainWindow.h
        include/ChessBoardWidget.h
        src/MainWindow.cpp
        src/ChessBoardWidget.cpp
        "${PROJECT_SOURCE_DIR}/assets/resources.qrc"
        include/PieceAssets.h
        src/PieceAssets.cpp
)

# links the chess engine and the Qt modules required by the desktop interface
target_link_libraries(chess_desktop
        PRIVATE chess_core Qt6::Widgets Qt6::Svg
)

# prevents Windows from opening an additional console window when launching the GUI application
set_target_properties(chess_desktop PROPERTIES WIN32_EXECUTABLE ON)