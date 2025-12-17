//
// Created by menna on 12/17/2025.
 //#include "include/XMLDecompression.hpp"

#include "XMLDecompression.hpp"
#include <fstream>
#include <iostream>
DNode* XMLDecompressor::buildTree() {
    priority_queue<DNode*, vector<DNode*>, DCompare> pq;

    for (auto& [ch, freq] : freqMap) {
        pq.push(new DNode(ch, freq));
    }

    if (pq.size() == 1) {
        DNode* single = pq.top(); pq.pop();
        return new DNode('\0', single->freq, single, nullptr);
    }

    while (pq.size() > 1) {
        DNode* left = pq.top(); pq.pop();
        DNode* right = pq.top(); pq.pop();
        pq.push(new DNode('\0', left->freq + right->freq, left, right));
    }

    return pq.top();
}

void XMLDecompressor::deleteTree(DNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

void XMLDecompressor::decompress(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
//    ofstream out(outputFile);
    ofstream out(outputFile, ios::binary);


    //if (!in || !out) return;
    if (!in) { cerr << "Error opening input file.\n"; return; }
    if (!out) { cerr << "Error opening output file.\n"; return; }

    int mapSize;
    long long totalChars;

    in.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
    in.read(reinterpret_cast<char*>(&totalChars), sizeof(totalChars));

    freqMap.clear();
    for (int i = 0; i < mapSize; i++) {
         int freq;
       //  char ch;
      //   in.read(&ch, sizeof(char));
        ////
        unsigned char ch;
        in.read(reinterpret_cast<char*>(&ch), sizeof(unsigned char));
        /////
        in.read(reinterpret_cast<char*>(&freq), sizeof(int));
        freqMap[ch] = freq;
    }

    DNode* root = buildTree();
    DNode* curr = root;

    unsigned char byte;
    long long written = 0;
    while (in.read(reinterpret_cast<char*>(&byte), 1) && written < totalChars) {
    //while (in.get(byte) && written < totalChars) {
        for (int i = 7; i >= 0 && written < totalChars; i--) {
            int bit = (byte >> i) & 1;
            curr = (bit == 0) ? curr->left : curr->right;

            if (!curr->left && !curr->right) {
                out.put(curr->ch);
                curr = root;
                written++;
            }
        }
    }

    deleteTree(root);
    in.close();
    out.close();
}
