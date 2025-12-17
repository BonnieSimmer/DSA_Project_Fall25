//
// Created by menna on 12/17/2025.
//

#ifndef DSA_PROJECT_FALL25_XMLDECOMPRESSION_H
#define DSA_PROJECT_FALL25_XMLDECOMPRESSION_H

#endif //DSA_PROJECT_FALL25_XMLDECOMPRESSION_H
#ifndef XML_DECOMPRESSOR_HPP
#define XML_DECOMPRESSOR_HPP

#include <string>
#include <unordered_map>
#include <queue>
#include <map>

using namespace std;

struct DNode {
    char ch;
    int freq;
    DNode* left;
    DNode* right;

    DNode(char c, int f, DNode* l = nullptr, DNode* r = nullptr)
        : ch(c), freq(f), left(l), right(r) {}
};

// struct DCompare {
//     bool operator()(DNode* a, DNode* b) {
//         return a->freq > b->freq;
//     }
// };
struct DCompare {
    bool operator()(DNode* a, DNode* b) {
        if (a->freq == b->freq)
            return a->ch > b->ch;   // tie-breaker مهم جدًا
        return a->freq > b->freq;
    }
};


class XMLDecompressor {
private:
    map <unsigned char, int> freqMap;

    DNode* buildTree();
    void deleteTree(DNode* root);

public:
    void decompress(const string& inputFile, const string& outputFile);
};

#endif
