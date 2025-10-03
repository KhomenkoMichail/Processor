#ifndef PROCESSOR_FUNCTIONS
#define PROCESSOR_FUNCTIONS

#include "structsAndEnums.h"

//int* makeCommandBuffer (const char* nameOfBinCodeFile);

void executeBufferCommands (stack_t* stack, FILE* dumpFile, struct info* dumpInfo, int* commandBuffer, const char* nameOfBinCodeFile);


#endif
