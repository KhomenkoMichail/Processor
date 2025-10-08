#include <stdio.h>
#include <assert.h>

#include "textStructs.h"
#include "assemblerFunctions.h"


int main (void) {

    const char* inputFile = "source2.asm";
    const char* outputTextFile = "textByteCode.txt";
    const char* outputBinFile = "binByteCode.bin";

    struct comands Cmd = {};
    size_t numOfBufferElements = 0;

    getStructComands (&Cmd, inputFile);

    for (size_t line = 0; line < Cmd.numberOfStrings; line++)
        commentsCleaner((Cmd.arrOfStringStructs[line]).ptrToString);

    int* commandBuffer = commandRewriter (&Cmd, &numOfBufferElements, inputFile);

    writeTextByteCode(commandBuffer, outputTextFile);
    writeBinByteCode(commandBuffer, numOfBufferElements, outputBinFile);

    return 0;
}
