#include "include/GraphicalUserInterface.h"
// #include "include/FileIO.hpp"

int main(int argc, char *argv[]) {
    // For Testing FileIO
    // string data = FileIO::readXML("../data/sample.xml", SourceType::File);
    // FileIO::writeData("../data/testingOutput.xml", data, SourceType::File);


    if (argc > 1) {
        // TODO: run cli
    }
    return run_gui(argc, argv);
}