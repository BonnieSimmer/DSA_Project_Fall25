#include "../include/PostSearch.hpp" 
#include <sstream>

// Extract words from text.
std::vector<std::string> PostSearch::extractWords(const std::string& text) {
    std::vector<std::string> words;
    std::stringstream ss(text);
    std::string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}

// Build hash indexes for words and topics
void PostSearch::buildIndexes() {
    wordIndex.clear(); 
    topicIndex.clear(); 
    
    // Iterate through all users and their posts.
    for (const auto& user : users) { 
        for (const auto& post : user.posts) { 
            // Extract all words from the post's body.
            std::vector<std::string> words = extractWords(post.getBody()); 
            for (const auto& word : words) {
                wordIndex[word].push_back(post);
            }
            // Index by topics.
            for (const auto& topic : post.getTopics()) { 
                topicIndex[topic].push_back(post);
            }
        }
    }
}

// Constructor that takes a vector of users and builds the indexes.
PostSearch::PostSearch(const std::vector<User>& users) : users(users) {
    buildIndexes();
}

// Posts search with a specific word
std::vector<Post> PostSearch::searchByWord(const std::string& word) {

    auto it = wordIndex.find(word); 
    if (it != wordIndex.end()) {
        return it->second;
    }
    // If the word not found, return an empty vector of posts.
    return std::vector<Post>(); 
}

// posts search with a topic
std::vector<Post> PostSearch::searchByTopic(const std::string& topic) {
    auto it = topicIndex.find(topic); 
    if (it != topicIndex.end()) { 
        return it->second; 
    }
    
    return std::vector<Post>();
}
