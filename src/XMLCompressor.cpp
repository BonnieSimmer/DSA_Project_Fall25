#include "XMLCompressor.hpp"
#include "XMLMinifier.hpp"
#include <fstream>
#include <iterator>

using namespace std;

Node::Node(char c, int f, Node* l, Node* r) {
    ch = c;
    freq = f;
    left = l;
    right = r;
}

//Overload () to return true if the frequency of l is greater than r
bool Compare::operator()(Node* l, Node* r) {
    return l->freq > r->freq;
}

// Generates binary codes from the Tree and populates huffmanCode map
void XMLCompressor::generateCodes(Node* root, string str) {
    if (!root) return;

    // Leaf node reached
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    generateCodes(root->left, str + "0");
    generateCodes(root->right, str + "1");
}

//delete tree memory after use
void XMLCompressor::deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

//Builds the Huffman Tree using the frequency map
Node* XMLCompressor::buildTree() {
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto const& [key, val] : freqMap) {
        pq.push(new Node(key, val));
    }

    if (pq.empty()) return nullptr;

    // Handle single character file: must have a root to define the code
    if (pq.size() == 1) {
        Node* single = pq.top(); pq.pop();
        return new Node('\0', single->freq, single, nullptr);
    }

    // Build the Huffman Tree by combining the two lowest frequency nodes
    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        int sum = left->freq + right->freq;
        pq.push(new Node('\0', sum, left, right)); // Internal node with dummy char
    }

    return pq.top();
}

//Writes the compressed file, including the header (frequency map) and the packed bits
void XMLCompressor::writeCompressedFile(const string& text, const string& outputFile) {
    ofstream out(outputFile, ios::binary);
    if (!out) { // Handle file open error silently or log it

        return;
    }

    //Write Map Size -> [Char, Freq] pairs for decompression
    int mapSize = freqMap.size();
    long long totalChars = text.length(); //needed for decompression

    out.write(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
    out.write(reinterpret_cast<char*>(&totalChars), sizeof(totalChars));//Write Total Character Count

    //write the frequency map
    for (auto const& [key, val] : freqMap) {
        out.write(&key, sizeof(char));
        out.write(reinterpret_cast<const char*>(&val), sizeof(int));
    }

    //Write the actual bits (Bit Packing)
    unsigned char buffer = 0;
    int bitCount = 0;

    for (char c : text) {
        // ensure character has a code
        if (huffmanCode.count(c) == 0) continue;

        const string& code = huffmanCode.at(c);
        for (char bit : code) {
            if (bit == '1') {
                buffer = buffer | (1 << (7 - bitCount)); // Set the bit
            }
            bitCount++;
            if (bitCount == 8) {
                out.put(buffer);
                buffer = 0;
                bitCount = 0;
            }
        }
    }

    // Write remaining partial byte (padding)
    if (bitCount > 0) {
        out.put(buffer);
    }

    out.close();
}

//Compresses the content of the inputFile (XML/JSON) and saves it to outputFile.
void XMLCompressor::compress(const string& inputFile, const string& outputFile) {
    // Read raw file
    ifstream in(inputFile, ios::binary);
    if (!in) {
        return;
    }
    string rawText((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    // Minify the XML (Pre-processing for maximum compression)
    XMLMinifier minifier;
    string text = minifier.minify(rawText);

    if (text.empty()) {
        return;
    }

    // Count Frequencies
    freqMap.clear();
    for (char c : text) {
        freqMap[c]++;
    }

    // Build Tree
    Node* root = buildTree();

    if (root) {
        // Generate Codes
        huffmanCode.clear();
        generateCodes(root, "");

        // Write compressed binary to file
        writeCompressedFile(text, outputFile);

        // Cleanup
        deleteTree(root);
    }
}
