#ifndef IRCCONNECTOR_H
#define IRCCONNECTOR_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <stdio.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <vector>
#include <sstream>

#define BUFFER_SIZE 1024

using namespace std;

class Connector {
public:
    Connector();
    Connector(string host, int port);
    ~Connector();

    // connection
    bool    Connect();
    void    Disconnect();

    void    Login(string username, string nickname, string password, string channel);
    void    IdentifyUser(string password);

    void    SendMessage(string message);
    string  ParseMessage(string buffer);

    void    Send(string message);
    bool    Receive(string &message);

    void    SetChannel(string channel);
    string  GetChannel();
private:

    // host parameter
    string  host;
    int     port;
    int     sock;

    string  channel;
};

#endif
