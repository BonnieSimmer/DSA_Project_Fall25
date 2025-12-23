#include "../include/GraphVisualizer.h"
#include "../include/AppMode.h"

#include <fstream>
#include <string>
#include <vector>
#include <QProcess>

using namespace std;

void GraphVisualizer::draw(const vector<User> &users, const string &outputPath) {
    std::string dotFilePath = "temp_graph.dot";

    if (generateDotFile(users, dotFilePath)) {
        renderGraph(dotFilePath, outputPath);
    }
}

bool GraphVisualizer::generateDotFile(const vector<User> &users, const string &filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << "digraph SocialNetwork {\n";
    file << "    node [shape=record, style=filled, fillcolor=lightblue];\n";
    file << "    rankdir=LR;\n";

    for (const auto& user : users) {
        file << "    " << user.id << " [label=\"ID: " << user.id
             << " | Name: " << user.name << "\"];\n";
    }

    for (const auto& user : users) {
        for (const auto& follower : user.followers) {
            file << "    " << user.id << " -> " << follower.getId() << ";\n";
        }
    }

    file << "}\n";
    file.close();
    return true;
}

void GraphVisualizer::renderGraph(const std::string& dotFile, const std::string& outFile) {
    QProcess process;
    process.start("dot", {"-V"});

    if (!process.waitForFinished(2000)) {
        reportError("Graphviz Not Found",
                    "The 'dot' executable was not found. Please install Graphviz to enable graph drawing.");
        return;
    }

    QStringList arguments;
    arguments << "-Tjpg" << QString::fromStdString(dotFile)
              << "-o" << QString::fromStdString(outFile);

    process.start("dot", arguments);

    if (!process.waitForFinished(-1)) {
        reportError("Rendering Failed",
                    "Graphviz started but failed to finish the image conversion.");
        return;
    }
    
    if (process.exitCode() != 0) {
        std::string stdErr = process.readAllStandardError().toStdString();
        reportError("Graphviz Error", "The dot tool returned an error: " + stdErr);
    }

    reportInfo("Success", "Graph saved to " + outFile);
}