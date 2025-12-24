#ifndef PARSEERROR_HPP
#define PARSEERROR_HPP

#include <string>
#include <vector>
#include <stack>
#include <iostream>

using namespace std;

struct TagInfo {
    string name;
    int line;
};

struct XMLError {
    int line;          // The line number where the error was detected
    string tagName;    // The name of the tag involved (e.g., "user", "id")
    string message;    // A human-readable description (e.g., "Missing closing tag")
    string type;       // A category for logic (e.g., "mismatch", "missing_open")
};
class ParseError{
public:
    static string cachedFixedXML;
    // Main function for 'xml_editor verify -i input.xml'
    static vector<XMLError> verify(const string& input, bool fix, string& outputResult);
    // -f
    static string solveErrors(const string& input, const vector<XMLError>& errors);
};

string getTagName(const string &content);
bool isLeafTag(const string& name);
string getChildName(const string& parentName);
bool isWhitespace(const string& s);

#endif
