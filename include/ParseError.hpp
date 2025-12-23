#ifndef PARSEERROR_HPP
#define PARSEERROR_HPP

#include <string>
#include <vector>
#include <stack>
#include <iostream>

using namespace std;

struct XMLError {
    int line;          // The line number where the error was detected
    string tagName;    // The name of the tag involved (e.g., "user", "id")
    string message;    // A human-readable description (e.g., "Missing closing tag")
    string type;       // A category for logic (e.g., "mismatch", "missing_open")
};
class ParseError{
public:
    // Main function for 'xml_editor verify -i input.xml' 
    static void verify(const string& input, bool fixFlag, string& outputResult);

private:
    static string extractTagName(const string& tag);
    // -f
    static string solveErrors(const string& input);
};

#endif
