#include <stdio.h>
#include <assert.h>

#include "textStructs.h"
#include "assemblerFunctions.h"


int main (void) {

    const char* inputFile = "source.asm";
    const char* outputTextFile = "textByteCode.txt";
    const char* outputBinFile = "binByteCode.bin";

    struct comands Cmd = {};
    size_t numOfBufferElements = 0;

    getStructComands (&Cmd, inputFile);

    int* commandBuffer = commandRewriter (&Cmd, &numOfBufferElements, inputFile);

    writeTextByteCode(commandBuffer, outputTextFile);

    writeBinByteCode(commandBuffer, numOfBufferElements, outputBinFile);

    return 0;
}
