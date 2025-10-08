#include <stdio.h>
#include <assert.h>
#include <TXLib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "textStructs.h"
#include "assemblerFunctions.h"
#include "../COMMON/commandsNames.h"

char* copyFileContent (struct comands* structAddress, const char* fileName) {
    assert(fileName);

    int fileDescriptor = open(fileName, O_RDONLY, 0);
    if (fileDescriptor == -1) {
        fprintf(stderr, "Error of opening file \"%s\"", fileName);
        perror("");
        return NULL;
    }

    ssize_t sizeOfFile = getSizeOfFile(fileDescriptor);
    if (sizeOfFile < 1) {
        close(fileDescriptor);
        return NULL;
    }

    char* fileCopyBuffer = (char*)calloc(sizeOfFile + 1, sizeof(char));

    size_t numOfReadSymbols = read(fileDescriptor, fileCopyBuffer, sizeOfFile);
    fileCopyBuffer[numOfReadSymbols] = '\0';
    //commentsCleaner(fileCopyBuffer);

    if(close(fileDescriptor) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", fileName);
        perror("");
        return NULL;
    }

    structAddress->sizeOfText = numOfReadSymbols;

    return fileCopyBuffer;
}

void getStructComands (struct comands* structAddress, const char* fileName) {
    assert(structAddress);
    assert(fileName);

    char* buffer = copyFileContent(structAddress, fileName);
    assert(buffer);

    size_t numberOfStrings    = getNumberOfSymbols(buffer, '\n');
    char** arrOfPtrsToStrings = (char**)calloc(numberOfStrings, sizeof(*arrOfPtrsToStrings));

    structAddress->text               = buffer;
    structAddress->numberOfStrings    = numberOfStrings;

    getArrOfStringStructs(structAddress);

}

void getArrOfStringStructs (struct comands* structAddress) {
    assert(structAddress);

    structAddress->arrOfStringStructs = (struct line*)calloc(structAddress->numberOfStrings, sizeof(struct line));
    size_t line = 0;
    (structAddress->arrOfStringStructs[line]).ptrToString = structAddress->text;
    //(structAddress->arrOfStringStructs[line]).lengthOfString = myStrlen((structAddress->arrOfStringStructs[line]).ptrToString) + 1;
    line++;

    size_t numOfCharInText = 0;
    for( ; (structAddress->text[numOfCharInText] != '\0') && (line < structAddress->numberOfStrings) ; numOfCharInText++) {

        if (structAddress->text[numOfCharInText] == '\n') {
            (structAddress->arrOfStringStructs[line]).ptrToString = structAddress->text + numOfCharInText + 1;
            line++;
        }
    }

    getLengthOfStrings(structAddress);
}

void getLengthOfStrings (struct comands* structAddress) {
    assert(structAddress);

    for(size_t line = 0; line < structAddress->numberOfStrings - 1; line++)
        (structAddress->arrOfStringStructs[line]).lengthOfString = (size_t)((structAddress->arrOfStringStructs[line+1]).ptrToString - (structAddress->arrOfStringStructs[line]).ptrToString);

    //(structAddress->arrOfStringStructs[structAddress->numberOfStrings - 1]).lengthOfString = myStrlen((structAddress->arrOfStringStructs[structAddress->numberOfStrings - 1]).ptrToString) + 1;
}

void freeStruct (struct comands* structAddress) {
    assert(structAddress);

    free(structAddress->text);
    free(structAddress->arrOfStringStructs);
}

ssize_t getSizeOfFile (int fileDescriptor) {
    struct stat fileInfo = {};

    if (fstat(fileDescriptor, &fileInfo) == 0)
        return fileInfo.st_size;

    perror("Error of getting the size of the file");
    return -1;
}

size_t getNumberOfSymbols (char* text, char searchedSymbol) {
    assert(text);

    size_t numOfSymbolsFound = 0;
    for(size_t numOfChar = 0; text[numOfChar] != '\0'; numOfChar++)
        if (text[numOfChar] == searchedSymbol)
            numOfSymbolsFound++;

    return numOfSymbolsFound;
}

