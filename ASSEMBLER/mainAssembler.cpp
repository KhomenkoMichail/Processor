#include <stdio.h>
#include <assert.h>

#include "structAssembler.h"
#include "textStructs.h"
#include "assemblerFunctions.h"
#include "textTools.h"


int main (int argc, const char* argv[]) {

    if (argc != 2) return printf("Usage: %s source.asm\n", argv[0]), 1;

    const char* inputFile = argv[1];
    const char* outputTextFile = "textByteCode.txt";
    const char* outputBinFile = "binByteCode.bin";
    const char* listingFile = "asmListing.lst";

    struct assembler Asm = {};

    assemblerCtor(&Asm, inputFile, outputTextFile, outputBinFile, listingFile);

    if (compileCommands(&Asm)) {
        assemblerDtor(&Asm);
        return 1;
    }


    if (compileCommands(&Asm)) {
        assemblerDtor(&Asm);
        return 1;
    }

    writeTextByteCode(&Asm);
    writeBinByteCode(&Asm);

    assemblerDtor(&Asm);
    return 0;
}
