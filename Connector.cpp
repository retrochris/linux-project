#include "Connector.h"

Connector::Connector() {
    host = "irc.freenode.net";
    port = 6667;
    channel = "";
}
Connector::Connector(string host, int port) {
    this->host = host;
    this->port = port;
    channel = "";
}
Connector::~Connector() {
}

bool Connector::Connect() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Failed to create socket...");
        Disconnect();
        return false;
    }
    hostent *hp = gethostbyname(host.c_str());
    if (!hp) {
        perror("Failed to get hostname...");
        Disconnect();
        return false;
    }

    sockaddr_in sin;
    memset((char*)&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    memcpy((char*)&sin.sin_addr, hp->h_addr, hp->h_length);
    sin.sin_port = htons(port);
    memset(&(sin.sin_zero), 0, 8*sizeof(char));

    if (connect(sock, (sockaddr*) &sin, sizeof(sin)) == -1) {
        perror("Failed to connect socket...");
        Disconnect();
        return false;
    } return true;
}
void Connector::Disconnect() {
    close(sock);
}

void Connector::Login(string username, string nickname, string password, string channel) {
    Send("NICK " + nickname);
    Send("USER " + username + " 0 0  :" + username);
    IdentifyUser(password);
    SetChannel(channel);
}
void Connector::IdentifyUser(string password) {
    Send("PRIVMSG NickServ IDENTIFY " + password);
}

void Connector::SendMessage(string message) {
    stringstream msg_stream;
    msg_stream << "PRIVMSG #" << channel << " :" << message;
    Send(msg_stream.str());
}
string Connector::ParseMessage(string buffer) {
    if (buffer.find("\r\n") == buffer.length()-2)
        buffer.erase(buffer.length()-2);
    return buffer;
}

void Connector::Send(string message) {
    stringstream msg_stream;
    msg_stream << message << "\r\n";
    send(sock, msg_stream.str().c_str(), msg_stream.str().length(), 0);
}
bool Connector::Receive(string &message) {
    char buffer[BUFFER_SIZE+1] = {0};
    if (recv(sock, buffer, BUFFER_SIZE*sizeof(char), 0) < 0) {
        perror("Failed to receive message...");
        return false;
    }
    message = ParseMessage(string(buffer));
    return true;
}
void Connector::SetChannel(string channel) {
    this->channel = channel;
    Send("JOIN #" + channel);
}
string Connector::GetChannel() {
    return this->channel;
}
