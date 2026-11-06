//
// Created by devrupamsahoo on 04-11-2026.
//

#include <QApplication>

#include "include/MainWindow.h"

// creates the application instance, displays the main window, and starts the Qt event loop
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    // keeps the application running and processes GUI events until the application exits
    return app.exec();
}
