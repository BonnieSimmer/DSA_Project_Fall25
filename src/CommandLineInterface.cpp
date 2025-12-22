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
    vector<int> ids;
    bool fix = false;
    bool is_word;

    for(int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-i") inputFile = argv[++i];
        if (arg == "-o") outputFile = argv[++i];
        if (arg == "-f") fix = true;
        if (arg == "-id") ids.push_back(atoi(argv[++i]));
        if (arg == "-w") is_word = true;
        if (arg == "-t") is_word = false;
        if (arg == "-ids" && i + 1 < argc) ids = FileIO::parseIds(argv[++i]);
    }

    if(command == "decompress"){
        decompressor.decompress(inputFile,outputFile);
        return 0;
    }

    // Load file
    string content = FileIO::readXML(inputFile, SourceType::File);
    string output;

    if (command == "verify") {
        // TODO Verify XML
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
    } else if (command == "draw") {
        XMLParser parser;
        parser.parse(content);
        GraphVisualizer::draw(parser.users, outputFile);
        return 0;
    } else if (command == "most_active") {
        XMLParser parser;
        parser.parse(content);
        NetworkAnalyzer analyzer(parser.users);
        analyzer.MostActiveUser();
        return 0;
    } else if (command == "most_influencer") {
        XMLParser parser;
        parser.parse(content);
        NetworkAnalyzer analyzer(parser.users);
        analyzer.MostInfluencerUser();
        return 0;
    } else if (command == "mutual") {
        if (!ids.empty()) {
            XMLParser parser;
            parser.parse(content);
            NetworkAnalyzer analyzer(parser.users);

            for (auto mutual = analyzer.mutualFollowers(ids); const auto& u : mutual) {
                cout << u.id << " " << u.name << endl;
            }
            return 0;
        }
        std::cout << "Please provide Ids to find mutual followers between them\n";
    } else if (command == "suggest") {
        if (!ids.empty()) {
            XMLParser parser;
            parser.parse(content);
            NetworkAnalyzer analyzer(parser.users);

            for (auto suggested = analyzer.suggestUsersToFollow(2); const auto& u : suggested) {
                cout << u.id << " " << u.name << endl;
            }
            return 0;
        }
        std::cout << "Please provide the Id of the user you want suggestions for\n";

    } else if (command == "search") {
        // TODO implement search
    } else {
        std::cout << "Unknown command\n";
    }

    FileIO::writeData(outputFile, output, SourceType::File);
    return 0;
}