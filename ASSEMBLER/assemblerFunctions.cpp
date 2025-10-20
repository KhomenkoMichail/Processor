#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "structAssembler.h"
#include "textStructs.h"
#include "assemblerFunctions.h"
#include "../COMMON/commandsNames.h"
#include "../COMMON/commonFunctions.h"
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
    Asm->listingFile = fopen(listingFile, "w");
    if(Asm->listingFile == NULL) {
        fprintf(stderr, "Error of opening file \"%s\"", listingFile);
        perror("");
    }

    getStructComands (&(Asm->cmds), inputFile);
    Asm->numOfLine = 0;

    for (size_t line = 0; line < (Asm->cmds).numberOfStrings; line++)
        commentsCleaner(((Asm->cmds).arrOfStringStructs[line]).ptrToString);

    Asm->commandCounter = 0;
    for (size_t numOfLabel = 0; numOfLabel < NUM_OF_LABELS; numOfLabel++)
        (Asm->labels)[numOfLabel] = -1;

    Asm->stringLabelCounter = 0;
    for (size_t numOfLabel = 0; numOfLabel < NUM_OF_STRING_LABELS; numOfLabel++)
        ((Asm->stringLabels)[numOfLabel]).labelAddress = -1;
}

void assemblerDtor (struct assembler* Asm) {
    assert(Asm);

    free(Asm->commandBuffer);
    free((Asm->cmds).text);
    free((Asm->cmds).arrOfStringStructs);

    if(fclose(Asm->listingFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", Asm->nameOfOutputTextFile);
        perror("");
    }
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

int getCmdRegArg (struct assembler* Asm, int offset, char* commandString) {
    assert(Asm);
    assert(commandString);

    char regNameString[5] = {};

    if(sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString + offset, "%s", regNameString) != 1) {
        printf("ERROR %s COMMAND! BAD OR NO %s VALUE! %s:%d\n",commandString, commandString, Asm->nameOfInputFile, (Asm->numOfLine+1));
        return 1;
    }

    int commandNumber = getNumberOfReg(regNameString);
    fprintf(Asm->listingFile, "%7d  | ", commandNumber);
    (Asm->commandBuffer)[Asm->commandCounter++] = commandNumber;

    return 0;
}

int getCmdLabelArg (struct assembler* Asm, int offset, char* commandString) {
    assert(Asm);
    assert(commandString);

    int commandNumber = 0;
    char labelName[15] = {};

    if(sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString + offset, "%d", &commandNumber) != 1) {

        if (sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString + offset, " :%d", &commandNumber) == 1) {
            (Asm->commandBuffer)[Asm->commandCounter++] = (Asm->labels)[commandNumber];
            fprintf(Asm->listingFile, "%7d  | ", commandNumber);
            return 0;
        }

        else if (sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString + offset, " :%s", labelName) == 1) {
            size_t numOfLabel = 0;

            for(; numOfLabel < Asm->stringLabelCounter; numOfLabel++)
                if (strcmp(labelName, ((Asm->stringLabels)[numOfLabel]).labelName) == 0)
                    break;

            if (numOfLabel == NUM_OF_STRING_LABELS)
                (Asm->commandBuffer)[Asm->commandCounter++] = -1;
            else
                (Asm->commandBuffer)[Asm->commandCounter++] = ((Asm->stringLabels)[numOfLabel]).labelAddress;

            fprintf(Asm->listingFile, "%7d  | ", ((Asm->stringLabels)[numOfLabel]).labelAddress);
            return 0;
        }

        else {
            printf("ERROR %s COMMAND! BAD OR NO %s VALUE! %s:%d\n", commandString, commandString, Asm->nameOfInputFile, (Asm->numOfLine+1));
            return 1;
        }
    }

    fprintf(Asm->listingFile, "%7d  | ", commandNumber);
    (Asm->commandBuffer)[Asm->commandCounter++] = commandNumber;
    return 0;
}

int getCmdNumArg (struct assembler* Asm, int offset, char* commandString) {
    assert(Asm);
    assert(commandString);

    int commandNumber = 0;

    if(sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString + offset, "%d", &commandNumber) != 1) {
        printf("ERROR PUSH COMMAND! BAD OR NO PUSH VALUE! %s:%d\n", Asm->nameOfInputFile, (Asm->numOfLine+1));
        return 1;
    }

    fprintf(Asm->listingFile, "%7d  | ", commandNumber);
    (Asm->commandBuffer)[Asm->commandCounter++] = commandNumber;
    return 0;
}

