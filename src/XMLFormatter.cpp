#include "../include/XMLFormatter.hpp"
#include <sstream>

using namespace std;

static string indentSpaces(int level) {
    return (level > 0) ? string(level * 4, ' ') : "";
}

string XMLFormatter::format(const string& xml) {
    stringstream formatted;
    int indent = 0;
    int i = 0;
    int n = xml.length();

    while (i < n) {
        if (isspace(static_cast<unsigned char>(xml[i]))) {
            i++;
            continue;
        }

        if (xml[i] == '<') {
            size_t tagEnd = xml.find('>', i);
            if (tagEnd == string::npos) break;

            string tag = xml.substr(i, tagEnd - i + 1);

            if (tag.size() > 1 && tag[1] == '/') {
                indent--;
                formatted << indentSpaces(indent) << tag << "\n";
            } else if (tag.size() > 2 && tag[tag.length() - 2] == '/') {
                formatted << indentSpaces(indent) << tag << "\n";
            }
            else {
                formatted << indentSpaces(indent) << tag << "\n";
                indent++;
            }

            i = tagEnd + 1;
        }

        else {
            int textEnd = xml.find('<', i);
            string text = xml.substr(i, textEnd - i);

            size_t first = text.find_first_not_of(" \t\n\r");
            if (first != string::npos) {
                size_t last = text.find_last_not_of(" \t\n\r");
                formatted << indentSpaces(indent) << text.substr(first, (last - first + 1)) << "\n";
            }
            i = textEnd;
        }
    }

    return formatted.str();
}