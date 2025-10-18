#include <stdio.h>
#include <stdlib.h>
#include <TXLib.h>

#include "processorFunctions.h"
#include "stackFunctions.h"
#include "structsAndEnums.h"
#include "executableCommands.h"
#include "../COMMON/commandsNames.h"
#include "../COMMON/commonFunctions.h"


void processorCtor (struct spu* processor, const char* processorName, const char* nameOfByteCodeFile) {
    assert(processor);
    assert(nameOfByteCodeFile);

    processor->nameOfSpu = processorName;

    struct info stackInfo = {};
    STACK_CTOR(processor->stk, stackInfo, 10);

    STACK_CTOR(processor->regAddr, stackInfo, 10);

    processor->commandCode = makeCommandBuffer(nameOfByteCodeFile);

    processor->ram = (int*)calloc(3*100*100, sizeof(int));
    processor->drawON = 0;
}

int processorVerifier (struct spu* processor) {
    assert(processor);

    if (((processor->commandCode)[0] != signature) || ((processor->commandCode)[1] != signature))
        processor->spuErrorCode |= badCodeSignature;

    if ((processor->commandCode)[2] != version)
        processor->spuErrorCode |= badVersion;

    if (processor->pc >= MAX_BUFFER_SIZE)
        processor->spuErrorCode |= badPc;

    if (processor->commandCode == NULL)
        processor->spuErrorCode |= badCommandCodePtr;

    if (processor->regs == NULL)
        processor->spuErrorCode |= badRegsPtr;

    if ((processor->commandCode)[MAX_BUFFER_SIZE - 1] != 0)
        processor->spuErrorCode |= codeBufferOverflow;

    int stackErrorCode = stackVerifier (&(processor->stk));
    processor->spuErrorCode |= stackErrorCode;

    return processor->spuErrorCode;
}

void processorDump (struct spu* processor, FILE* file, struct info dumpInfo) {
    assert(processor);
    assert(file);

    fprintf(file, "----------------------------------------------------------------------------------------------\n");
    fprintf(file, "----------------------------------------------------------------------------------------------\n");

    fprintf(file, "processorDump() from %s at %s:%d\n", dumpInfo.nameOfFunct, dumpInfo.nameOfFile, dumpInfo.numOfLine);
    fprintf(file, "processor [%p] from %s at %s:%d\n\n", processor, processor->spuInfo.nameOfFunct, processor->spuInfo.nameOfFile, processor->spuInfo.numOfLine);

    fprintfErrorForProcessorDump(processor, file);
    fprintf(file, "program counter == %d\n\n", processor->pc);
    fprintfCommandCode(processor, file);
    fprintfProcessorRegs (processor, file);

    fprintf(file, "----------------------------------------------------------------------------------------------\n");
    stackDump (&(processor->stk), file, dumpInfo);
    fprintf(file, "----------------------------------------------------------------------------------------------\n");
    fprintf(file, "----------------------------------------------------------------------------------------------\n");
}

void fprintfErrorForProcessorDump (struct spu* processor, FILE* file) {
    assert(processor);
    assert(file);

    if (processor->spuErrorCode & badCodeSignature)
        fprintf(file, "_______________________ERROR! WRONG CODE SIGNATURE(%d)\n", badCodeSignature);

    if (processor->spuErrorCode & badVersion)
        fprintf(file, "_______________________ERROR! WRONG PROCESSOR VERSION(%d)\n", badVersion);

    if (processor->spuErrorCode & unknownCommand)
        fprintf(file, "_______________________ERROR! UNKNOWN COMMAND(%d)\n", unknownCommand);

    if (processor->spuErrorCode & badPc)
        fprintf(file, "_______________________ERROR! BAD PROGRAM COUNTER(%d)\n", badPc);

    if (processor->spuErrorCode & codeBufferOverflow)
        fprintf(file, "_______________________ERROR! CODE BUFFER IS OVERFLOW(%d)\n", codeBufferOverflow);

    if (processor->spuErrorCode & badRegsPtr)
        fprintf(file, "_______________________ERROR! REGS POINTER IS NULL(%d)\n", badRegsPtr);

    if (processor->spuErrorCode & badCommandCodePtr)
        fprintf(file, "_______________________ERROR! COMMAND CODE POINTER IS NULL(%d)\n", badCommandCodePtr);
}

void fprintfCommandCode(struct spu* processor, FILE* file) {
    assert(processor);
    assert(file);

    fprintf(file, "signature == %X %X\n", (processor->commandCode)[0], (processor->commandCode)[1]);
    if (processor->spuErrorCode & badCodeSignature)
        fprintf(file, "(BAD CODE SIGNATURE!)\n");

    fprintf(file, "version == %d\n", (processor->commandCode)[2]);
    if (processor->spuErrorCode & badVersion)
        fprintf(file, "(BAD PROCESSOR VERSION!)\n");

    fprintf (file, "Command code:\n");
    for(size_t numOfElement = 3; (processor->commandCode)[numOfElement] != HLTcmd; numOfElement++)
        fprintf(file, "%05d ", (processor->commandCode)[numOfElement]);

    fprintf (file, "\n");

    for (size_t numOfElement = 0; numOfElement < processor->pc; numOfElement++)
        fprintf(file, "      ");
    fprintf(file, "^^^^^\n");
}

void fprintfProcessorRegs (struct spu* processor, FILE* file) {
    assert(processor);
    assert(file);

    fprintf(file, "REGS [%p] :\n", processor->regs);

    for(size_t numOfReg = 0; numOfReg < NUM_OF_REGS; numOfReg++)
        fprintf(file, "%cx : %05d\n", 'A' + numOfReg, (processor->regs)[numOfReg]);
}

void processorDtor(struct spu* processor) {
    assert(processor);

    free(processor->commandCode);
    free(processor->stk.data);
}

void drawRam (int* ram) {
    assert(ram);
    txSetColor (TX_DARKGRAY);

    int x = 0;
    int y = 0;

    for (int i = 0; i < 30000; i += 3) {

        x = i/3 % 100;
        y = i/3 / 100;

        txSetFillColor (RGB (ram[i], ram[i+1], ram[i+2]));
        txRectangle (x*10, y*10, x*10+15, y*10+15);
    }
}

int executeCommands (struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(dumpFile);
    assert(dumpInfo);

    int stopExecution = 0;
    int txWindowCreated = 0;
    int numOfCmd = 0;

    #include "../COMMON/commandsArray.h"

    for (processor->pc = 0; ; ) {
        PROCESSOR_ERRORS_CHECK(processor, dumpFile, dumpInfo);

        for (numOfCmd = 0; numOfCmd < NUMBER_OF_COMMANDS - 1; numOfCmd++) {
            if ((processor->commandCode)[processor->pc + 3] == (comandsArray[numOfCmd]).commandCode)
                break;
        }

        stopExecution = (*((comandsArray[numOfCmd]).commandFunc))((comandsArray[numOfCmd]).commandCode, processor, dumpFile, dumpInfo);

        PROCESSOR_ERRORS_CHECK(processor, dumpFile, dumpInfo);

        if ((processor->drawON) && (!txWindowCreated)) {
            txCreateWindow (900, 900);
            txWindowCreated = 1;
        }

        if (stopExecution)
            break;
    }

    return 0;
}
