#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>
#include <vector>

using namespace std;

enum class SourceType {
    File,
    GUI
};


class FileIO {
public:
    // The input and output paths are relative to cmake-build-debug/
    static string readXML(const string& input, SourceType sourceType);
    static string writeData(const string& output, const std::string& data, SourceType sourceType);
    static vector<int> parseIds(const string &rawIds);

};

#endif