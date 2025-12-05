#include<bits/stdc++.h>
#include<memory>

using namespace std;

class XMLNode : public enable_shared_from_this<XMLNode> {
public:
    string tagname;
    map<string, string> attributes;
    string text;
    vector<shared_ptr<XMLNode>>children;
    weakptr<XMLNode>parent;

    int lineNumber; //for error tracking

    //constructor
    XMLNode(string &name, int line_num = 0){
        tagname = name;
        lineNumber = line_num;
        //the rest of its fields are default-initialized as text = ""
    }
    void setAttribute(string &key, string &value){
        attributes[key] = value;
    }
    void setText(string &content){
        text = content;
    }
    void addChild(shared_ptr<XMLNode> child){
        //just for establish the parent child connection when creating new nodes in xmlParser
        child -> parent = shared_from_this();
        children.push_back(child);
    }

};
