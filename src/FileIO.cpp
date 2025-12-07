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

// ----------------------------
// Read preview (GUI)
// ----------------------------
// Reads only the first 'maxLines' lines of a file.
// Used by the GUI to show a short preview of large XML files
// without loading the entire content into memory.
//
// Returns the preview text as a single string.
bool FileIO::exists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

string FileIO::readPreview(const string &path, int maxLines) {
    if (!exists(path)) {
        throw runtime_error("Preview file does not exist: " + path);
    }

    
    ifstream file(path);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file for preview: " + path);
    }

    string line;
    string output;
    int count = 0;

    while (getline(file, line) && count < maxLines) {
        output += line + "\n";
        count++;
    }

    return output;
}



