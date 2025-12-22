//
// Created by bonniesimmer on 12/21/25.
//

#ifndef DSA_PROJECT_FALL25_GRAPHVISUALIZER_H
#define DSA_PROJECT_FALL25_GRAPHVISUALIZER_H
#include <vector>

#include "XMLParsser.hpp"

using namespace std;

class GraphVisualizer {
public:
    static void draw(const vector<User>& users, const string& outputPath);
private:
    static bool generateDotFile(const vector<User>& users, const string& filename);
    static void renderGraph(const string& dotFile, const string& outFile);
};

#endif //DSA_PROJECT_FALL25_GRAPHVISUALIZER_H