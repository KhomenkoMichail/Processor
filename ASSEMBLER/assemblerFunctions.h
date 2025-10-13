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

int getCmdRegValue (struct assembler* Asm, int offset, char* commandString);

int getCmdJumpValue (struct assembler* Asm, int offset, char* commandString);

int getCmdPushValue (struct assembler* Asm, int offset, char* commandString);

#endif