int* commandRewriter (struct comands* structAddress, size_t* numOfBufferElements, const char* nameOfFile) {
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
        sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString, "%s", commandString);

        commandNumber = commandComparator(commandString);

        if (commandNumber == emptyString)
            continue;

        commandBuffer[numberOfSymbols++] = commandNumber;

        if (commandNumber == ERROR_COMMANDcmd) {
            printf("ERROR! UNKNOWN COMMAND: \"%s\" ENTER ONLY AVAILABLE COMANDS! %s:%d\n", commandString, nameOfFile, (line+1));
            return NULL;
        }

        if (commandNumber == PUSHcmd) {
            if(sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + 5, "%d", &commandNumber) != 1) {
                printf("ERROR PUSH COMMAND! BAD OR NO PUSH VALUE! %s:%d\n", nameOfFile, (line+1));
                return NULL;
            }
        commandBuffer[numberOfSymbols++] = commandNumber;
        continue;
        }

        if (commandNumber == PUSHREGcmd) {
            if(sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + 7, "%s", regNameString) != 1) {
                printf("ERROR PUSHREG COMMAND! BAD OR NO PUSHREG VALUE! %s:%d\n", nameOfFile, (line+1));
                return NULL;
            }
        commandNumber = getNumberOfReg(regNameString);
        commandBuffer[numberOfSymbols++] = commandNumber;
        continue;
        }

        if (commandNumber == POPREGcmd) {
            if(sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + 6, "%s", regNameString) != 1) {
                printf("ERROR POPREG COMMAND! BAD OR NO POPREG VALUE! %s:%d\n", nameOfFile, (line+1));
                return NULL;
            }
        commandNumber = getNumberOfReg(regNameString);
        commandBuffer[numberOfSymbols++] = commandNumber;
        continue;
        }

        if (commandNumber == JMPcmd) {
                if((sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + 4, "%d", &commandNumber) != 1) || (commandNumber < 0)){
                    printf("ERROR JMP COMMAND! BAD OR NO JMP VALUE! %s:%d\n", nameOfFile, (line+1));
                    return NULL;
                }
            commandBuffer[numberOfSymbols++] = commandNumber;
            continue;
        }

        if (commandNumber == JBcmd) {
                if((sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + 3, "%d", &commandNumber) != 1) || (commandNumber < 0)){
                    printf("ERROR JB COMMAND! BAD OR NO JB VALUE! %s:%d\n", nameOfFile, (line+1));
                    return NULL;
                }
            commandBuffer[numberOfSymbols++] = commandNumber;
            continue;
        }

        if (commandNumber == JBEcmd) {
                if((sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + 4, "%d", &commandNumber) != 1) || (commandNumber < 0)){
                    printf("ERROR JBE COMMAND! BAD OR NO JBE VALUE! %s:%d\n", nameOfFile, (line+1));
                    return NULL;
                }
            commandBuffer[numberOfSymbols++] = commandNumber;
            continue;
        }

        if (commandNumber == JAcmd) {
                if((sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + 3, "%d", &commandNumber) != 1) || (commandNumber < 0)){
                    printf("ERROR JA COMMAND! BAD OR NO JA VALUE! %s:%d\n", nameOfFile, (line+1));
                    return NULL;
                }
            commandBuffer[numberOfSymbols++] = commandNumber;
            continue;
        }

        if (commandNumber == JAEcmd) {
                if((sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + 4, "%d", &commandNumber) != 1) || (commandNumber < 0)){
                    printf("ERROR JAE COMMAND! BAD OR NO JAE VALUE! %s:%d\n", nameOfFile, (line+1));
                    return NULL;
                }
            commandBuffer[numberOfSymbols++] = commandNumber;
            continue;
        }

        if (commandNumber == JEcmd) {
                if((sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + 3, "%d", &commandNumber) != 1) || (commandNumber < 0)){
                    printf("ERROR JE COMMAND! BAD OR NO JE VALUE! %s:%d\n", nameOfFile, (line+1));
                    return NULL;
                }
            commandBuffer[numberOfSymbols++] = commandNumber;
            continue;
        }

        if (commandNumber == JNEcmd) {
                if((sscanf(((structAddress->arrOfStringStructs)[line]).ptrToString + 4, "%d", &commandNumber) != 1) || (commandNumber < 0)){
                    printf("ERROR JNE COMMAND! BAD OR NO JNE VALUE! %s:%d\n", nameOfFile, (line+1));
                    return NULL;
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
