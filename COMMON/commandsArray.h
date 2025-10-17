const int NUMBER_OF_COMMANDS = 26;


typedef int (*processorFunc_t)(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

struct command {
    const char* name;
    comandsNames commandCode;
    arguments argType;
    processorFunc_t commandFunc;
};

struct command comandsArray[NUMBER_OF_COMMANDS] = { {"", emptyString, noArg, NULL},
                                                    {"PUSH", PUSHcmd, numArg, &pushFunc},
                                                    {"ADD", ADDcmd, noArg, &calcFunc},
                                                    {"SUB", SUBcmd, noArg, &calcFunc},
                                                    {"MUL", MULcmd, noArg, &calcFunc},
                                                    {"DIV", DIVcmd, noArg, &calcFunc},
                                                    {"POW", POWcmd, noArg, &calcFunc},
                                                    {"SQRT", SQRTcmd, noArg, &sqrtFunc},
                                                    {"OUT", OUTcmd, noArg, &outFunc},
                                                    {"HLT", HLTcmd, noArg, &hltFunc},
                                                    {"PUSHREG", PUSHREGcmd, regArg, &pushFunc},
                                                    {"POPREG", POPREGcmd, regArg, &popFunc},
                                                    {"IN", INcmd, noArg, &inFunc},
                                                    {"JMP", JMPcmd, labelArg, &jumpAndCallFunc},
                                                    {"JB", JBcmd, labelArg, &comparingJumpFunc},
                                                    {"JBE", JBEcmd, labelArg, &comparingJumpFunc},
                                                    {"JA", JAcmd, labelArg, &comparingJumpFunc},
                                                    {"JAE", JAEcmd, labelArg, &comparingJumpFunc},
                                                    {"JE", JEcmd, labelArg, &comparingJumpFunc},
                                                    {"JNE", JNEcmd, labelArg, &comparingJumpFunc},
                                                    {"CALL", CALLcmd, labelArg, &jumpAndCallFunc},
                                                    {"RET", RETcmd, noArg, &retFunc},
                                                    {"PUSHM", PUSHMcmd, regArg, &pushFunc},
                                                    {"POPM", POPMcmd, regArg, &popFunc},
                                                    {"MOD", MODcmd, regArg, &calcFunc},
                                                    {"ERROR", ERROR_COMMANDcmd, noArg, NULL} };
