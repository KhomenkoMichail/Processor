#ifndef PROCESSOR_FUNCTIONS
#define PROCESSOR_FUNCTIONS

#include "structsAndEnums.h"

void processorCtor (struct spu* processor, const char* nameOfByteCodeFile);

void executeBufferCommands (struct spu* processor, FILE* dumpFile, struct info* dumpInfo, const char* nameOfBinCodeFile);

#endif
