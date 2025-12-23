#include "../include/ParseError.hpp"
ParseError::ParseError(int l, int c, string msg){
    line = l;
    column = c;
    message = msg;
}

            string fullTag = line.substr(pos, endPos - pos + 1);
            string tagName = extractTagName(fullTag);

            if (fullTag[1] == '/') { // Closing Tag
                if (!tagStack.empty() && tagStack.top().first == tagName) {
                    tagStack.pop();
                } else {
                    errors.push_back({currentLine, tagName, "Mismatched or extra closing tag"});
                }
            } else if (fullTag.find("/>") == string::npos) { // Opening Tag
                tagStack.push({tagName, currentLine});
            }
            pos = endPos + 1;
        }
    }

    // Capture remaining unclosed tags
    while (!tagStack.empty()) {
        errors.push_back({tagStack.top().second, tagStack.top().first, "Missing closing tag"});
        tagStack.pop();
    }

    // Prepare Output 
    if (errors.empty()) {
        outputResult = "Output valid";
    } else {
        stringstream report;
        report << "Output invalid\nTotal Errors: " << errors.size() << "\n";
        for (const auto& e : errors) {
            report << "Line " << e.line << ": " << e.type << " for <" << e.tagName << ">\n";
        }
        
        // If -f is specified, fix and update outputResult 
        if (fix) {
            outputResult = solveErrors(input);
        } else {
            outputResult = report.str();
        }
    }
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
            string name = extractTagName(tag);

            if (tag[1] == '/') { // Closing
                if (!s.empty() && s.top() == name) {
                    processedLine += tag;
                    s.pop();
                } // Ignore mismatched closing tags to "fix" the structure
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