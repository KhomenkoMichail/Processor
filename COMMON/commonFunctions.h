#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

const size_t MAX_BUFFER_SIZE = 2048;

int* makeCommandBuffer (const char* nameOfBinCodeFile);

unsigned long long getStringHash(const char* string);

#endif
