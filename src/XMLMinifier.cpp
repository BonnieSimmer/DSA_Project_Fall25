/**
* remove ALL unnecessary spaces
* remove indentation
* removes unnecessary newlines
* preserves text inside tags
**/

//isspace is a function that checks if the string has whitespaces only or not
// whitespaces --> space or \n or \t or \r

#include "../include/XMLMinifier.hpp"
#include <string>
#include <cctype>

using namespace std;

    string XMLMinifier::minify(const string& xml) { //receive full XML
        string result; //store the output "minified XML"

        bool insideTag = false; //for reading character between < and >
        bool previousWasTagClose = false; //for detecting text node

        for (int  i = 0; i < xml.size(); ++i) {
            char c = xml[i];

            //opening a tag '<' [ inside the tag ]
            if (c == '<') {
                 insideTag = true;

                 // Remove whitespace before <
                 while (!result.empty() && isspace(result.back()))
                     result.pop_back();

                 result.push_back(c);
             }
             //End of a tag
            else if (c == '>') {
                 insideTag = false; // Exit tag
                 result.push_back(c);
                 previousWasTagClose = true; //tag closed
            }

            else {
                // Characters inside a tag
                 if (insideTag) {
                     //  Remove all whitespace if it is not whitespace push it to result
                     if (!isspace((unsigned char)c))
                         result.push_back(c);
                 }
                // Characters outside a tag
                 else {
                     if (previousWasTagClose) {
                          previousWasTagClose = false;
                          result.push_back(c);
                     }
                     else {
                              result.push_back(c);
                     }
                 }
             }
        }

       return result;

    }