#include <stdio.h>
#include <typeinfo>
#include <stdlib.h>

#include "structsAndEnums.h"
#include "stackFunctions.h"
#include "calcFunctions.h"
#include "processorFunctions.h"

int main(void) {
    const char* nameOfDumpFile = "writeDump.log";
    const char* nameOfByteCodeFile = "binByteCode.bin";

    struct stack stk = {};
    struct info stackInfo = {};
    struct info dumpInfo = {};

    FILE* logFile = fopen(nameOfDumpFile, "w");

    STACK_CTOR(stk, stackInfo, 1);

    int* commandBuffer = makeCommandBuffer(nameOfByteCodeFile);
    executeBufferCommands (&stk, logFile, &dumpInfo, commandBuffer, nameOfByteCodeFile);

    fclose(logFile);
    free(commandBuffer);

    return 0;
}
