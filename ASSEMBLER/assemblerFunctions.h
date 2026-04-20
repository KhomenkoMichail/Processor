#ifndef ASSEMBLER_FUNCTIONS_H
#define ASSEMBLER_FUNCTIONS_H

void assemblerCtor (struct assembler* Asm, const char* inputFile, const char* outputTextFile, const char* outputBinFile, const char* listingFile);

void assemblerDtor (struct assembler* Asm);

void writeTextByteCode (struct assembler* Asm);

void writeBinByteCode (struct assembler* Asm);

int getNumberOfReg(const char* nameOfReg);

void commentsCleaner(char* str);

int getCmdRegArg (struct assembler* Asm, int offset, char* commandString);

int getCmdLabelArg (struct assembler* Asm, int offset, char* commandString);

int getCmdNumArg (struct assembler* Asm, int offset, char* commandString);

int getCmdArg (arguments argType, struct assembler* Asm, int offset, char* commandString);

int getLabel (const char* commandString, struct assembler* Asm);

int structCommandComparator(const void* firstStruct, const void* secondStruct);

int bsearchCmdComparator(const void* commandHash, const void* structCommand);

int compileCommands(struct assembler* Asm);

void writeSignature (struct assembler* Asm);

int getCmdRamArg (struct assembler* Asm, int offset, char* commandString);

//int bsearchLabelComparator(const void* firstParam, const void* secondParam);
int bsearchLabelComparator(const void* key, const void* element);

int structLabelComparator(const void* firstStruct, const void* secondStruct);

int badCommand (struct command* searchedCommand, const char* commandString, struct assembler* Asm);

#endif
