#ifndef STRUCT_ASSEMBLER_H
#define STRUCT_ASSEMBLER_H

#include "textStructs.h"
#include "../COMMON/commandsNames.h"

const int NUM_OF_LABELS = 10;

struct assembler {
    const char* nameOfInputFile;
    const char* nameOfOutputTextFile;
    const char* nameOfOutputBinFile;

    struct comands cmds;
    size_t numOfLine;

    int* commandBuffer;
    size_t commandCounter;

    int labels[NUM_OF_LABELS];

};

struct comand {
    const char* name;
    comandsNames commandCode;
    arguments argType;
};

#endif
