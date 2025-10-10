#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "textStructs.h"
#include "assemblerFunctions.h"
#include "../COMMON/commandsNames.h"

int* commandRewriter (struct comands* structAddress, int* labels, size_t* numOfBufferElements, const char* nameOfFile) {
    assert(structAddress);

    int* commandBuffer = (int*)calloc((structAddress->numberOfStrings)*2 + 4, sizeof(int));
    size_t numberOfSymbols = 3;

    char regNameString[5] = {};

    int commandNumber = 0;

    commandBuffer[0] = signature;
    commandBuffer[1] = signature;
    commandBuffer[2] = version;

    for (size_t line = 0; line < structAddress->numberOfStrings; line++) {
        char commandString[10] = {};
        int offset = 0;

        sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString, "%s%n", commandString, &offset);

        commandNumber = commandComparator(commandString);

        if (commandNumber == emptyString)
            continue;

        if (commandNumber == labelString) {
            int numOfLabel = 0;
            sscanf(commandString, ": %d", &numOfLabel);
            labels[numOfLabel] = numberOfSymbols - 3;
            continue;
        }

        commandBuffer[numberOfSymbols++] = commandNumber;

        if (commandNumber == ERROR_COMMANDcmd) {
            printf("ERROR! UNKNOWN COMMAND: \"%s\" ENTER ONLY AVAILABLE COMANDS! %s:%d\n", commandString, nameOfFile, (line+1));
            return NULL;
        }

        if (commandNumber == PUSHcmd) {
            if(sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + offset, "%d", &commandNumber) != 1) {
                printf("ERROR PUSH COMMAND! BAD OR NO PUSH VALUE! %s:%d\n", nameOfFile, (line+1));
                return NULL;
            }
            commandBuffer[numberOfSymbols++] = commandNumber;
            continue;
        }

        if ((commandNumber == POPREGcmd) || (commandNumber == PUSHREGcmd)) {
            if(sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + offset, "%s", regNameString) != 1) {
                printf("ERROR %s COMMAND! BAD OR NO %s VALUE! %s:%d\n",commandString, commandString, nameOfFile, (line+1));
                return NULL;
            }
            commandNumber = getNumberOfReg(regNameString);
            commandBuffer[numberOfSymbols++] = commandNumber;
            continue;
        }

        if ((commandNumber == JMPcmd) || (commandNumber == JBcmd) ||
            (commandNumber == JBEcmd) || (commandNumber == JAcmd) ||
            (commandNumber == JAEcmd) || (commandNumber == JEcmd) ||
            (commandNumber == JNEcmd)) {

                if((sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + offset, "%d", &commandNumber) != 1) || (commandNumber < 0)) {
                    if (sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + offset, " :%d", &commandNumber) == 1) {
                        commandBuffer[numberOfSymbols++] = labels[commandNumber];
                        continue;
                    }
                    else {
                        printf("ERROR %s COMMAND! BAD OR NO %s VALUE! %s:%d\n", commandString, commandString, nameOfFile, (line+1));
                        return NULL;
                    }
                }
            commandBuffer[numberOfSymbols++] = commandNumber;
            continue;
        }
    }

    commandBuffer[numberOfSymbols] = END_OF_COMMANDS;
    *numOfBufferElements = numberOfSymbols;
    return commandBuffer;
}

int commandComparator (char* command) {
    assert(command);

    if (strcmp(command, "") == 0)
        return emptyString;

    if (command[0] == ':')
        return labelString;

    if (strcmp(command, "PUSH") == 0)
        return PUSHcmd;

    if (strcmp(command, "ADD") == 0)
        return ADDcmd;

    if (strcmp(command, "SUB") == 0)
        return SUBcmd;

    if (strcmp(command, "MUL") == 0)
        return MULcmd;

    if (strcmp(command, "DIV") == 0)
        return DIVcmd;

    if (strcmp(command, "POW") == 0)
        return POWcmd;

    if (strcmp(command, "SQRT") == 0)
        return SQRTcmd;

    if (strcmp(command, "OUT") == 0)
        return OUTcmd;

    if (strcmp(command, "HLT") == 0)
        return HLTcmd;

    if (strcmp(command, "PUSHREG") == 0)
        return PUSHREGcmd;

    if (strcmp(command, "POPREG") == 0)
        return POPREGcmd;

    if (strcmp(command, "IN") == 0)
        return INcmd;

    if (strcmp(command, "JMP") == 0)
        return JMPcmd;

    if (strcmp(command, "JB") == 0)
        return JBcmd;

    if (strcmp(command, "JBE") == 0)
        return JBEcmd;

    if (strcmp(command, "JA") == 0)
        return JAcmd;

    if (strcmp(command, "JAE") == 0)
        return JAEcmd;

    if (strcmp(command, "JE") == 0)
        return JEcmd;

    if (strcmp(command, "JNE") == 0)
        return JNEcmd;

    return ERROR_COMMANDcmd;
}

void writeTextByteCode (int* codeBuffer, const char* nameOfFile) {
    assert(codeBuffer);
    assert(nameOfFile);

    FILE* byteCodeFile = fopen(nameOfFile, "w");
    if(byteCodeFile == NULL) {
        fprintf(stderr, "Error of opening file \"%s\"", nameOfFile);
        perror("");
        return;
    }

    for (size_t numOfElement = 0; codeBuffer[numOfElement] != END_OF_COMMANDS; numOfElement++)
        fprintf (byteCodeFile, "%d ", codeBuffer[numOfElement]);

    if(fclose(byteCodeFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", nameOfFile);
        perror("");
    }
}

void writeBinByteCode (int* codeBuffer, size_t numOfBufferElements, const char* nameOfFile) {
    assert(codeBuffer);
    assert(nameOfFile);

    FILE* byteCodeFile = fopen(nameOfFile, "wb");

    if(byteCodeFile == NULL) {
        fprintf(stderr, "Error of opening file \"%s\"", nameOfFile);
        perror("");
        return;
    }

    if (fwrite(codeBuffer, sizeof(int), numOfBufferElements, byteCodeFile) != numOfBufferElements) {
        fprintf(stderr, "Error of writing the file \"%s\"", nameOfFile);
        perror("");
    }

    if(fclose(byteCodeFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", nameOfFile);
        perror("");
    }
}

int getNumberOfReg(const char* nameOfReg) {
    assert(nameOfReg);

    char firstLetter = 0;
    sscanf(nameOfReg, "%c", &firstLetter);
    return (int)(firstLetter - 'A');
}

void commentsCleaner(char* str) {
    assert(str);

    char* commentPtr = NULL;
    while ((commentPtr = strchr(str, ';')) != NULL)
        *commentPtr = '\0';

    while ((commentPtr = strchr(str, '\n')) != NULL)
        *commentPtr = '\0';
}

/*
int getCmdREGvalue (struct comands* structAddress, int line, int offset, const char* nameOfFile, const char* commandString

    char regNameString[5] = {};

    if(sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + offset, "%s", regNameString) != 1) {
        printf("ERROR %s COMMAND! BAD OR NO %s VALUE! %s:%d\n",commandString, commandString, nameOfFile, (line+1));
        return 1;
    }
    int commandNumber = getNumberOfReg(regNameString);
    commandBuffer[numberOfSymbols++] = commandNumber;

    return 0;
}
*/
