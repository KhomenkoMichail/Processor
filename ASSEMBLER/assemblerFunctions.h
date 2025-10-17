#ifndef ASSEMBLER_FUNCTIONS_H
#define ASSEMBLER_FUNCTIONS_H

void assemblerCtor (struct assembler* Asm, const char* inputFile, const char* outputTextFile, const char* outputBinFile);

void assemblerDtor (struct assembler* Asm);

int commandComparator (char* command);

void writeTextByteCode (struct assembler* Asm);

void writeBinByteCode (struct assembler* Asm);

int getNumberOfReg(const char* nameOfReg);

void commentsCleaner(char* str);

int commandRewriter(struct assembler* Asm);

int getCmdRegArg (struct assembler* Asm, int offset, char* commandString);

int getCmdLabelArg (struct assembler* Asm, int offset, char* commandString);

int getCmdNumArg (struct assembler* Asm, int offset, char* commandString);

int commandRewriter2(struct assembler* Asm);

int getCmdArg (arguments argType, struct assembler* Asm, int offset, char* commandString);

int getLabel (const char* commandString, struct assembler* Asm);

#endif
