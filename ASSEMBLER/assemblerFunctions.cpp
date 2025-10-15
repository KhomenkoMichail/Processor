#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "structAssembler.h"
#include "textStructs.h"
#include "assemblerFunctions.h"
#include "../COMMON/commandsNames.h"
#include "textTools.h"

void assemblerCtor (struct assembler* Asm, const char* inputFile, const char* outputTextFile, const char* outputBinFile) {
    assert(Asm);
    assert(inputFile);
    assert(outputTextFile);
    assert(outputBinFile);

    Asm->nameOfInputFile = inputFile;
    Asm->nameOfOutputTextFile = outputTextFile;
    Asm->nameOfOutputBinFile = outputBinFile;

    getStructComands (&(Asm->cmds), inputFile);
    Asm->numOfLine = 0;

    for (size_t line = 0; line < (Asm->cmds).numberOfStrings; line++)
        commentsCleaner(((Asm->cmds).arrOfStringStructs[line]).ptrToString);

    Asm->commandCounter = 0;
    for (size_t numOfLabel = 0; numOfLabel < NUM_OF_LABELS; numOfLabel++)
        (Asm->labels)[numOfLabel] = -1;
}

void assemblerDtor (struct assembler* Asm) {
    assert(Asm);

    free(Asm->commandBuffer);
    free((Asm->cmds).text);
    free((Asm->cmds).arrOfStringStructs);
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

    if (strcmp(command, "CALL") == 0)
        return CALLcmd;

    if (strcmp(command, "RET") == 0)
        return RETcmd;

    if (strcmp(command, "PUSHM") == 0)
        return PUSHMcmd;

    if (strcmp(command, "POPM") == 0)
        return POPMcmd;

    if (strcmp(command, "MOD") == 0)
        return MODcmd;

    return ERROR_COMMANDcmd;
}

