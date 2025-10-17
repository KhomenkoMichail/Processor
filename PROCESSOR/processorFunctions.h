#ifndef PROCESSOR_FUNCTIONS
#define PROCESSOR_FUNCTIONS

#include "structsAndEnums.h"

#define PROCESSOR_CTOR(spuName, nameOfByteCodeFile)\
    spuName.spuInfo.nameOfFile = __FILE__;\
    spuName.spuInfo.nameOfFunct = __func__;\
    spuName.spuInfo.numOfLine = __LINE__;\
    processorCtor(&spuName, #spuName, nameOfByteCodeFile);\

#define PROCESSOR_ERRORS_CHECK(processorAddress, filePtr, dumpInfoAddress) \
    assert(filePtr);\
    assert(processorAddress);\
    dumpInfoAddress->nameOfFunct = __func__; \
    if(processorVerifier(processorAddress)) { \
        processorDump(processorAddress, filePtr, *dumpInfoAddress);\
        processorDump(processorAddress, stdout, *dumpInfoAddress);\
        return 1;\
    };\

void processorCtor (struct spu* processor, const char* processorName, const char* nameOfByteCodeFile);

int executeBufferCommands (struct spu* processor, FILE* dumpFile, struct info* dumpInfo, const char* nameOfBinCodeFile);

int processorVerifier (struct spu* processor);

void processorDump (struct spu* processor, FILE* file, struct info dumpInfo);

void fprintfErrorForProcessorDump (struct spu* processor, FILE* file);

void fprintfCommandCode(struct spu* processor, FILE* file);

void fprintfProcessorRegs (struct spu* processor, FILE* file);

void processorDtor(struct spu* processor);

void drawRam (int ram[30000]);

int executeBufferCommands2 (struct spu* processor, FILE* dumpFile, struct info* dumpInfo, const char* nameOfBinCodeFile);

#endif


