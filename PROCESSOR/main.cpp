#include <stdio.h>
#include <typeinfo>
#include <stdlib.h>

#include "structsAndEnums.h"
#include "stackFunctions.h"
#include "calcFunctions.h"
#include "processorFunctions.h"
#include "../COMMON/commonFunctions.h"

int main(int argc, char* argv[]) {
    const char* nameOfDumpFile = "writeDump.log";
    //const char* nameOfByteCodeFile = "binByteCode.bin";

    struct stack stk = {};
    struct info stackInfo = {};
    struct info dumpInfo = {};

    FILE* logFile = fopen(nameOfDumpFile, "w");

    STACK_CTOR(stk, stackInfo, 1);

    if(argc == 2) {
        int* commandBuffer = makeCommandBuffer(argv[1]);
        executeBufferCommands (&stk, logFile, &dumpInfo, commandBuffer, argv[1]);
        free(commandBuffer);
    }
    else
        readCommands(&stk, logFile, &dumpInfo);


    fclose(logFile);

    return 0;
}
