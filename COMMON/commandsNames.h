#ifndef COMANDS_NAMES_H
#define COMANDS_NAMES_H

const int version = 9;
const int signature = 0xDED;

const int labelString = 98;

enum commandsNames {
    PUSHcmd = 1,
    ADDcmd = 2,
    SUBcmd = 3,
    MULcmd = 4,
    DIVcmd = 5,
    POWcmd = 6,
    SQRTcmd = 7,
    OUTcmd = 8,
    HLTcmd = 9,
    MODcmd = 10,
    ERROR_COMMANDcmd = 12,
    INcmd = 13,
    JMPcmd = 20,
    JBcmd = 21,
    JBEcmd = 22,
    JAcmd = 23,
    JAEcmd = 24,
    JEcmd = 25,
    JNEcmd = 26,
    PUSHREGcmd = 33,
    POPREGcmd = 52,
    PUSHMcmd = 81,
    POPMcmd = 82,
    CALLcmd = 91,
    RETcmd = 92,
    DRAWcmd = 95,
    emptyString = 99,
};

enum arguments {
    noArg = 0,
    regArg = 1,
    labelArg = 2,
    numArg = 3,
};

#endif
