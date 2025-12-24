#include "../include/ParseError.hpp"
#include <sstream>
#include <stack>
#include <vector>

vector<XMLError> ParseError::verify(const string& input, bool fix, string& outputResult) {
    vector<XMLError> errors;
    stack<TagInfo> tagStack; // Stores {tagName, lineNumber}
    int currentLine = 1;

    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == '\n') {
            currentLine++; continue;
        }

        if (input[i] == '<') {
            size_t endPos = input.find('>', i);
            if (endPos == string::npos) break;

            string content = input.substr(i + 1, endPos - i - 1);
            // To ignore comments, preprocessor tags and self-closing tags like in section
            if (content[0] == '?' || content[0] == '!' || content.back() == '/') {
                i = endPos; continue;
            }

            string tagName = getTagName(content);

            if (content[0] == '/') { // Closing Tag
                if (!tagStack.empty() && tagStack.top().name == tagName) {
                    tagStack.pop(); // Normal match
                } else {
                    bool foundInStack = false;
                    stack<TagInfo> temp = tagStack;

                    while (!temp.empty()) {
                        if (temp.top().name == tagName) {
                            foundInStack = true;
                            break;
                        }
                        temp.pop();
                    }

                    if (foundInStack) {
                        while (!tagStack.empty() && tagStack.top().name != tagName) {
                            errors.push_back({
                                tagStack.top().line,
                                tagStack.top().name,
                                "missing_closing for <" + tagStack.top().name + ">",
                                "missing_closing"
                            });
                            tagStack.pop(); // Sync the stack
                        }
                        tagStack.pop();
                    } else {
                        errors.push_back({
                            currentLine,
                            tagName,
                            "Extra closing tag </" + tagName + "> found (no matching opener)",
                            "extra_closer"
                        });
                    }
                }
            } else { // Opening Tag
                tagStack.push({tagName, currentLine});
            }
            i = endPos;
        }
    }
    // Capture remaining unclosed tags
    while (!tagStack.empty()) {
        errors.push_back({tagStack.top().line, tagStack.top().name, "missing_closing for <" + tagStack.top().name + ">", "missing_closing"});
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
    struct TagLine {
        string name;
        int line;
    };

    stack<string> s;
    stringstream ss(input);
    stringstream fixed;
    string line;
    int currentLine = 1;

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
            string name = getTagName(tag);

            if (tag[1] == '/') { // Closing
                if (!s.empty()) {
                    if (s.top() == name && s.top().name == name) {
                        processedLine += tag;
                        s.pop();
                    } else {
                        // Close unmatched tags until the matching one is found
                        stack<TagLine> tempStack;
                        bool found = false;

                        while (!s.empty()) {
                            if (s.top().name == name) {
                                found = true;
                                break;
                            }
                            tempStack.push(s.top());
                            s.pop();
                        }

                        //close unmatched tags
                        while(!tempStack.empty()) {
                            processedLine += "</" + tempStack.top() + ">";
                            tempStack.pop();
                        }

                        // Pop the matching tag if found
                        if (found) s.pop();
                        processedLine += tag; // Append current closing tag
                }
            } else { // Opening
                processedLine += tag;
                s.push(name, currentLine);
            }
            pos = end + 1;
        }
        fixed << processedLine << "\n";
        currentLine++;
    }

    // close any remaining tags in the stack
    while (!s.empty()) {
        fixed << "</" << s.top() << ">\n";
        s.pop();
    }
    return fixed.str();
}

string getTagName(string content) {
    if (content.empty()) {
        return "";
    }
    size_t start = (content[0] == '/') ? 1 : 0;
    size_t end = content.find_first_of(" \t\r\n", start);

    if (end == string::npos) {
        return content.substr(start);
    }
    return content.substr(start, end - start);
}