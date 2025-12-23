# Project Idea
XML (Extensible Markup Language) is one of the most famous formats for storing and sharing
information among different devices. Some text editors, such as Sublime Text, are able to parse
such files and do some basic operations. In this project, you will work on developing a GUI
(Graphical User Interface) based program to parse and visualize an XML file.

In our system the XML file will represent users in a social network
Each user has id (unique), name, list of posts, list of followers.
Each post has a text and a list of topics.

# Project Structure
```
├── CMakeLists.txt
├── include/
│   ├── (all header files)
│   └── ...
├── src/
│   ├── (all source files)
│   └── ...
└── main.cpp
│
└── README.md

```
# Build & run
On windows
```
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

# Requirements
## Level 1
- Build a desktop application that can process XML files.
- The application should work in two modes:
  #### 1. Command line mode
     User can run the application through the command line using the command
     xml_editor followed by the action, input and output files locations (if needed)
     xml_editor verify -i input_file.xml
     xml_editor compress -i input_file.xml -o output_file.xml
  #### 2. GUI mode
     The GUI should allow the user to provide the input file either through a browse
     button or a text file in the application
     The GUI should allow the user to see the operations outputs in a read-only text field
     and also support saving the output file in a location determined by the user.
     The GUI should contain buttons for the user to do the required operations on the XML.


-  Building a GUI in which the user can specify the location of an input XML file. 
- Checking the XML consistency: The input XML may have inconsistencies like missing any
  of the closing and opening tags or not matching tags. 
- The program should be able to detect and visually show any errors in consistency. 
- The program will also be able to solve the errors.
- Formatting (Prettifying) the XML: the XML file should be well formatted by keeping the
  indentation for each level.
- Converting XML to JSON: JSON (JavaScript Object Notation) is another format used
  to represent data. It’s helpful to convert the XML into JSON, especially when using
  JavaScript as there are tons of libraries and tools that use JSON notation.
- Minifying the XML file: Since spaces and newlines (\n) are actually characters that can
  increase the size of an XML document. This feature should aim at decreasing the size of
  an XML file (compressing it) by deleting the white spaces and indentations. 
- Compressing the data in the XML/JSON file: You should come up with a way to reduce the
  size of the file using a data compression technique. You can invent your own adhoc
  method for such compression. On the other hand, you can check how JSON works and try
  to distill ideas from it. Finally, you can use a data compression technique such as byte pair
  encoding (https://en.wikipedia.org/wiki/Byte_pair_encoding).
  The smaller the output file is, the more efficient your algorithm is. 
- Decompressing the compressed data to return to its original format.


## Level 2
- representing the user's data using the graph data structure: the XML file will represent the
 users' data in a social network (their posts, followers, ..., etc.).
The user data is his id (unique across the network), name, list of his posts and followers.
So you should represent the relation between the followers using the graph data
structure as it will be very helpful for the network analysis.
- Network analysis: by representing the network using the graph data structure, we can
  extract some important data:
  - who is the most influencer user (has the most followers)
  - who is the most active user (connected to lots of users)
  - the mutual followers between n users
  - for each user, suggest a list of users to follow (the followers of his followers)
- Post search: given a specific word or topic, get the posts where this word or topic was
    mentioned
- Graph visualization: you’re free to use any tool or third party library to help you visualize the
  graph of the network showing how the user is connected to each other.