#include "include/CommandLineInterface.hpp"
#include "include/GraphicalUserInterface.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        return CommandLineInterface::run(argc, argv);
    }
    return run_gui(argc, argv);
}