#include "../include/CommandLineInterface.hpp"
#include <iostream>
#include <vector>

XMLCompressor CommandLineInterface::compressor;
XMLDecompressor CommandLineInterface::decompressor;

int CommandLineInterface::run(const int argc, char* argv[]) {

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

    if(command == "decompress"){
        decompressor.decompress(inputFile,outputFile);
        return 0;
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
       output = convertXMLToJSON(content);
    }
    else if (command == "mini") {
        output = XMLMinifier::minify(content);
    }
    else if(command == "compress"){
        compressor.compress(content,outputFile);
        return 0;
    }
    else {
        std::cout << "Unknown command\n";
    }

    FileIO::writeData(outputFile, output, SourceType::File);
    return 0;
}