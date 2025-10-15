#include <stdio.h>
#include <assert.h>

#include "structAssembler.h"
#include "textStructs.h"
#include "assemblerFunctions.h"
#include "textTools.h"


int main (void) {

    const char* inputFile = "circle.asm";
    const char* outputTextFile = "textByteCode.txt";
    const char* outputBinFile = "binByteCode.bin";

    struct assembler Asm = {};

    assemblerCtor(&Asm, inputFile, outputTextFile, outputBinFile);

    if (commandRewriter(&Asm)) {
        assemblerDtor(&Asm);
        return 1;
    }


    if (commandRewriter(&Asm)) {
        assemblerDtor(&Asm);
        return 1;
    }

    writeTextByteCode(&Asm);
    writeBinByteCode(&Asm);

    assemblerDtor(&Asm);
    return 0;
}
