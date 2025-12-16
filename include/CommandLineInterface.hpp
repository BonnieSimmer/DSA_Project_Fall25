#ifndef COMMAND_LINE_INTERFACE_HPP
#define COMMAND_LINE_INTERFACE_HPP

#include "FileIO.hpp"
#include "XMLFormatter.hpp"
#include "XMLMinifier.hpp"

class CommandLineInterface {

public:
    static int run(int argc, char* argv[]);
};

#endif
