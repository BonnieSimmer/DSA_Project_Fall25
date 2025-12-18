#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "XMLNode.hpp"
#include "XMLParsser.hpp"

using namespace std;

void buildJsonFromTree(__shared_ptr<XMLNode> node, int level, string& jsonBuilder, bool isArrayElement){
    string indent(level *4, ' ');     //indentation for readability

    if(!isArrayElement)
        jsonBuilder += indent + "\"" + node->tagname + "\": ";
    else
        jsonBuilder += indent;

    //case 1:no children
    if(node->children.empty()){
        jsonBuilder += "\"" + node->text + "\"";
        return;
    }    

    //case2: array of children
    bool isArray = (node->children.size() > 1) && (node->children[0]->tagname == node->children[1]->tagname);

    if(!isArray){
        jsonBuilder += "{\n";
        for(size_t i=0; i<node->children.size(); ++i){
            buildJsonFromTree(node->children[i], level+1, jsonBuilder, false);

            if(i<node->children.size()-1) jsonBuilder += ",";   //adding comma after every elemnet except last one
                jsonBuilder += "\n";
        }
        jsonBuilder +=indent + "}";
    }
    else{
        jsonBuilder += "{\n" + indent + "    \"" + node->children[0]->tagname + "\": [\n";
        for (size_t i = 0; i < node->children.size(); ++i) {
            buildJsonFromTree(node->children[i], level + 2, jsonBuilder, true);
            
            if (i < node->children.size() - 1) jsonBuilder += ",";
            jsonBuilder += "\n";
        }
        jsonBuilder += indent + "    ]\n" + indent + "}";
    }
}
string convertXMLToJSON(const string& xmlContent) {
    XMLParser parser;
    shared_ptr<XMLNode> root = parser.parse(xmlContent);

    // Basic error handling for empty or failed parses
    if (!root) return "{}";

    // Start building the JSON string
    string finalJson = "{\n";
    buildJsonFromTree(root, 1, finalJson, false);
    finalJson += "\n}";
    
    return finalJson;
}



