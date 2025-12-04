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
    static string readXML(const string& input, SourceType sourceType);
    static string writeData(const string& output, const std::string& data, SourceType sourceType);
};

#endif