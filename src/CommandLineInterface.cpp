#include "../include/CommandLineInterface.hpp"
#include "ParseError.hpp"
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
        if (arg == "-i" && i + 1 < argc) inputFile = argv[++i];
        if (arg == "-o" && i + 1 < argc) outputFile = argv[++i];
        if (arg == "-f") fix = true;
    }


    // Load file
    string content = FileIO::readXML(inputFile, SourceType::File);
    string output;
    string result;

    if (command == "verify") {
        
    // result will be filled with either the error report OR the fixed XML
    ParseError::verify(content, fix, result);

    if (!fix) {
        // CASE 1: No -f flag. Just print the report to console.
        std::cout << result << std::endl;
        return 0; 
    } else {
        // CASE 2: -f flag is present. 
        std::cout << "XML fixed successfully." << std::endl;
        output = result; 
    }
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

    // 4. Save to file (Only if output is not empty and outputFile was provided)
    if (!outputFile.empty() && !output.empty()) {
        FileIO::writeData(outputFile, output, SourceType::File);
    }
    return 0;
}