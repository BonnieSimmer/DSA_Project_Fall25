#ifndef XMLNODE_H
#define XMLNODE_H
#include <vector>
#include <map>
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
    void setAttributes(const map<string, string> &attr){
        attributes = attr;
    }
    void setText(const string &content);
    void setTag (const string &name);
    void addChild(shared_ptr<XMLNode> child);

    shared_ptr<XMLNode> getParent() const;
    vector<shared_ptr<XMLNode>>& getChildren();
    string getAttribute(const string& name) const;
    vector<shared_ptr<XMLNode>> getChildrenByTag(const string& tag_name) const;

};

#endif
