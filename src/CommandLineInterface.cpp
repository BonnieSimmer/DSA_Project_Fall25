#include "../include/CommandLineInterface.hpp"
#include <iostream>
#include <vector>

XMLCompressor CommandLineInterface::compressor;
XMLDecompressor CommandLineInterface::decompressor;

int CommandLineInterface::run(const int argc, char* argv[]) {

    if (argc < 3) {
        cout << "Invalid command\n";
        return 0;
    }

    string command = argv[1];

    string inputFile, outputFile, searchTerm;
    vector<int> ids;
    bool fix = false;
    bool is_word;

    for(int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-i" && i + 1 < argc) inputFile = argv[++i];
        if (arg == "-o" && i + 1 < argc) outputFile = argv[++i];
        if (arg == "-f" && i + 1 < argc) fix = true;
        if (arg == "-id" && i + 1 < argc) ids.push_back(atoi(argv[++i]));
        if (arg == "-w" && i + 1 < argc) {
            is_word = true;
            searchTerm = argv[++i];
        }
        if (arg == "-t" && i + 1 < argc) {
            is_word = false;
            searchTerm = argv[++i];
        }
        if (arg == "-ids" && i + 1 < argc) ids = FileIO::parseIds(argv[++i]);
    }

    string result;

    if(command == "decompress"){
        decompressor.decompress(inputFile,outputFile);
        return 0;
    }

    // Load file
    string content = FileIO::readXML(inputFile, SourceType::File);
    string output;

    if (command == "verify") {
        // result will be filled with either the error report OR the fixed XML
        validate(content, fix, result);

        if (!fix) {
            // CASE 1: No -f flag. Just print the report to console.
            std::cout << result << std::endl;
            return 0;
        }
        // CASE 2: -f flag is present.
        std::cout << "XML fixed successfully." << std::endl;
        output = result;
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
        if ( validate(content, false, result) ) {
            XMLParser parser;
            parser.parse(content);
            GraphVisualizer::draw(parser.users, outputFile);
            return 0;
        }
        cout << "XML Errors Found. Please fix them.\n";
    } else if (command == "most_active") {
        if ( validate(content, false, result) ) {
            XMLParser parser;
            parser.parse(content);
            NetworkAnalyzer analyzer(parser.users);
            cout << analyzer.MostActiveUser();
            return 0;
        }
        cout << "XML Errors Found. Please fix them.\n";
    } else if (command == "most_influencer") {
        if ( validate(content, false, result) ) {
        XMLParser parser;
        parser.parse(content);
        NetworkAnalyzer analyzer(parser.users);
        cout << analyzer.MostInfluencerUser();
        return 0;
        }
        cout << "XML Errors Found. Please fix them.\n";
    } else if (command == "mutual") {
        if (!ids.empty()) {
            if ( validate(content, false, result) ) {
            XMLParser parser;
            parser.parse(content);
            NetworkAnalyzer analyzer(parser.users);

            cout << analyzer.mutualFollowers(ids);
            return 0;
            }
            cout << "XML Errors Found. Please fix them.\n";
        }
        cout << "Please provide Ids to find mutual followers between them\n";
    } else if (command == "suggest") {
        if (!ids.empty()) {
            if ( validate(content, false, result) ) {
            XMLParser parser;
            parser.parse(content);
            NetworkAnalyzer analyzer(parser.users);

            cout << analyzer.suggestUsersToFollow(ids[0]) << endl;
            return 0;
            }
            cout << "XML Errors Found. Please fix them.\n";
        }
        cout << "Please provide the Id of the user you want suggestions for\n";

    } else if (command == "search") {
        if ( validate(content, false, result) ) {
            std::vector<Post> searchResults;
            XMLParser parser;
            parser.parse(content);
            PostSearch search(parser.users);
            if (is_word) {
                searchResults = search.searchByWord(searchTerm);
            } else {
                searchResults = search.searchByTopic(searchTerm);
            }

            if (!searchResults.empty()) {
                std::cout << "Found " << searchResults.size() << " post(s):\n";
                std::cout << std::string(50, '=') << "\n";

                for (const auto& post : searchResults) {
                    std::cout << "Content: " << post.getBody() << "\n";
                    std::cout << "Topics:  [ ";
                    const std::vector<std::string>& topics = post.getTopics();
                    for (size_t i = 0; i < topics.size(); ++i) {
                        std::cout << topics[i];
                        if (i < topics.size() - 1) {
                            std::cout << ", ";
                        }
                    }
                    std::cout << " ]\n";
                    std::cout << std::string(50, '-') << "\n";
                }
            } else {
                std::cout << "No posts found matching \"" << searchTerm << "\".\n";
            }
        }
        cout << "XML Errors Found. Please fix them.\n";
    } else {
        cout << "Unknown command\n";
    }

    // 4. Save to file (Only if output is not empty and outputFile was provided)
    if (!outputFile.empty() && !output.empty()) {
        FileIO::writeData(outputFile, output, SourceType::File);
    }
    return 0;
}

bool validate(string& content, bool fix,string& result) {
    if (vector<XMLError> errors = ParseError::verify(content, fix, result); errors.empty()) {
        return true;
    }
    return false;
}
