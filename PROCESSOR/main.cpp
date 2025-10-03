#include <stdio.h>
#include <typeinfo>
#include <stdlib.h>

#include "structsAndEnums.h"
#include "stackFunctions.h"
#include "calcFunctions.h"
#include "processorFunctions.h"
#include "../COMMON/commonFunctions.h"
#include "../COMMON/commandsNames.h"


int main(void) {
    const char* nameOfDumpFile = "writeDump.log";
    const char* nameOfByteCodeFile = "binByteCode.bin";

    struct spu processor = {};
    processorCtor (&processor, nameOfByteCodeFile);

    struct info dumpInfo = {};
    FILE* logFile = fopen(nameOfDumpFile, "w");

    executeBufferCommands (&processor, logFile, &dumpInfo, nameOfByteCodeFile);

    free(processor.stk.data);
    free(processor.commandCode);
    fclose(logFile);

    return 0;
}