void writeTextByteCode (struct assembler* Asm) {
    assert(Asm);

    FILE* byteCodeFile = fopen(Asm->nameOfOutputTextFile, "w");
    if(byteCodeFile == NULL) {
        fprintf(stderr, "Error of opening file \"%s\"", Asm->nameOfOutputTextFile);
        perror("");
        return;
    }

    for (size_t numOfElement = 0; (Asm->commandBuffer)[numOfElement] != END_OF_COMMANDS; numOfElement++)
        fprintf (byteCodeFile, "%d ", (Asm->commandBuffer)[numOfElement]);

    if(fclose(byteCodeFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", Asm->nameOfOutputTextFile);
        perror("");
    }
}

void writeBinByteCode (struct assembler* Asm) {
    assert(Asm);

    FILE* byteCodeFile = fopen(Asm->nameOfOutputBinFile, "wb");

    if(byteCodeFile == NULL) {
        fprintf(stderr, "Error of opening file \"%s\"", Asm->nameOfOutputBinFile);
        perror("");
        return;
    }

    if (fwrite(Asm->commandBuffer, sizeof(int), Asm->commandCounter, byteCodeFile) != Asm->commandCounter) {
        fprintf(stderr, "Error of writing the file \"%s\"", Asm->nameOfOutputBinFile);
        perror("");
    }

    if(fclose(byteCodeFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", Asm->nameOfOutputBinFile);
        perror("");
    }
}

int getNumberOfReg(const char* nameOfReg) {
    assert(nameOfReg);

    char firstLetter = 0;
    sscanf(nameOfReg, "%c", &firstLetter);

    if (firstLetter == '[')
        sscanf(nameOfReg + 1, "%c", &firstLetter);

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

int commandRewriter(struct assembler* Asm) {
    assert(Asm);

    Asm->commandBuffer = (int*)calloc(((Asm->cmds).numberOfStrings)*2 + 4, sizeof(int));
    Asm->commandCounter = 3;

    int commandNumber = 0;

    Asm->commandBuffer[0] = signature;
    Asm->commandBuffer[1] = signature;
    Asm->commandBuffer[2] = version;

    for (Asm->numOfLine = 0; Asm->numOfLine < (Asm->cmds).numberOfStrings; Asm->numOfLine++) {
        char commandString[10] = {};
        int offset = 0;

        sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString, "%s%n", commandString, &offset);

        commandNumber = commandComparator(commandString);

        if (commandNumber == emptyString)
            continue;

        if (commandNumber == labelString) {
            int numOfLabel = 0;
            sscanf(commandString, ": %d", &numOfLabel);
            (Asm->labels)[numOfLabel] = Asm->commandCounter - 3;
            continue;
        }

        (Asm->commandBuffer)[Asm->commandCounter++] = commandNumber;

        if (commandNumber == ERROR_COMMANDcmd) {
            printf("ERROR! UNKNOWN COMMAND: \"%s\" ENTER ONLY AVAILABLE COMANDS! %s:%d\n", commandString, Asm->nameOfInputFile, (Asm->numOfLine+1));
            return 1;
        }

        if (commandNumber == PUSHcmd) {
            if  (getCmdPushValue (Asm, offset, commandString))
                return 1;
            continue;
        }

        if ((commandNumber == POPREGcmd) || (commandNumber == PUSHREGcmd) ||
            (commandNumber == POPMcmd) || (commandNumber == PUSHMcmd)) {
            if  (getCmdRegValue (Asm, offset, commandString))
                return 1;
            continue;
        }

        if ((commandNumber == JMPcmd) || (commandNumber == JBcmd) ||
            (commandNumber == JBEcmd) || (commandNumber == JAcmd) ||
            (commandNumber == JAEcmd) || (commandNumber == JEcmd) ||
            (commandNumber == JNEcmd) || (commandNumber == CALLcmd)) {

            if  (getCmdJumpValue (Asm, offset, commandString))
                return 1;
            continue;
        }

    }

    (Asm->commandBuffer)[Asm->commandCounter] = END_OF_COMMANDS;
    return 0;
}


int getCmdRegValue (struct assembler* Asm, int offset, char* commandString) {
    assert(Asm);
    assert(commandString);

    char regNameString[5] = {};

    if(sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString + offset, "%s", regNameString) != 1) {
        printf("ERROR %s COMMAND! BAD OR NO %s VALUE! %s:%d\n",commandString, commandString, Asm->nameOfInputFile, (Asm->numOfLine+1));
        return 1;
    }

    int commandNumber = getNumberOfReg(regNameString);
    (Asm->commandBuffer)[Asm->commandCounter++] = commandNumber;

    return 0;
}

int getCmdJumpValue (struct assembler* Asm, int offset, char* commandString) {
    assert(Asm);
    assert(commandString);

    int commandNumber = 0;

    if((sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString + offset, "%d", &commandNumber) != 1) || (commandNumber < 0)) {
        if (sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString + offset, " :%d", &commandNumber) == 1) {
            (Asm->commandBuffer)[Asm->commandCounter++] = (Asm->labels)[commandNumber];
            return 0;
        }
        else {
            printf("ERROR %s COMMAND! BAD OR NO %s VALUE! %s:%d\n", commandString, commandString, Asm->nameOfInputFile, (Asm->numOfLine+1));
            return 1;
        }
    }

    (Asm->commandBuffer)[Asm->commandCounter++] = commandNumber;
    return 0;
}

int getCmdPushValue (struct assembler* Asm, int offset, char* commandString) {
    assert(Asm);
    assert(commandString);

    int commandNumber = 0;

    if(sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString + offset, "%d", &commandNumber) != 1) {
        printf("ERROR PUSH COMMAND! BAD OR NO PUSH VALUE! %s:%d\n", Asm->nameOfInputFile, (Asm->numOfLine+1));
        return 1;
    }

    (Asm->commandBuffer)[Asm->commandCounter++] = commandNumber;
    return 0;
}
