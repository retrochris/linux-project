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
