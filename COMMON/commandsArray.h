#ifndef COMMANDS_ARRAY_H
#define COMMANDS_ARRAY_H

const int NUMBER_OF_COMMANDS = 27;


typedef int (*processorFunc_t)(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

struct command {
    const char* name;
    commandsNames commandCode;
    arguments argType;
    processorFunc_t commandFunc;
};

struct command comandsArray[NUMBER_OF_COMMANDS] = { { "",        emptyString,      noArg,     &errorMessageFunc  },
                                                    { "PUSH",    PUSHcmd,          numArg,    &pushFunc          },
                                                    { "ADD",     ADDcmd,           noArg,     &calcFunc          },
                                                    { "SUB",     SUBcmd,           noArg,     &calcFunc          },
                                                    { "MUL",     MULcmd,           noArg,     &calcFunc          },
                                                    { "DIV",     DIVcmd,           noArg,     &calcFunc          },
                                                    { "POW",     POWcmd,           noArg,     &calcFunc          },
                                                    { "SQRT",    SQRTcmd,          noArg,     &sqrtFunc          },
                                                    { "OUT",     OUTcmd,           noArg,     &outFunc           },
                                                    { "HLT",     HLTcmd,           noArg,     &hltFunc           },
                                                    { "PUSHREG", PUSHREGcmd,       regArg,    &pushFunc          },
                                                    { "POPREG",  POPREGcmd,        regArg,    &popFunc           },
                                                    { "IN",      INcmd,            noArg,     &inFunc            },
                                                    { "JMP",     JMPcmd,           labelArg,  &jumpAndCallFunc   },
                                                    { "JB",      JBcmd,            labelArg,  &comparingJumpFunc },
                                                    { "JBE",     JBEcmd,           labelArg,  &comparingJumpFunc },
                                                    { "JA",      JAcmd,            labelArg,  &comparingJumpFunc },
                                                    { "JAE",     JAEcmd,           labelArg,  &comparingJumpFunc },
                                                    { "JE",      JEcmd,            labelArg,  &comparingJumpFunc },
                                                    { "JNE",     JNEcmd,           labelArg,  &comparingJumpFunc },
                                                    { "CALL",    CALLcmd,          labelArg,  &jumpAndCallFunc   },
                                                    { "RET",     RETcmd,           noArg,     &retFunc           },
                                                    { "PUSHM",   PUSHMcmd,         regArg,    &pushFunc          },
                                                    { "POPM",    POPMcmd,          regArg,    &popFunc           },
                                                    { "MOD",     MODcmd,           regArg,    &calcFunc          },
                                                    { "DRAW",    DRAWcmd,          noArg,     &drawFunc          },
                                                    { "ERROR",   ERROR_COMMANDcmd, noArg,     &errorMessageFunc  } };


#endif
