#ifndef COMMAND_LINE_INTERFACE_HPP
#define COMMAND_LINE_INTERFACE_HPP

#include "FileIO.hpp"
#include "XMLFormatter.hpp"
#include "XMLMinifier.hpp"
#include "XMLCompressor.hpp"
#include "XMLDecompression.hpp"
#include "XMLToJson.hpp"
#include "GraphVisualizer.h"
#include "NetworkAnalyzer.hpp"
#include "PostSearch.hpp"

class CommandLineInterface {

public:
    static int run(int argc, char* argv[]);

private:
    static XMLCompressor compressor;
    static XMLDecompressor decompressor;
};

#endif
