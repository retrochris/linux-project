#include "ConfLoader.h"

ConfLoader::ConfLoader() {
    SetConfigFile();
}
ConfLoader::ConfLoader(char* host, int port, char* channel) {
    SetConfigFile(host, port, channel);
}
ConfLoader::~ConfLoader() {
    delete configData;
}
void ConfLoader::SetConfigFile() {
    const char* FILENAME = "config.cfg";

    // oeffne Config-File
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL)
        return;
    fseek(file, 0, SEEK_END);
    unsigned long length = ftell(file);
    rewind(file);
    char* buffer = (char*) malloc (sizeof(char)*length);
    if (buffer == NULL) {
        fclose(file);
        return;
    }
    unsigned long result = fread(buffer, 1, length, file);
    string configFile = string(buffer, 0, result);
    fclose(file);
    free(buffer);

    // Erstelle config, falls keine gefunden
    if (configFile.find("host") == string::npos ||
        configFile.find("port") == string::npos ||
        configFile.find("channel") == string::npos) {
        stringstream sstr;
        sstr << "host:irc.freenode.net\nport:6667\nchannel:RetroChannel\n";
        file = fopen(FILENAME, "w");
        if (file == NULL)
            return;
        fputs(sstr.str().c_str(), file);
        fclose(file);
        strcpy(configData->host, "irc.freenode.net");
        configData->port = 6667;
        strcpy(configData->channel, "RetroChannel");

    } else {
        size_t pos1 = configFile.find("host:")+5;
        size_t pos2 = configFile.find("\n")+1;
        string sHost = configFile.substr(pos1, pos2-pos1-1);
        pos1 = configFile.find("port:")+5;
        pos2 = configFile.find("\n", pos1);
        int sPort = atoi(configFile.substr(pos1, pos2-pos1).c_str());
        pos1 = configFile.find("channel:")+8;
        pos2 = configFile.find("\n", pos1);
        string sChannel = configFile.substr(pos1, pos2-pos1);
        strcpy(configData->host, sHost.c_str());
        configData->port = sPort;
        strcpy(configData->channel, sChannel.c_str());
    }
}
void ConfLoader::SetConfigFile(char* host, int port, char* channel) {
        const char* FILENAME = "config.cfg";
        FILE *file = fopen(FILENAME, "w");
        if (file == NULL)
            return;
        stringstream sstr;
        if (port != 0)
            sstr << "host:" << host << "\n" << "port:" << port << "\n" << "channel:" << channel << "\n";
        fputs(sstr.str().c_str(), file);
        fclose(file);
}
