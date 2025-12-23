#ifndef XMLCOMPRESSOR_HPP
#define XMLCOMPRESSOR_HPP

#include <string>
#include <vector>
#include <queue>
#include <map>

using namespace std;

//using Huffman Coding

// Custom Node for the Huffman Tree
struct Node {
    char ch;
    int freq;
    Node *left;
    Node *right;

    Node(char c, int f, Node* l = nullptr, Node* r = nullptr);
};

// Comparator for the Priority Queue (Min-Heap)
struct Compare {
    bool operator()(Node* l, Node* r);
};

class XMLCompressor {
private:
    map<char, string> huffmanCode;
    map<unsigned char, int> freqMap;

    Node* buildTree();
    void generateCodes(Node* root, string str);
    void writeCompressedFile(const string& text, const string& outputFile);
    void deleteTree(Node* root);

public:
    void compress(const string& data, const string& outputFile);
};

#endif
