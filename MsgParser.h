#ifndef MsgParser_H
#define MsgParser_H

#include <stdio.h>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>

using namespace std;

class MsgParser {
public:
    MsgParser(string buffer);
    ~MsgParser();

    string sender;
    vector<string> message;

    int atPosition(string msgVar);
};


#endif
