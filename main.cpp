// #include "include/CommandLineInterface.hpp"
// #include "include/GraphicalUserInterface.h"

// int main(int argc, char *argv[]) {
//     if (argc > 1) {
//         return CommandLineInterface::run(argc, argv);
//     }
//     return run_gui(argc, argv);
// }

#include <iostream>
#include <string>
#include <vector>
#include "include\CommandLineInterface.hpp"

int main() {
    CommandLineInterface cli;

    std::cout << "--- TEST 1: Verification (Output to console) ---" << std::endl;
    // Simulating: xml_editor verify -i input_errors.xml
    char* test1_argv[] = {
        (char*)"xml_editor", 
        (char*)"verify", 
        (char*)"-i", 
        (char*)"sample.xml"
    };
    cli.run(4, test1_argv);

    std::cout << "\n--- TEST 2: Fixing (Output to file) ---" << std::endl;
    // Simulating: xml_editor verify -i input_errors.xml -f -o output_fixed.xml
    char* test2_argv[] = {
        (char*)"xml_editor", 
        (char*)"verify", 
        (char*)"-i", 
        (char*)"sample.xml", 
        (char*)"-f", 
        (char*)"-o", 
        (char*)"testingOutput.xml"
    };
    cli.run(7, test2_argv);

    return 0;
}