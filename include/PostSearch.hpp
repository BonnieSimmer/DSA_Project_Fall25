/* ========================================================================
Post Search: 
1. Search Word: Writes a list of posts where the given word was mentioned
2. Search Topic: Writes a list of posts where the given topic was mentioned
======================================================================== */

#ifndef POSTSEARCH_HPP
#define POSTSEARCH_HPP

#include "XMLParsser.hpp"
#include <vector>
#include <string>
#include <unordered_map>

class PostSearch{
private:
    std::vector<User> users;
    std::unordered_map<std::string, std::vector<Post>> wordIndex;
    std::unordered_map<std::string, std::vector<Post>> topicIndex;

public:
    PostSearch() : users(), wordIndex(), topicIndex() {
        buildIndexes(); 
    } 
    PostSearch(const std::vector<User>& users);

    void buildIndexes();
    std::vector<std::string> extractWords(const std::string& text);
        
    // Search for posts containing a specific word in the body
    std::vector<Post> searchByWord(const std::string& word);
    
    // Search for posts containing a specific topic
    std::vector<Post> searchByTopic(const std::string& topic);
};

#endif