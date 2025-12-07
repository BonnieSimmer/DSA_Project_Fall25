#include <sstream>
#include "../include/FileIO.hpp"



string FileIO::readXML(const string &input, SourceType sourceType) {
    if (sourceType == SourceType::GUI) {
        return input;
    }

    if (sourceType == SourceType::File) {
        ifstream file(input);
        if (!file.is_open()) {
            throw runtime_error("Cannot open input file: " + input);
        }

        ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    throw runtime_error("You forgot to add the source type!");
}

string FileIO::writeData(const string &output, const string &data, SourceType sourceType) {
    if (sourceType == SourceType::GUI) {
        return data;
    }

    if (sourceType == SourceType::File) {
        if (output.empty()) {
            throw runtime_error("No output file path provided.");
        }

        ofstream file(output);
        if (!file.is_open()) {
            throw runtime_error("Cannot open output file: " + output);
        }

        file << data;
        return "File saved successfully.";
    }

    throw runtime_error("You forgot to add the source type!");
}



