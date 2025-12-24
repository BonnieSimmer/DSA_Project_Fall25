/* ====================================================================================
XMLParser Class
- Load XML from file or string
- Builds a tree structure using XMLNode class
- Detect syntax errors 
- Fix XML errors
==================================================================================== */
#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include <stack>
#include <vector> 
#include <cctype>
#include "XMLNode.hpp"
#include "ParseError.hpp"
#include "FileIO.hpp"

// -------------- Post Class -------------- //
class Post{
public:
    std::string body;
    std::vector<std::string> topics;

    Post(): body(""), topics() {}
    const std::string& getBody() const { return body; }
    const std::vector<std::string>& getTopics() const { return topics; }
};
// -------------- Follower Class -------------- //
class Follower{
private:
    int id;
public:
    Follower() : id(0) {}
    Follower(int id) : id(id) {}
    int getId() const { return id; }
friend class XMLParser;
};
// -------------- User Class -------------- //
class User{
public:
    std::string name;
    int id;
    std::vector<Post> posts;
    std::vector<Follower> followers;

    User(): name(""), id(0), followers(), posts() {}
    User(const std::string& name, int id) : name(name), id(id) {}

    void setName(const std::string& newName) {
        name = newName;
    }

    void addPost(const Post& post) {
        posts.push_back(post);
    }
friend class XMLParser;

};
    
// ---------------- XMLParser Class ---------------- //
class XMLParser {
private:
    User user;
    Post post;
    Follower follower;
    void extractUsersFromTree(const std::shared_ptr<XMLNode>& root);
    static std::string trim(const std::string &str);
public:
    static string extractTagName(const std::string& tagContent);

    std::vector<User> users; 
    shared_ptr<XMLNode> parse(const string& input);
    vector<User> getUsers() const {
        return users;
    }
};

#endif
