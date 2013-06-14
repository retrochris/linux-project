#include "iBot.h"

// Bot Konstruktor
iBot::iBot(string name, string password) {
    SQLLogging::InitSQLLogging("chatlog.sqlite");
    logging = false;
    this->name = name;
    this->password = password;
    connector = NULL;
}
// Bot Desktruktor
iBot::~iBot() {
    delete connector;
}
void iBot::Configure() {
    config = new ConfLoader();
    ConnectToServer(string(config->configData->host), config->configData->port, string(config->configData->channel));
}
void iBot::Configure(string host, int port, string channel) {
    config = new ConfLoader((char*)host.c_str(), port, (char*)channel.c_str());
    ConnectToServer(host, port, channel);
}

int iBot::ConnectToServer(string host, int port, string channel) {
    connector = new Connector(host, port);
    if (!connector->Connect())
        return -1;
    connector->Login(name, name, password, channel);
    BotLoop();
    connector->Disconnect();
    return 0;
}

// Hauptschleife des Bots
void iBot::BotLoop() {
    string message;
    while (true) {
        if (!connector->Receive(message))
            break;
        cout << message << endl;
        ParsePing(message);
        if (!PerformAction(message))
            break;
    }
}

void iBot::ParsePing(string message) {
    size_t pos = message.find("PING");
    if (pos != string::npos) {
        string pong("PONG" + message.substr(pos + 4));
        cout << pong << endl;
        connector->Send(pong);
    }
}

bool iBot::PerformAction(string buffer) {
    MsgParser data(buffer);
    int pos=0;

    if (logging) {
        stringstream msg;
        for(int i=0;i<(int)data.message.size();i++)
            msg << data.message[i] << " ";
        if (data.message.size() > 0)
            LogMessage(data.sender, msg.str());
    }

    if ((pos=data.atPosition("#logging")) != -1 && (int)data.message.size() > pos+1 ) {
        if (data.message[pos+1] == "on" ) {
            logging = true;
            connector->SendMessage("Loggin enabled");
        } else if( data.message[pos+1] == "off" ) {
            logging = false;
            connector->SendMessage("Loggin disabled");
        }
    } else if((pos=data.atPosition("#show_chatlog")) != -1 )
        ShowChatLog();
    else if ((pos=data.atPosition("#last_seen_user")) != -1 && data.message.size() > pos+1)
        ShowLastSeen(data.message[pos+1]);
    else if ((pos=data.atPosition("#bye")) != -1 ) {
        connector->SendMessage("PRIVMSG #" + connector->GetChannel() + " :bye!");
        connector->Send("QUIT bye");
        return false;
    } else if( (pos=data.atPosition("#topic")) != -1 && (int)data.message.size() > pos+1 )
        ChangeTopic(data.message[pos+1]);
    else if( (pos=data.atPosition("#nick")) != -1 && (int)data.message.size() > pos+1 ) {
        SetNickname(data.message[pos+1]);
    } else if( (pos=data.atPosition("#join")) != -1 && (int)data.message.size() > pos+1 )
        JoinChannel( data.message[pos+1] );
    else if( (pos=data.atPosition("#leave")) != -1 && (int)data.message.size() > pos+1 )
        LeaveChannel( data.message[pos+1] );
    return true;
}

void iBot::SetNickname(string nickname) {
    connector->Send("NICK " + nickname);
}
void iBot::SetUsername(string username) {
    connector->Send("USER " + username + " 0 0  :" + username);
}
void iBot::JoinChannel(string channel) {
    connector->SetChannel(channel);
}
void iBot::LeaveChannel(string channel) {
    connector->Send("PART #" + channel);
}
void iBot::ChangeTopic(string topic) {
    connector->Send("TOPIC #" + connector->GetChannel() + " " + topic);
}

void iBot::SetLogging(bool logging) {
    this->logging = logging;
}
void iBot::LogMessage(string name, string message) {
    int t = time(0);
    stringstream date; date << t;
    SQLLogging::AddLog(date.str().c_str(), name.c_str(), connector->GetChannel().c_str(), message.c_str());
}
void iBot::ClearChatLog() {
    SQLLogging::ClearLogTable();
}
void iBot::ShowChatLog() {
    string chatlog = SQLLogging::GetLog();
    if (!chatlog.empty())
        connector->SendMessage(chatlog);
    else
        connector->SendMessage("Empty Log...");
}
void iBot::ShowLastSeen(string name) {
    string date = SQLLogging::LastSeen(name.c_str());
    if (!date.empty())
        connector->SendMessage(date);
    else
        connector->SendMessage("sorry, never logged " + name);
}
