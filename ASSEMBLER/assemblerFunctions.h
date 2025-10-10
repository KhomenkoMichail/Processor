#ifndef ASSEMBLER_FUNCTIONS_H
#define ASSEMBLER_FUNCTIONS_H

int* commandRewriter (struct comands* structAddress, int* labels, size_t* numOfBufferElements, const char* nameOfFile);

int commandComparator (char* command);

void writeTextByteCode (int* codeBuffer, const char* nameOfFile);

void writeBinByteCode (int* codeBuffer, size_t numOfBufferElements, const char* nameOfFile);

int getNumberOfReg(const char* nameOfReg);

void commentsCleaner(char* str);

#endif
