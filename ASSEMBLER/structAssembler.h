#ifndef STRUCT_ASSEMBLER_H
#define STRUCT_ASSEMBLER_H

#include "textStructs.h"
#include "../COMMON/commandsNames.h"

const int NUM_OF_LABELS = 10;
const int NUM_OF_STRING_LABELS = 100;

struct stringLabel {
    char labelName[30];
    int labelAddress;
    unsigned long long stringLabelHash;
};

struct assembler {
    const char* nameOfInputFile;
    const char* nameOfOutputTextFile;
    const char* nameOfOutputBinFile;

    FILE* listingFile;

    struct comands cmds;
    size_t numOfLine;

    int* commandBuffer;
    size_t commandCounter;

    int labels[NUM_OF_LABELS];

    struct stringLabel stringLabels[NUM_OF_STRING_LABELS];
    size_t stringLabelCounter;
};

#endif
