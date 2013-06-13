#include "iBot.h"

int main(int argc, char **argv) {
    iBot *ircBot = new iBot("RetroBot", "d4rknigger");

    switch (argc) {
    case 1:
        ircBot->Configure();
        break;
    case 4:
        ircBot->Configure(argv[1], atoi(argv[2]), argv[3]);
        break;
    default:
        perror("Unzulaessige Anzahl an Parameter");
        exit(1);
        break;
    };
    delete ircBot;
    return 0;
}
