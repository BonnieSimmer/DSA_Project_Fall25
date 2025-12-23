#include <sstream>
#include "../include/FileIO.hpp"
#include "../include/AppMode.h"

#include <charconv>


string FileIO::readXML(const string &input, SourceType sourceType) {
    if (sourceType == SourceType::GUI) {
        return input;
    }

    if (sourceType == SourceType::File) {
        ifstream file(input);
        if (!file.is_open()) {
            reportError("Input Error","Cannot open input file: " + input);
            exit(1);
        }

        ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    reportError("Input Error","You forgot to add the source type!");
    exit(1);
}

string FileIO::writeData(const string &output, const string &data, SourceType sourceType) {
    if (sourceType == SourceType::GUI) {
        return data;
    }

    if (sourceType == SourceType::File) {
        if (output.empty()) {
            reportError("Output Error","No output file path provided.");
            exit(1);
        }

        ofstream file(output);
        if (!file.is_open()) {
            reportError("Output Error","Cannot open output file: " + output);
            exit(1);
        }

        file << data;
        return "File saved successfully.";
    }
    reportError("Output Error","You forgot to add the source type!");
    exit(1);
}

vector<int> FileIO::parseIds(const string& rawIds) {
    vector<int> ids;
    const char* first = rawIds.data();
    const char* last = rawIds.data() + rawIds.size();

    while (first < last) {
        while (first < last && *first == ' ') first++;
        if (first >= last) break;

        int value;
        auto [ptr, ec] = std::from_chars(first, last, value);

        if (ec == std::errc()) {
            ids.push_back(value);
            first = ptr;

            while (first < last && *first == ' ') first++;

            if (first < last && *first == ',') {
                first++;
            }
        } else {
            while (first < last && *first != ',') first++;
            if (first < last) first++;
        }
    }
    return ids;
}
