#include "../include/CommandLineInterface.hpp"
#include <iostream>
#include <vector>
#include <fstream>

int CommandLineInterface::run(int argc, char* argv[]) {

    if (argc < 3) {
        std::cout << "Invalid command\n";
        return 0;
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
    string content = FileIO::readXML(inputFile, SourceType::File);
    string output;

    if (command == "verify") {
        // Verify XML
    }
    else if (command == "format") {
        output = XMLFormatter::format(content);
    }
    else if (command == "json") {
       // Convert XML to JSON
    }
    else if (command == "mini") {
        output = XMLMinifier::minify(content);
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

    FileIO::writeData(outputFile, output, SourceType::File);
    return 0;
}