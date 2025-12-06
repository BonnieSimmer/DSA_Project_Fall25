#include "CommandLineInterface.hpp"
#include <iostream>
#include <vector>
#include <fstream>

void CommandLineInterface::run(int argc, char* argv[]) {

    if (argc < 3) {
        std::cout << "Invalid command\n";
        return;
    }

    std::string command = argv[1];

    std::string inputFile, outputFile;
    bool fix = false;

    for(int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-i") inputFile = argv[++i];
        if (arg == "-o") outputFile = argv[++i];
        if (arg == "-f") fix = true;
    }

    // Load file
    std::ifstream in(inputFile);

    std::vector<std::string> lines;
    std::string s;
    while (getline(in, s)) lines.push_back(s);

    if (command == "verify") {
        // Verify XML
    }
    else if (command == "format") {
        // Format XML
    }
    else if (command == "json") {
       // Convert XML to JSON
    }
    else if (command == "mini") {
        // Minify XML
    }
    else if(command == "compress"){
        // Compress XML
    }
    else if(command == "decompress"){
        // Decompress XML
    }
    else {
        std::cout << "Unknown command\n";
    }
}