int getCmdArg (arguments argType, struct assembler* Asm, int offset, char* commandString) {
    assert(Asm);
    assert(commandString);

    if (argType == noArg) {
        fprintf(Asm->listingFile, "         | ");
        return 0;
    }

    if (argType == numArg)
        if  (getCmdNumArg (Asm, offset, commandString))
            return 1;

    if (argType == regArg)
        if  (getCmdRegArg (Asm, offset, commandString))
            return 1;

    if (argType == labelArg)
        if  (getCmdLabelArg (Asm, offset, commandString))
            return 1;

    return 0;
}

int getLabel (const char* commandString, struct assembler* Asm) {
    assert(commandString);
    assert(Asm);

    if (commandString[0] == ':') {
        size_t numOfLabel = 0;
        char labelName[15] = {};

        if(sscanf(commandString, ":%d", &numOfLabel) == 1) {
            (Asm->labels)[numOfLabel] = Asm->commandCounter - 3;
            return 1;
        }

        if(sscanf(commandString, ":%s", labelName) == 1) {

            if(Asm->stringLabelCounter >= NUM_OF_STRING_LABELS) {
                printf("ERROR! Too many string labels. ONLY %d string labels are supported!", NUM_OF_STRING_LABELS);
                return 0;
            }

            for (numOfLabel = 0; numOfLabel < Asm->stringLabelCounter; numOfLabel++)
                if(((Asm->stringLabels)[numOfLabel]).labelAddress == (int)(Asm->commandCounter - 3))
                    return 1;

            strcpy(((Asm->stringLabels)[Asm->stringLabelCounter]).labelName, labelName);
            ((Asm->stringLabels)[Asm->stringLabelCounter++]).labelAddress = Asm->commandCounter - 3;
            return 1;
        }
    }

    return 0;
}

int structCommandComparator(const void* firstStruct, const void* secondStruct) {
    assert(firstStruct);
    assert(secondStruct);

    const struct command* firstCommand = (const struct command*)firstStruct;
    const struct command* secondCommand = (const struct command*)secondStruct;

    if (firstCommand->commandHash < secondCommand->commandHash)
        return -1;

    if (firstCommand->commandHash == secondCommand->commandHash)
        return 0;

    return 1;
}

int bsearchComparator(const void* firstParam, const void* secondParam) {
    assert(firstParam);
    assert(secondParam);

    const unsigned long long* commandHash = (const unsigned long long*)firstParam;
    const struct command* command = (const struct command*)secondParam;

    if (*commandHash < command->commandHash)
        return -1;

    if (*commandHash == command->commandHash)
        return 0;

    return 1;
}

int compileCommands(struct assembler* Asm) {
    assert(Asm);

    Asm->commandBuffer = (int*)calloc(((Asm->cmds).numberOfStrings)*2 + 4, sizeof(int));
    Asm->commandCounter = 3;

    Asm->commandBuffer[0] = signature;
    Asm->commandBuffer[1] = signature;
    Asm->commandBuffer[2] = version;

    #include"../COMMON/commandsArray.h"
    qsort(commandsArray, NUMBER_OF_COMMANDS, sizeof(struct command), structCommandComparator);

    for (Asm->numOfLine = 0; Asm->numOfLine < (Asm->cmds).numberOfStrings; Asm->numOfLine++) {

        char commandString[10] = {};
        int offset = 0;

        sscanf((((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString, "%s%n", commandString, &offset);

        if (getLabel (commandString, Asm))
            continue;

        unsigned long long commandStringHash = getCommandHash(commandString);
        struct command* searchedCommand = (struct command*)bsearch(&commandStringHash, commandsArray, NUMBER_OF_COMMANDS, sizeof(struct command), bsearchComparator);

        if ((searchedCommand == NULL) || (searchedCommand->commandCode == ERROR_COMMANDcmd)) {
            printf("ERROR! UNKNOWN COMMAND: \"%s\" ENTER ONLY AVAILABLE COMANDS! %s:%d\n", commandString, Asm->nameOfInputFile, (Asm->numOfLine+1));
            return 1;
        }

        if (searchedCommand->commandCode == emptyString)
            continue;

        fprintf(Asm->listingFile, "[%4d]  | %4d ", Asm->commandCounter, searchedCommand->commandCode);

        (Asm->commandBuffer)[Asm->commandCounter++] = searchedCommand->commandCode;

        if (getCmdArg (searchedCommand->argType, Asm, offset, commandString))
            return 1;

        fprintf(Asm->listingFile, "%s\n", (((Asm->cmds).arrOfStringStructs)[Asm->numOfLine]).ptrToString);
    }

    return 0;
}
