#include "../include/XMLFormatter.hpp"
#include <sstream>
using namespace std;

static string indentSpaces(int level) {
    return string(level * 4, ' ');
}

string XMLFormatter::format(const string& xml) {
    stringstream formatted;
    int indent = 0;
    int i = 0;
    int n = xml.length();

    while (i < n) {

        if (xml[i] == ' ' || xml[i] == '\n' || xml[i] == '\t') {
            i++;
            continue;
        }


        if (xml[i] == '<') {
            int tagEnd = xml.find('>', i);
            if (tagEnd == string::npos)
                break;

            string tag = xml.substr(i, tagEnd - i + 1);


            if (tag[1] == '/') {
                indent--;
                formatted << indentSpaces(indent) << tag << "\n";
            }
            else if (tag[tag.length() - 2] == '/') {
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

            if (!text.empty()) {
                formatted << indentSpaces(indent) << text << "\n";
            }
            i = textEnd;
        }
    }

    return formatted.str();
}