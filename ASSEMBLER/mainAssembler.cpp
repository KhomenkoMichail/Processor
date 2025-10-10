#include <stdio.h>
#include <assert.h>

#include "textStructs.h"
#include "assemblerFunctions.h"
#include "textTools.h"


int main (void) {

    const char* inputFile = "source3.asm";
    const char* outputTextFile = "textByteCode.txt";
    const char* outputBinFile = "binByteCode.bin";

    struct comands Cmd = {};
    size_t numOfBufferElements = 0;
    int labels[10] = {};

    getStructComands (&Cmd, inputFile);

    for (size_t line = 0; line < Cmd.numberOfStrings; line++)
        commentsCleaner((Cmd.arrOfStringStructs[line]).ptrToString);

    int* commandBuffer = commandRewriter (&Cmd, labels, &numOfBufferElements, inputFile);
    commandBuffer = commandRewriter (&Cmd, labels, &numOfBufferElements, inputFile);

    writeTextByteCode(commandBuffer, outputTextFile);
    writeBinByteCode(commandBuffer, numOfBufferElements, outputBinFile);

    return 0;
}
