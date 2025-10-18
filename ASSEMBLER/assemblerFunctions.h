#ifndef ASSEMBLER_FUNCTIONS_H
#define ASSEMBLER_FUNCTIONS_H

void assemblerCtor (struct assembler* Asm, const char* inputFile, const char* outputTextFile, const char* outputBinFile, const char* listingFile);

void assemblerDtor (struct assembler* Asm);

void writeTextByteCode (struct assembler* Asm);

void writeBinByteCode (struct assembler* Asm);

int getNumberOfReg(const char* nameOfReg);

void commentsCleaner(char* str);

int getCmdRegArg (struct assembler* Asm, int offset, char* commandString, FILE* listingFile);

int getCmdLabelArg (struct assembler* Asm, int offset, char* commandString, FILE* listingFile);

int getCmdNumArg (struct assembler* Asm, int offset, char* commandString, FILE* listingFile);

int compileCommands(struct assembler* Asm);

int getCmdArg (arguments argType, struct assembler* Asm, int offset, char* commandString, FILE* listingFile);

int getLabel (const char* commandString, struct assembler* Asm);

#endif
