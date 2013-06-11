#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <stdio.h>

#include <vector>
#include <algorithm>
#include <sstream>

#define BUFFER_SIZE 1024

using namespace std;

// Bot functions
void    initBot();
int     startBot();
void    serverConnect();
void    serverDisconnect();
void    sendToServer(const char *message);
void    serverPing(string message);
void    parseMessage(string message);
void    identifyBot();
void    botOptions(string message);

// Bot variables
char    *ircHost;
int     ircPort;
string  botName;
string  botPassword;
string  botChannel;

// Socket file descriptor
int sock;

int main(int argc, char **argv) {
    if (argc != 3) {
        perror("Es wurden zu wenige Argumente übergeben");
        exit(1);
    } return startBot(argv[1], argv[2]);
}

// initialize bot parameters
void initBot() {
    botName = "RetroBot";
    botPassword = "darkn1gger";
    botChannel = "RetroChannel";
}
int startBot(const char *host, int port) {
    ircHost = host;
    ircPort = port;

    serverConnect();
    identifyBot();
    while (1) {
        char message[BUFFER_SIZE+1] = {0};
        if (recv(sock, message, BUFFER_SIZE, 0) < 0) {
            perror("Fehler beim Empfangen...");
            serverDisconnect();
            exit(1);
        } parseMessage(message);
    } serverDisconnect();
    return 0;
}
void serverConnect() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket konnte nicht erstellt werden...");
        serverDisconnect();
        exit(1);
    }
    hostent *hp = gethostbyname(ircHost);
    if(!hp) {
        perror("Fehler beim Holen des Hostnamen");
        serverDisconnect();
        exit(1);
    }

    sockaddr_in server;
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    memcpy((char*)&server.sin_addr, hp->h_addr, hp->h_length);
    server.sin_port = htons(PORT);
    memset(&(server.sin_zero), 0, 8*sizeof(char));

    if (connect(sock, (sockaddr*) &server, sizeof(server)) == -1) {
        perror("Fehler beim Verbinden des Bots mit dem Server");
        serverDisconnect();
        exit(1);
    }
}
void serverDisconnect() {
    close(sock);
}
void sendToServer(const char *message) {
    send(sock, message, strlen(message), 0);
}
void serverPing(string message) {
    size_t pingPos = message.find("PING");
    if (pingPos != string::npos) {
        string pong("PONG"+message.substr(pingPos+4)+"\r\n");
        cout << pong;
        sendToServer(pong.c_str());
    }
}
void parseMessage(string message) {
    if (message.find("\r\n") == message.length()-2)
        message.erase(message.length()-2);
    serverPing(message);
    botOptions(message);
}
void identifyBot() {
    sendToServer(("NICK " + botName + "\r\n").c_str());
    sendToServer(("USER " + botName + " 0 0  :" + botName + "\r\n").c_str());
    sendToServer(("PRIVMSG NickServ IDENTIFY " + botPassword + "\r\n").c_str());
    sendToServer(("JOIN #" + botChannel + "\r\n").c_str());
    sendToServer(("PRIVMSG #" + botChannel + ":Hallo!\r\n").c_str());
}
void botOptions(string message) {

}
