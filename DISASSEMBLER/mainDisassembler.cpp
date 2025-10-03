#include <stdio.h>
#include <stdlib.h>

#include "../COMMON/commonFunctions.h"
#include "disassemblerFunctions.h"

int main(void) {
    const char* nameOfByteCodeFile = "binByteCode.bin";
    const char* nameOfOutputFile = "dis.asm";
    int* commandBuffer = makeCommandBuffer(nameOfByteCodeFile);

    writeCommands (commandBuffer, nameOfOutputFile);

    free(commandBuffer);
}
