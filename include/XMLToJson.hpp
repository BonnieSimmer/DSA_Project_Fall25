#ifndef XML_TO_JSON_HPP
#define XML_TO_JSON_HPP

#include <string>
#include <memory>

/*
 * Main function to convert an XML string into a formatted JSON string.
 * This function utilizes the project's XMLParser to build a tree structure and then traverses it to generate JSON.
 */
std::string convertXMLToJSON(const std::string& xmlContent);

#endif 
