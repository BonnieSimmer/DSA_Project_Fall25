#include "../include/ParseError.hpp"
#include <sstream>
#include <stack>
#include <set>
#include <vector>

string ParseError::cachedFixedXML = "";

vector<XMLError> ParseError::verify(const string& input, bool fix, string& outputResult) {
    vector<XMLError> errors;
    stack<TagInfo> tagStack; // Stores {tagName, lineNumber}
    stringstream ss(input);
    string fullFixed;
    string line;
    int currentLine = 1;

    while (getline(ss, line)) {
        size_t pos = 0;
        string processedLine;

        while (pos < line.length()) {
            size_t start = line.find('<', pos);
            if (start == string::npos) {
                processedLine += line.substr(pos);
                break;
            }

            processedLine += line.substr(pos, start - pos);
            size_t end = line.find('>', start);
            if (end == string::npos) break;

            string fullTag = line.substr(start, end - start + 1);
            string name = getTagName(fullTag);

            if (fullTag[1] == '?' || fullTag[1] == '!' || fullTag[fullTag.length() - 2] == '/') {
                processedLine += fullTag;
            }
            else if (fullTag[1] == '/') { // Closing
                if (!tagStack.empty() && tagStack.top().name == name) {
                    processedLine += fullTag;
                    tagStack.pop();
                } else {
                    bool foundInStack = false;
                    stack<TagInfo> temp = tagStack;
                    while (!temp.empty()) {
                        if (temp.top().name == name) { foundInStack = true; break; }
                        temp.pop();
                    }

                    if (foundInStack) {
                        // Missing closed tag fix
                        while (!tagStack.empty() && tagStack.top().name != name) {
                            errors.push_back({tagStack.top().line, tagStack.top().name, "missing_closer"});
                            processedLine += "</" + tagStack.top().name + ">";
                            tagStack.pop();
                        }
                        processedLine += fullTag;
                        tagStack.pop();
                    } else {
                        // Missing open tag fix
                        errors.push_back({currentLine, name, "extra_closer"});
                        string childName = getChildName(name);
                        string currentContext = fullFixed + processedLine;
                        size_t insertionPos = currentContext.length();

                        if (!childName.empty()) {
                            // find the start of the child block
                            string childOpener = "<" + childName + ">";
                            size_t lastChild = currentContext.rfind(childOpener);
                            if (lastChild != string::npos) {
                                insertionPos = lastChild;
                                while (true) {
                                    size_t prevChild = currentContext.rfind(childOpener, insertionPos - 1);
                                    if (prevChild == string::npos) break;
                                    // Stop if there is a non-child tag in between
                                    string gap = currentContext.substr(prevChild, insertionPos - prevChild);
                                    if (gap.find("</") != string::npos && gap.find("</" + childName) == string::npos) break;
                                    insertionPos = prevChild;
                                }
                            }
                        } else {
                            // wrap the preceding text
                            size_t searchPos = currentContext.length() - 1;
                            while (searchPos > 0 && currentContext[searchPos] != '>') searchPos--;
                            insertionPos = (currentContext[searchPos] == '>') ? searchPos + 1 : 0;
                        }

                        currentContext.insert(insertionPos, "<" + name + ">");
                        fullFixed = currentContext;
                        processedLine = fullTag;
                    }
                }
            } else { // Opening
                if (!tagStack.empty() && isLeafTag(tagStack.top().name) && name != tagStack.top().name) {
                    errors.push_back({tagStack.top().line, tagStack.top().name, "missing_closer"});
                    processedLine += "</" + tagStack.top().name + ">";
                    tagStack.pop();
                }
                tagStack.push({name, currentLine});
                processedLine += fullTag;
            }
            pos = end + 1;
        }
        fullFixed += processedLine + (ss.eof() ? "" : "\n");
        currentLine++;
    }

    // close any remaining tags in the stack
    while (!tagStack.empty()) {
        fullFixed += "\n</" + tagStack.top().name + ">";
        tagStack.pop();
    }
    cachedFixedXML = fullFixed;

    if (errors.empty()) {
        outputResult = "Success: XML is valid.";
    } else {
        if (fix) {
            outputResult = cachedFixedXML; // Instant return
        } else {
            stringstream report;
            report << "Invalid XML: " << errors.size() << " errors found.\n";
            for (const auto& e : errors) report << "L" << e.line << ": " << e.type << " <" << e.tagName << ">\n";
            outputResult = report.str();
        }
    }

    return errors;
}

string ParseError::solveErrors(const string& input, const vector<XMLError>& errors) {
    if (errors.empty()) return input;
    return cachedFixedXML;
}

string getTagName(const string &content) {
    size_t start = 0;
    while (start < content.length() && (content[start] == '<' || content[start] == '/' || content[start] == ' '))
        start++;
    size_t end = content.find_first_of(" \t\r\n>", start);
    if (end == string::npos) return content.substr(start);
    return content.substr(start, end - start);
}

bool isLeafTag(const string& name) {
    static const std::set<string> leaves = {"id", "name", "topic", "body"};
    return leaves.contains(name);
}

string getChildName(const string& parentName) {
    if (parentName == "topics") return "topic";
    if (parentName == "followers") return "follower";
    if (parentName == "posts") return "post";
    if (parentName == "users") return "user";
    return "";
}