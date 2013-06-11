#ifndef IRCBOT_H
#define IRCBOT_H

#include "Connector.h"
#include "MsgParser.h"

class iBot {
public:
    iBot(string name, string password);
    ~iBot();

    void    Configure();

private:
    string  name;
    string  password;

    bool logging;

    Connector    *connector;
    ConfLoader    *config;

    void    BotLoop();

    void    ParsePing(string message);
    bool    PerformAction(string message);
    void    SetLogging(bool logging);
    void    LogMessage(string name, string message);
    void    ClearChatLog();
    void    ShowChatLog();
    void    ShowLastSeen(string name);
    void    SetNickname(string nickname);
    void    SetUsername(string username);
    void    JoinChannel(string channel);
    void    LeaveChannel(string channel);
    void    ChangeTopic(string topic);
};

#endif
