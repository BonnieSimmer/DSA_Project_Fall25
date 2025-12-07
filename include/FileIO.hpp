#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>

using namespace std;

enum class SourceType {
    File,
    GUI
};


class FileIO {
public:
    // The input and output paths are relative to cmake-build-debug/
    string readXML(const string& input, SourceType sourceType);
    string writeData(const string& output, const std::string& data, SourceType sourceType);
};

#endif