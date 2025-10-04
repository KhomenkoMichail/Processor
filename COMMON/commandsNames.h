#ifndef COMANDS_NAMES_H
#define COMANDS_NAMES_H

const int version = 4;
const int signature = 0xDED;
const int END_OF_COMMANDS = 0xEDA;

enum comandsNames {
    PUSHcmd = 1,
    ADDcmd = 2,
    SUBcmd = 3,
    MULcmd = 4,
    DIVcmd = 5,
    POWcmd = 6,
    SQRTcmd = 7,
    OUTcmd = 8,
    HLTcmd = 9,
    REALLOC_DOWNcmd = 10,
    ERROR_COMMANDcmd = 12,
    INcmd = 13,
    JMPcmd = 20,
    PUSHREGcmd = 33,
    POPREGcmd = 52,
};

#endif
