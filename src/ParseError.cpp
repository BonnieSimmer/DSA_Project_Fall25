#include "../include/ParseError.hpp"
ParseError::ParseError(int l, int c, string msg){
    line = l;
    column = c;
    message = msg;
}

int ParseError::getLine() const {return line;}
int ParseError::getCol() const {return column;}
string ParseError::getMessage() const {return message;}

void ParseError::setLine(int l) {line = l;}
void ParseError::setCol(int c){column = c;}
void ParseError::setMessage(string msg){message = msg;}

string ParseError::applyFix(){
    int errorIndex = message.find("Missing closing tag");
    if(errorIndex != -1){
        int start = message.find("\"");
        int end = message.find("\"",start+1);

        if(start != -1 && end != -1){
            string tag = message.substr(start +1, end -start -1);
            return "</" + tag + ">";
        }
    }
    return "";
}
