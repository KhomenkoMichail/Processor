#include <stdio.h>
#include <stdlib.h>

#include "processorFunctions.h"
#include "stackFunctions.h"
#include "structsAndEnums.h"
#include "calcFunctions.h"
#include "../COMMON/commandsNames.h"

const int END_OF_COMMANDS = 0xEDA;

int* makeCommandBuffer (const char* nameOfBinCodeFile) {
    assert(nameOfBinCodeFile);

    const size_t MAX_BUFFER_SIZE = 500;

    int* commandBuffer = (int*)calloc(MAX_BUFFER_SIZE, sizeof(int));
    assert(commandBuffer);

    FILE* binCodeFile = fopen(nameOfBinCodeFile, "rb");
    if (binCodeFile == NULL) {
        fprintf(stderr, "Error of opening file \"%s\"", nameOfBinCodeFile);
        perror("");
        return NULL;
    }

    size_t numOfReadSymbols = fread(commandBuffer, sizeof(int), MAX_BUFFER_SIZE - 1, binCodeFile);
    commandBuffer[numOfReadSymbols] = END_OF_COMMANDS;

    if(numOfReadSymbols == MAX_BUFFER_SIZE - 1)
        fprintf(stderr,"ERROR! The command buffer is full, increase it or call fewer commands!\n");

    if(fclose(binCodeFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", nameOfBinCodeFile);
        perror("");
    }

    return commandBuffer;
}

void executeBufferCommands (stack_t* stack, FILE* dumpFile, struct info* dumpInfo, int* commandBuffer, const char* nameOfBinCodeFile) {
    assert(dumpFile);
    assert(dumpInfo);

    int errorCode = noErrors;

    for (size_t numOfElement = 0; commandBuffer[numOfElement] != END_OF_COMMANDS; numOfElement++) {
        switch (commandBuffer[numOfElement]) {

            case PUSHcmd:
                numOfElement++;
                if (commandBuffer[numOfElement] == END_OF_COMMANDS) {
                    fprintf(dumpFile, "ERROR PUSH COMMAND!  NO PUSH VALUE!\n");
                    printf("ERROR PUSH COMMAND! BAD OR NO PUSH VALUE!\n");
                    return;
                }
                STACK_PUSH(stack, commandBuffer[numOfElement], dumpFile, dumpInfo);
                break;

            case ADDcmd:
                errorCode = stackAdd(stack, dumpFile, dumpInfo);
                break;

            case SUBcmd:
                errorCode = stackSub(stack, dumpFile, dumpInfo);
                break;

            case MULcmd:
                errorCode = stackMul(stack, dumpFile, dumpInfo);
                break;

            case DIVcmd:
                errorCode = stackDiv(stack, dumpFile, dumpInfo);
                break;

            case POWcmd:
                errorCode = stackPow(stack, dumpFile, dumpInfo);
                break;

            case SQRTcmd:
                errorCode = stackSqrt(stack, dumpFile, dumpInfo);
                break;

            case OUTcmd:
                errorCode = stackOut(stack, dumpFile, dumpInfo);
                break;

            case HLTcmd:
            case REALLOC_DOWNcmd:
                errorCode = 1;
                break;

            default:
                printf("ERROR! UNKNOWN COMMAND: \"%d\" number %d from %s\n", commandBuffer[numOfElement], numOfElement + 1, nameOfBinCodeFile);
        }

        if (errorCode)
            break;
    }
}

