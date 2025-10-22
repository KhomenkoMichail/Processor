#ifndef COMANDS_NAMES_H
#define COMANDS_NAMES_H

const int version = 9;
const int signature = 0xDED;
const int signSize = 3;

const int labelString = 98;
const int NUM_OF_REGS = 8;

enum commandsNames {
    emptyString = 0,
    PUSHcmd = 1,
    ADDcmd = 2,
    SUBcmd = 3,
    MULcmd = 4,
    DIVcmd = 5,
    POWcmd = 6,
    SQRTcmd = 7,
    OUTcmd = 8,
    HLTcmd = 9,
    PUSHREGcmd = 10,
    POPREGcmd = 11,
    INcmd = 12,
    JMPcmd = 13,
    JBcmd = 14,
    JBEcmd = 15,
    JAcmd = 16,
    JAEcmd = 17,
    JEcmd = 18,
    JNEcmd = 19,
    CALLcmd = 20,
    RETcmd = 21,
    PUSHMcmd = 22,
    POPMcmd = 23,
    MODcmd = 24,
    DRAWcmd = 25,
    UPDATEcmd = 26,
    ERROR_COMMANDcmd = 27,
};


enum arguments {
    noArg = 0,
    regArg = 1,
    labelArg = 2,
    numArg = 3,
    ramArg = 4,
};

typedef int (*processorFunc_t)(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

struct command {
    const char* name;
    commandsNames commandCode;
    arguments argType;
    processorFunc_t commandFunc;
    unsigned long long commandHash;
};

#endif
