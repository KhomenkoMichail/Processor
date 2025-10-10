#include <stdio.h>
#include <typeinfo>
#include <stdlib.h>

#include "structsAndEnums.h"
#include "stackFunctions.h"
#include "executableCommands.h"
#include "processorFunctions.h"
#include "../COMMON/commonFunctions.h"
#include "../COMMON/commandsNames.h"
#include <string.h>


int main(void) {
    const char* nameOfDumpFile = "writeDump.log";
    const char* nameOfByteCodeFile = "binByteCode.bin";

    struct spu processor = {};
    PROCESSOR_CTOR(processor, nameOfByteCodeFile);

    struct info dumpInfo = {};
    FILE* logFile = fopen(nameOfDumpFile, "w");

    executeBufferCommands (&processor, logFile, &dumpInfo, nameOfByteCodeFile);

    processorDtor(&processor);
    fclose(logFile);

    return 0;
}
