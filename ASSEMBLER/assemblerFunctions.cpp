#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "structAssembler.h"
#include "textStructs.h"
#include "assemblerFunctions.h"
#include "../COMMON/commandsNames.h"
#include "textTools.h"
#include "../PROCESSOR/executableCommands.h"

void assemblerCtor (struct assembler* Asm, const char* inputFile, const char* outputTextFile, const char* outputBinFile, const char* listingFile) {
    assert(Asm);
    assert(inputFile);
    assert(outputTextFile);
    assert(outputBinFile);
    assert(listingFile);

    Asm->nameOfInputFile = inputFile;
    Asm->nameOfOutputTextFile = outputTextFile;
    Asm->nameOfOutputBinFile = outputBinFile;
    Asm->nameOfListingFile = listingFile;

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

void writeTextByteCode (struct assembler* Asm) {
    assert(Asm);

    FILE* byteCodeFile = fopen(Asm->nameOfOutputTextFile, "w");
    if(byteCodeFile == NULL) {
        fprintf(stderr, "Error of opening file \"%s\"", Asm->nameOfOutputTextFile);
        perror("");
        return;
    }

    for (size_t numOfElement = 0; numOfElement < Asm->commandCounter; numOfElement++)
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

int getCmdRegArg (struct assembler* Asm, int offset, char* commandString,  FILE* listingFile) {
    assert(Asm);
    assert(commandString);

    char regNameString[5] = {};

    if(sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString + offset, "%s", regNameString) != 1) {
        printf("ERROR %s COMMAND! BAD OR NO %s VALUE! %s:%d\n",commandString, commandString, Asm->nameOfInputFile, (Asm->numOfLine+1));
        return 1;
    }

    int commandNumber = getNumberOfReg(regNameString);
    fprintf(listingFile, "%7d  | ", commandNumber);
    (Asm->commandBuffer)[Asm->commandCounter++] = commandNumber;

    return 0;
}

int getCmdLabelArg (struct assembler* Asm, int offset, char* commandString,  FILE* listingFile) {
    assert(Asm);
    assert(commandString);

    int commandNumber = 0;

    if((sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString + offset, "%d", &commandNumber) != 1) || (commandNumber < 0)) {
        if (sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString + offset, " :%d", &commandNumber) == 1) {
            (Asm->commandBuffer)[Asm->commandCounter++] = (Asm->labels)[commandNumber];
            fprintf(listingFile, "%7d  | ", commandNumber);
            return 0;
        }
        else {
            printf("ERROR %s COMMAND! BAD OR NO %s VALUE! %s:%d\n", commandString, commandString, Asm->nameOfInputFile, (Asm->numOfLine+1));
            return 1;
        }
    }

    fprintf(listingFile, "%7d  | ", commandNumber);
    (Asm->commandBuffer)[Asm->commandCounter++] = commandNumber;
    return 0;
}

int getCmdNumArg (struct assembler* Asm, int offset, char* commandString, FILE* listingFile) {
    assert(Asm);
    assert(commandString);

    int commandNumber = 0;

    if(sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString + offset, "%d", &commandNumber) != 1) {
        printf("ERROR PUSH COMMAND! BAD OR NO PUSH VALUE! %s:%d\n", Asm->nameOfInputFile, (Asm->numOfLine+1));
        return 1;
    }

    fprintf(listingFile, "%7d  | ", commandNumber);
    (Asm->commandBuffer)[Asm->commandCounter++] = commandNumber;
    return 0;
}

int compileCommands(struct assembler* Asm) {
    assert(Asm);

    FILE* listingFile = fopen(Asm->nameOfListingFile, "w");

    Asm->commandBuffer = (int*)calloc(((Asm->cmds).numberOfStrings)*2 + 4, sizeof(int));
    Asm->commandCounter = 3;

    Asm->commandBuffer[0] = signature;
    Asm->commandBuffer[1] = signature;
    Asm->commandBuffer[2] = version;

    #include "../COMMON/commandsArray.h"

    for (Asm->numOfLine = 0; Asm->numOfLine < (Asm->cmds).numberOfStrings; Asm->numOfLine++) {

        char commandString[10] = {};
        int offset = 0;
        size_t numOfCmd = 0;

        sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString, "%s%n", commandString, &offset);

        for (numOfCmd = 0; numOfCmd < NUMBER_OF_COMMANDS - 1; numOfCmd++) {
            if (strcmp(commandString, (comandsArray[numOfCmd]).name ) == 0)
                break;
        }

        if ((comandsArray[numOfCmd]).commandCode == emptyString)
            continue;

        if (getLabel (commandString, Asm))
            continue;

        if ((comandsArray[numOfCmd]).commandCode == ERROR_COMMANDcmd) {
            printf("ERROR! UNKNOWN COMMAND: \"%s\" ENTER ONLY AVAILABLE COMANDS! %s:%d\n", commandString, Asm->nameOfInputFile, (Asm->numOfLine+1));
            return 1;
        }

        fprintf(listingFile, "[%4d]  | %4d ", Asm->commandCounter, (comandsArray[numOfCmd]).commandCode);

        (Asm->commandBuffer)[Asm->commandCounter++] = (comandsArray[numOfCmd]).commandCode;

        if (getCmdArg ((comandsArray[numOfCmd]).argType, Asm, offset, commandString, listingFile))
            return 1;

        fprintf(listingFile, "%s\n", (((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString);
    }

    fclose(listingFile);
    return 0;
}

int getCmdArg (arguments argType, struct assembler* Asm, int offset, char* commandString, FILE* listingFile) {
    assert(Asm);
    assert(commandString);

    if (argType == noArg) {
        fprintf(listingFile, "         | ");
        return 0;
    }

    if (argType == numArg)
        if  (getCmdNumArg (Asm, offset, commandString, listingFile))
            return 1;

    if (argType == regArg)
        if  (getCmdRegArg (Asm, offset, commandString, listingFile))
            return 1;

    if (argType == labelArg)
        if  (getCmdLabelArg (Asm, offset, commandString, listingFile))
            return 1;

    return 0;
}

int getLabel (const char* commandString, struct assembler* Asm) {
    assert(commandString);
    assert(Asm);

    if (commandString[0] == ':') {
        int numOfLabel = 0;
        sscanf(commandString, ": %d", &numOfLabel);
        (Asm->labels)[numOfLabel] = Asm->commandCounter - 3;
        return 1;
    }

    return 0;
}

