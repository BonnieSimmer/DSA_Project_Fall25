#include "../include/XMLNode.hpp"
    //constructor
XMLNode::XMLNode(const string &name, int line_num){
    tagname = name;
    lineNumber = line_num;
    //the rest of its fields are default-initialized as text = ""
}
void XMLNode::setAttribute(const string &key, const string &value){
    attributes[key] = value;
}
void XMLNode::setText(const string &content){
    text = content;
}
void XMLNode::setTag(const string &name){
    tagname = name;
}
void XMLNode::addChild(shared_ptr<XMLNode> child){
    //just for establish the parent child connection when creating new nodes in xmlParser
    child -> parent = shared_from_this();
    children.push_back(child);
}

shared_ptr<XMLNode> XMLNode::getParent() const {
    return parent.lock();
}
vector<shared_ptr<XMLNode>>& XMLNode::getChildren() {
    return children;
}
string XMLNode::getAttribute(const string& name) const {
    auto it = attributes.find(name);
    if (it != attributes.end()) {
        return it->second;
    }
    return ""; // Return empty string if attribute not found
}
vector<shared_ptr<XMLNode>> XMLNode::getChildrenByTag(const string& tag_name) const{
    vector<shared_ptr<XMLNode>> matchingChildren;

    for (const auto& child : children) {
        // Check if the child's tag name matches the requested tag
        if (child->tagname == tag_name) {
            matchingChildren.push_back(child);
        }
    }
    return matchingChildren;
}
