#include "../include/ParseError.hpp"
#include "../include/XMLParsser.hpp" //to use function (extractTagName)
#include <sstream>
#include <stack>
#include <vector>

vector<XMLError> ParseError::verify(const string& input, bool fix, string& outputResult) {
    vector<XMLError> errors;
    stack<pair<string, int>> tagStack; // Stores {tagName, lineNumber}
    stringstream ss(input);
    string line;
    int currentLine = 0;
    bool rootFound = false;

    while (getline(ss, line)) {
        currentLine++;
        size_t pos = 0;
        while ((pos = line.find('<', pos)) != string::npos) {
            size_t endPos = line.find('>', pos);
            if (endPos == string::npos) {
                errors.push_back({currentLine, "unknown", "Missing closing bracket '>'"});
                break;
            }

            string fullTag = line.substr(pos, endPos - pos + 1);
            string tagName = XMLParser::extractTagName(fullTag);

            if (tagName.empty()) {
                pos = endPos + 1;
                continue;
            }

            if (fullTag[1] == '/') { // Closing Tag
                if (!tagStack.empty() && tagStack.top().first == tagName) {
                    tagStack.pop();
                } else {
                    string expected = tagStack.empty() ? "none" : tagStack.top().first;
                    errors.push_back({currentLine, tagName, "Mismatched closing tag (expected </" + expected + ">)"});
                }
            }
            else if (fullTag.find("/>") != string::npos) { // Self-closing
                rootFound = true;
            }
            else { // Opening Tag
                rootFound = true;
                tagStack.push({tagName, currentLine});
            }
            pos = endPos + 1;
        }
    }

    if (!rootFound && !input.empty()) {
        errors.push_back({1, "Root", "Missing root container (e.g., <users>)"});
    }
    // Capture remaining unclosed tags
    while (!tagStack.empty()) {
        errors.push_back({tagStack.top().second, tagStack.top().first, "Tag opened but never closed"});
        tagStack.pop();
    }

    if (errors.empty()) {
        outputResult = "Success: XML is valid.";
    } else {
        if (fix) {
            outputResult = solveErrors(input);
        } else {
            stringstream report;
            report << "Invalid XML Structure\nTotal Errors: " << errors.size() << "\n";
            for (const auto& e : errors) {
                report << "Line " << e.line << ": " << e.type << " for <" << e.tagName << ">\n";
            }
            outputResult = report.str();
        }
    }
    return errors;
}

string ParseError::solveErrors(const string& input) {
    stack<string> s;
    stringstream ss(input);
    stringstream fixed;
    string line;

    while (getline(ss, line)) {
        string processedLine = "";
        size_t pos = 0;
        while (pos < line.length()) {
            size_t start = line.find('<', pos);
            if (start == string::npos) {
                processedLine += line.substr(pos);
                break;
            }
            processedLine += line.substr(pos, start - pos);
            size_t end = line.find('>', start);
            if (end == string::npos) break;

            string tag = line.substr(start, end - start + 1);
            string name = XMLParser::extractTagName(tag);

            if (tag[1] == '/') { // Closing
                if (!s.empty()) {
                    if (s.top() == name) {
                        processedLine += tag;
                        s.pop();
                    } else {
                        while(!s.empty() && s.top() != name) {
                            processedLine += "</" + s.top() + ">";
                            s.pop();
                        }
                        if(!s.empty()) {
                            processedLine += tag;
                            s.pop();
                        }
                    }
                }
            } else { // Opening
                processedLine += tag;
                s.push(name);
            }
            pos = end + 1;
        }
        fixed << processedLine << "\n";
    }

    // Force close any tags left open at the end of the file
    while (!s.empty()) {
        fixed << "</" << s.top() << ">\n";
        s.pop();
    }
    return fixed.str();
}