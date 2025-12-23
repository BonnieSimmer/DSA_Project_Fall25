#include "include/CommandLineInterface.hpp"
#include "include/GraphicalUserInterface.h"
#include "include/AppMode.h"

#include <iostream>
#include <QMessageBox>

AppMode currentMode = AppMode::GUI;

int main(int argc, char *argv[]) {
    if (argc > 1) {
        currentMode = AppMode::CLI;
        return CommandLineInterface::run(argc, argv);
    }
    currentMode = AppMode::GUI;
    return run_gui(argc, argv);
}

void reportError(const std::string& title, const std::string& message) {
    if (currentMode == AppMode::GUI) {
        QMessageBox::critical(nullptr, QString::fromStdString(title), QString::fromStdString(message));
    } else {
        std::cerr << "\033[1;31m[ERROR]: " << title << " - " << message << "\033[0m" << std::endl;
    }
}



void reportInfo(const std::string& title, const std::string& message) {
    if (currentMode == AppMode::GUI) {
        QMessageBox::information(nullptr, QString::fromStdString(title), QString::fromStdString(message));
    } else {
        std::cout << title << " - " << message << std::endl;
    }
}