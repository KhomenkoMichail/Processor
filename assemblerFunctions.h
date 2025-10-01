#ifndef ASSEMBLER_FUNCTIONS_H
#define ASSEMBLER_FUNCTIONS_H

#include "textStructs.h"

char* copyFileContent (struct comands* structAddress, const char* fileName);

void getStructComands (struct comands* structAddress, const char* fileName);

void getArrOfStringStructs (struct comands* structAddress);

void getLengthOfStrings (struct comands* structAddress);

void freeStruct (struct comands* structAddress);

ssize_t getSizeOfFile (int fileDescriptor);

size_t getNumberOfSymbols (char* text, char searchedSymbol);

int* commandRewriter (struct comands* structAddress, const char* nameOfFile);

int commandComparator (char* command);

#endif
