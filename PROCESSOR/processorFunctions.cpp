#include <stdio.h>
#include <stdlib.h>

#include "processorFunctions.h"
#include "stackFunctions.h"
#include "structsAndEnums.h"
#include "calcFunctions.h"
#include "../COMMON/commandsNames.h"
#include "../COMMON/commonFunctions.h"

void processorCtor (struct spu* processor, const char* nameOfByteCodeFile) {
    assert(processor);
    assert(nameOfByteCodeFile);

    struct info stackInfo = {};
    STACK_CTOR(processor->stk, stackInfo, 10);

    processor->commandCode = makeCommandBuffer(nameOfByteCodeFile);
}


void executeBufferCommands (struct spu* processor, FILE* dumpFile, struct info* dumpInfo, const char* nameOfBinCodeFile) {
    assert(dumpFile);
    assert(dumpInfo);

    int errorCode = noErrors;

    for (size_t numOfElement = 0; (processor->commandCode)[numOfElement] != END_OF_COMMANDS; numOfElement++) {
        switch ((processor->commandCode)[numOfElement]) {

            case PUSHcmd:
                numOfElement++;
                if ((processor->commandCode)[numOfElement] == END_OF_COMMANDS) {
                    fprintf(dumpFile, "ERROR PUSH COMMAND!  NO PUSH VALUE!\n");
                    printf("ERROR PUSH COMMAND! BAD OR NO PUSH VALUE!\n");
                    return;
                }
                STACK_PUSH(&(processor->stk), (processor->commandCode)[numOfElement], dumpFile, dumpInfo);
                break;

            case ADDcmd:
                errorCode = stackAdd(&(processor->stk), dumpFile, dumpInfo);
                break;

            case SUBcmd:
                errorCode = stackSub(&(processor->stk), dumpFile, dumpInfo);
                break;

            case MULcmd:
                errorCode = stackMul(&(processor->stk), dumpFile, dumpInfo);
                break;

            case DIVcmd:
                errorCode = stackDiv(&(processor->stk), dumpFile, dumpInfo);
                break;

            case POWcmd:
                errorCode = stackPow(&(processor->stk), dumpFile, dumpInfo);
                break;

            case SQRTcmd:
                errorCode = stackSqrt(&(processor->stk), dumpFile, dumpInfo);
                break;

            case OUTcmd:
                errorCode = stackOut(&(processor->stk), dumpFile, dumpInfo);
                break;

            case HLTcmd:
            case REALLOC_DOWNcmd:
                errorCode = 1;
                break;

            default:
                printf("ERROR! UNKNOWN COMMAND: \"%d\" number %d from %s\n", (processor->commandCode)[numOfElement], numOfElement + 1, nameOfBinCodeFile);
        }

        if (errorCode)
            break;
    }
}
