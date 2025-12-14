// -----------------For Testing XMLFormatter-------------------
// #include "include/XMLFormatter.hpp"
// #include "include/FileIO.hpp"
// #include <iostream>
//
// int main() {
//     string XML = FileIO::readXML("data/unformatted.xml",SourceType::File);
//     string formattedXML = XMLFormatter::format(XML);
//     FileIO::writeData("data/formatted.xml", formattedXML , SourceType::File);
//     return 0;
// }
// -----------------For Testing XMLMinifier-------------------
// #include <iostream>
// #include "include/XMLMinifier.hpp"
// #include "include/FileIO.hpp"
// using namespace std;
// int main() {
//     string xml = FileIO::readXML("data/sample.xml", SourceType::File);
//
//     string minified = XMLMinifier::minify(xml);
//
//     FileIO::writeData("data/minified.xml", minified, SourceType::File);
//
//     cout << "Minified successfully! Check testingOutput.xml\n";
//
//     return 0;
// }

// -----------------For Testing XMLNode-------------------
//
//#include "include/XMLNode.hpp"
//#include <iostream>
//#include <memory>
//
//using namespace std;
//
//// Helper to create a shared pointer easily
//shared_ptr<XMLNode> createNode(const string& tagName, int line = 0) {
//    return make_shared<XMLNode>(tagName, line);
//}
//
//int main() {
//    // BUILD THE TREE STRUCTURE
//
//    // Root: <SocialNetwork id="2025" name="GeminiNet">
//    auto rootNode = make_shared<XMLNode>("SocialNetwork", 1);
//    rootNode->setAttribute("id", "2025");
//    rootNode->setAttribute("name", "GeminiNet");
//
//    // Child 1: <User uid="A1">
//    auto userA = make_shared<XMLNode>("User", 2);
//    userA->setAttribute("uid", "A1");
//
//    // Grandchild 1.1: <Name>Alice</Name>
//    auto nameA = make_shared<XMLNode>("Name", 3);
//    nameA->setText("Alice Johnson");
//    userA->addChild(nameA);
//
//    // Grandchild 1.2: <Post status="New">
//    auto post1 = make_shared<XMLNode>("Post", 4);
//    post1->setAttribute("status", "New");
//    post1->setText("First post!");
//    userA->addChild(post1);
//
//    // Grandchild 1.3: <Post status="Old">
//    auto post2 = make_shared<XMLNode>("Post", 5);
//    post2->setAttribute("status", "Old");
//    post2->setText("Second post.");
//    userA->addChild(post2);
//
//    rootNode->addChild(userA);
//
//    // Child 2: <User uid="B2">
//    auto userB = make_shared<XMLNode>("User", 6);
//    userB->setAttribute("uid", "B2");
//    rootNode->addChild(userB);
//
//
//    // Test getAttribute
//    cout << "Root Node ID: " << rootNode->getAttribute("id") << "\n";
//    cout << "Post 1 Status Attribute: " << post1->getAttribute("status") << "\n";
//
//    if (nameA->getParent()) {
//        cout << "Name node's parent tag: " << nameA->getParent()->tagname << "\n"; //Expected User
//    } else {
//        cout << "Name node has no parent (ERROR)\n";
//    }
//    cout << "Name A's text content: " << nameA->getChildren().empty() << "\n"; //Expected 1
//    cout << "Name A's text: " << nameA->text << "\n"; //Expected Alice Johnson
//    auto users = rootNode->getChildrenByTag("User");
//    cout << "Found " << users.size() << "\n"; //Expected 2
//    if (!users.empty()) {
//        cout << "  First User's UID: " << users[0]->getAttribute("uid") << "\n";
//    }
//    auto posts = userA->getChildrenByTag("Post");
//    cout << "User A has " << posts.size() << "\n" ; //expected 2
//    if (posts.size() == 2) {
//        cout << "  Post 1 content: " << posts[0]->text << "\n";
//        cout << "  Post 2 content: " << posts[1]->text << "\n";
//    }
//    auto comments = userA->getChildrenByTag("Comment");
//    cout << "User A has " << comments.size() << "\n" ;//expected 0
//
//}

// -----------------For Testing FileIO-------------------

//#include "include/GraphicalUserInterface.h"
// #include "include/FileIO.hpp"

//int main(int argc, char *argv[]) {
//    // string data = FileIO::readXML("../data/sample.xml", SourceType::File);
//    // FileIO::writeData("../data/testingOutput.xml", data, SourceType::File);
//
//
//    if (argc > 1) {
//        // TODO: run cli
//    }
//    return run_gui(argc, argv);
//}