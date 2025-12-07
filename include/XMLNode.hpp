#ifndef XMLNODE_H
#define XMLNODE_H
#include <bits/stdc++.h>
#include <memory>

using namespace std;

class XMLNode : public enable_shared_from_this<XMLNode> {
public:
    string tagname;
    map<string, string> attributes;
    string text;
    vector<shared_ptr<XMLNode>>children;
    std::weak_ptr <XMLNode>parent;
    int lineNumber; //for error tracking

    XMLNode(const string &name, int line_num = 0);

    void setAttribute(const string &key, const string &value);
    void setText(const string &content);
    void addChild(shared_ptr<XMLNode> child);

    shared_ptr<XMLNode> getParent() const;
    vector<shared_ptr<XMLNode>>& getChildren();
    string getAttribute(const string& name) const;
    vector<shared_ptr<XMLNode>> getChildrenByTag(const string& tag_name) const;

};

#endif

