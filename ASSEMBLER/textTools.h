#ifndef TEXT_TOOLS_H
#define TEXT_TOOLS_H

char* copyFileContent (struct comands* structAddress, const char* fileName);

void getStructComands (struct comands* structAddress, const char* fileName);

void getArrOfStringStructs (struct comands* structAddress);

void getLengthOfStrings (struct comands* structAddress);

void freeStruct (struct comands* structAddress);

ssize_t getSizeOfFile (int fileDescriptor);

size_t getNumberOfSymbols (char* text, char searchedSymbol);

#endif
