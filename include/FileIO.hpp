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

    // helper: check file existence
    static bool exists(const std::string& path);

    // read first maxLines lines for preview
    static std::string readPreview(const std::string& path, int maxLines = 200);
};

#endif