#include <stdio.h>
#include <assert.h>


#include "disassemblerFunctions.h"
#include "../COMMON/commandsNames.h"

void writeCommands (int* commandBuffer, const char* nameOfOutputFile) {
    assert(commandBuffer);
    assert(nameOfOutputFile);

    FILE* outputFile = fopen(nameOfOutputFile, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Error of opening file \"%s\"", nameOfOutputFile);
        perror("");
        return;
    }

    int errorCode = 0;

    for (size_t numOfElement = 0; commandBuffer[numOfElement] != END_OF_COMMANDS; numOfElement++) {
        switch (commandBuffer[numOfElement]) {

            case PUSHcmd:
                numOfElement++;
                if (commandBuffer[numOfElement] == END_OF_COMMANDS) {
                    printf("ERROR PUSH COMMAND! BAD OR NO PUSH VALUE!\n");
                    return;
                }
                fprintf(outputFile, "PUSH %d\n", commandBuffer[numOfElement]);
                break;

            case ADDcmd:
                fprintf(outputFile, "ADD\n");
                break;

            case SUBcmd:
                fprintf(outputFile, "SUB\n");
                break;

            case MULcmd:
                fprintf(outputFile, "MUL\n");
                break;

            case DIVcmd:
                fprintf(outputFile, "DIV\n");
                break;

            case POWcmd:
                fprintf(outputFile, "POW\n");
                break;

            case SQRTcmd:
                fprintf(outputFile, "SQRT\n");
                break;

            case OUTcmd:
                fprintf(outputFile, "OUT\n");
                break;

            case HLTcmd:
                fprintf(outputFile, "HLT\n");
                break;

            case REALLOC_DOWNcmd:
                fprintf(outputFile, "REALLOC_DOWN\n");
                break;

            default:
                printf("ERROR! UNKNOWN COMMAND: \"%d\" number %d\n", commandBuffer[numOfElement], numOfElement + 1);
                errorCode = 1;
                break;
        }

        if (errorCode)
            break;
    }

    if(fclose(outputFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", nameOfOutputFile);
        perror("");
    }
}
