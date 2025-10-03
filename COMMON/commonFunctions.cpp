#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "commonFunctions.h"
#include "commandsNames.h"

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
