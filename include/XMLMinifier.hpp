#ifndef MINIFIER_HPP
#define MINIFIER_HPP

#include <string>

using namespace std;

class XMLMinifier {
public:
    // removes whitespace between tags
    static string minify(const string& xml);
};

#endif