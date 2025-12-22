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
    size_t pos = 0;
    std::stack<std::shared_ptr<XMLNode>> nodeStack; //to keep track of current node
    shared_ptr<XMLNode> root = nullptr;

    while ((pos = input.find('<', pos)) != string::npos) {
        size_t endTag = input.find('>', pos);
        if (endTag == string::npos) break;

        std::string fullTag = input.substr(pos, endTag - pos + 1);

        if (fullTag[1] == '/') { // Check if closing tag
            if (!nodeStack.empty()) {
                nodeStack.pop();
            }
            pos = endTag + 1;
        }
        else { // Opening tag
            string tagName = extractTagName(fullTag);
            auto newNode = std::make_shared<XMLNode>(tagName);

            // Handle root logic
            if (!root) root = newNode;
            if (!nodeStack.empty()) nodeStack.top()->addChild(newNode);

            // Check for text content before the next tag
            size_t nextOpen = input.find('<', endTag);
            if (nextOpen != string::npos && nextOpen > endTag + 1) {
                string text = input.substr(endTag + 1, nextOpen - endTag - 1);
                // Apply your trimming logic here...
                newNode->setText(trim(text));
            }

            // If not self-closing (/>), push to stack
            if (fullTag.find("/>") == string::npos) {
                nodeStack.push(newNode);
            }
            pos = endTag + 1;
        }
    }
    if(root) extractUsersFromTree(root);
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

std::string XMLParser::trim(const std::string& str) {
    const std::string whitespace = " \t\r\n";
    size_t first = str.find_first_not_of(whitespace);

    // If string is all whitespace
    if (first == std::string::npos) {
        return "";
    }

    size_t last = str.find_last_not_of(whitespace);
    return str.substr(first, (last - first + 1));
}