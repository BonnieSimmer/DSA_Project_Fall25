#include "../include/XMLParsser.hpp"
#include <sstream>
#include <iostream>

// --------  Extract tag name from tag string ------------------- //
string XMLParser::extractTagName(const std::string& tagContent) {
    std::string name;
    for(auto c: tagContent){
        if(c == '<')    continue;
        if(c == '>' || c == '/' || c == ' ' || c == '\t')   break;
        else name += c;
    }   
    return name;
}

// -------- Parse XML from file or string ------------------- //
shared_ptr<XMLNode> XMLParser::parse(const string& input){
    std::istringstream stream(input);
    std::string line;
    
    std::stack<std::shared_ptr<XMLNode>> nodeStack; //to keep track of current node
    shared_ptr<XMLNode> root = nullptr;
    int lineNumber = 0;
    
    while (std::getline(stream, line)) {
        lineNumber++;
        
        if(line.empty()) continue;
        
        // Find first non-whitespace character
        size_t firstNonSpace = line.find_first_not_of(" \t\r\n");
        if(firstNonSpace == string::npos) continue;
        
        std::string trimmedLine = line.substr(firstNonSpace);
        
        if(trimmedLine[0] != '<') continue;
        // Check if closing tag
        if(trimmedLine[1] == '/'){
            if(!nodeStack.empty()){
                nodeStack.pop();
            }
            continue;
        }
        
        // Extract tag name
        std::string tagName = extractTagName(trimmedLine);
        if(tagName.empty()) continue;
        
        auto newNode = std::make_shared<XMLNode>(tagName, lineNumber);
        
        // Extract text content between opening and closing tags on same line
        size_t openEnd = trimmedLine.find('>');
        size_t closeStart = trimmedLine.rfind('<');
        
        if(openEnd != string::npos && closeStart != string::npos && openEnd < closeStart){
            string content = trimmedLine.substr(openEnd + 1, closeStart - openEnd - 1);

            // Trim content
            size_t contentStart = content.find_first_not_of(" \t\n\r");
            size_t contentEnd = content.find_last_not_of(" \t\n\r");
            if(contentStart != string::npos){
                content = content.substr(contentStart, contentEnd - contentStart + 1);
            }
            newNode->setText(content);
        }
        
        // Build tree structure
        if(root == nullptr){
            root = newNode;
            nodeStack.push(root);
        } else if(!nodeStack.empty()){
            nodeStack.top()->addChild(newNode);
            
            // Check if self-closing (has content on same line or ends with />)
            bool selfClosing = (trimmedLine.find("/>") != string::npos) || 
                             (openEnd != string::npos && closeStart != string::npos && closeStart > openEnd);
            
            if(!selfClosing)
                nodeStack.push(newNode);
        }
    }
    // Extract users from the tree
    if(root)    
        extractUsersFromTree(root);
    return root;
}

// -------- Extract users from the XML tree ------------------- //
void XMLParser::extractUsersFromTree(const std::shared_ptr<XMLNode>& root) {
    if(!root) return;   
    // Find all <user> tags
    vector<shared_ptr<XMLNode>> userNodes = root->getChildrenByTag("user");    
    for(const auto& userNode : userNodes){
        User user;
        
        // Extract id
        vector<shared_ptr<XMLNode>> idNodes = userNode->getChildrenByTag("id");
        if(!idNodes.empty() && !idNodes[0]->text.empty()){
            user.id = std::stoi(idNodes[0]->text);
        }
        
        // Extract name
        vector<shared_ptr<XMLNode>> nameNodes = userNode->getChildrenByTag("name");
        if(!nameNodes.empty()){
            user.setName(nameNodes[0]->text);
        }
        
        // Extract posts
        vector<shared_ptr<XMLNode>> postsContainers = userNode->getChildrenByTag("posts");
        if(!postsContainers.empty()){
            vector<shared_ptr<XMLNode>> postNodes = postsContainers[0]->getChildrenByTag("post");
            
            for(const auto& postNode : postNodes){
                Post post;
                
                // Get body
                vector<shared_ptr<XMLNode>> bodyNodes = postNode->getChildrenByTag("body");
                if(!bodyNodes.empty()){
                    post.body = bodyNodes[0]->text;
                }
                
                // Get topics
                vector<shared_ptr<XMLNode>> topicsContainers = postNode->getChildrenByTag("topics");
                if(!topicsContainers.empty()){
                    vector<shared_ptr<XMLNode>> topicNodes = topicsContainers[0]->getChildrenByTag("topic");
                    for(const auto& topicNode : topicNodes){
                        if(!topicNode->text.empty()){
                            post.topics.push_back(topicNode->text);
                        }
                    }
                }
                
                user.addPost(post);
            }
        }
        // Extract followers
        vector<shared_ptr<XMLNode>> followersContainers = userNode->getChildrenByTag("followers");
        if(!followersContainers.empty()){
            vector<shared_ptr<XMLNode>> followerNodes = followersContainers[0]->getChildrenByTag("follower");
            
            for(const auto& followerNode : followerNodes){
                vector<shared_ptr<XMLNode>> followerIdNodes = followerNode->getChildrenByTag("id");
                if(!followerIdNodes.empty() && !followerIdNodes[0]->text.empty()){
                    Follower f(std::stoi(followerIdNodes[0]->text));
                    user.followers.push_back(f);
                }
            }
        }
        users.push_back(user);
    }
}       