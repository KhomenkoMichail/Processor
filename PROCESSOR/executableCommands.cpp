#include <stdio.h>
#include <math.h>
#include <string.h>
#include <TXLib.h>

#include "stackFunctions.h"
#include "structsAndEnums.h"
#include "executableCommands.h"
#include "../COMMON/commandsNames.h"
#include "../COMMON/commonFunctions.h"
#include "processorFunctions.h"

int pushFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;

    if (executableCommand == PUSHcmd) {
        STACK_PUSH(&(processor->stk), (processor->commandCode)[processor->pc + 3], dumpFile, dumpInfo);
    }

    if (executableCommand == PUSHREGcmd) {
        STACK_PUSH(&(processor->stk), (processor->regs)[((processor->commandCode)[processor->pc + 3])], dumpFile, dumpInfo);
    }

    if (executableCommand == PUSHMcmd) {
        int numOfReg = (processor->commandCode)[processor->pc + 3];
        int numOfRamElem = (processor->regs)[numOfReg];

        STACK_PUSH(&(processor->stk), (processor->ram)[numOfRamElem], dumpFile, dumpInfo);

        if (processor->drawON)
            drawRam(processor->ram);
    }

    processor->pc++;

    return 0;
}

int popFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;

    if (executableCommand == POPREGcmd) {
        STACK_POP(&(processor->stk), (processor->regs) + (processor->commandCode)[processor->pc + 3], dumpFile, dumpInfo);
    }

    if (executableCommand == POPMcmd) {
        int numOfReg = (processor->commandCode)[processor->pc + 3];
        int numOfRamElem = (processor->regs)[numOfReg];
        STACK_POP(&(processor->stk), (processor->ram) + numOfRamElem, dumpFile, dumpInfo);

        if (processor->drawON)
            drawRam(processor->ram);
    }

    processor->pc++;
    return 0;
}

int calcFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    int firstOperand = 0;
    int secondOperand = 0;
    int result = 0;

    STACK_POP(&(processor->stk), &firstOperand, dumpFile, dumpInfo);
    STACK_POP(&(processor->stk), &secondOperand, dumpFile, dumpInfo);

    if (executableCommand ==  ADDcmd)
        result = secondOperand + firstOperand;

    if (executableCommand == SUBcmd)
        result = secondOperand - firstOperand;

    if (executableCommand == MULcmd)
        result = secondOperand * firstOperand;

    if (executableCommand == MODcmd)
        result = secondOperand % firstOperand;

    if (executableCommand ==  DIVcmd) {
        if (firstOperand == 0) {
            STACK_PUSH(&(processor->stk), secondOperand, dumpFile, dumpInfo);
            STACK_PUSH(&(processor->stk), firstOperand, dumpFile, dumpInfo);

            fprintf(dumpFile, "ERROR! DIVISION BY ZERO IS IMPOSSIBLE!\n");
            printf("ERROR! DIVISION BY ZERO IS IMPOSSIBLE!\n");

            dumpInfo->nameOfFunct = __func__;
            dumpInfo->nameOfFile = __FILE__;
            dumpInfo->numOfLine = __LINE__;
            stackDump(&(processor->stk), dumpFile, *dumpInfo);
            stackDump(&(processor->stk), stdout, *dumpInfo);

            return divisionByZero;
        }

        result = secondOperand / firstOperand;
    }

    if (executableCommand ==  POWcmd) {
        result = 1;
        for (int i = 0; i < firstOperand; i++)
            result *= secondOperand;
    }

    STACK_PUSH(&(processor->stk), result, dumpFile, dumpInfo);
    processor->pc++;

    return 0;
}

int sqrtFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpInfo);
    assert(dumpFile);
    (void)executableCommand;


    stackElement_t operand = 0;

    STACK_POP(&(processor->stk), &operand, dumpFile, dumpInfo);

    if (operand < 0) {
        STACK_PUSH(&(processor->stk), operand, dumpFile, dumpInfo);

        fprintf(dumpFile, "ERROR! THE RADICAL EXPRESSION IS LESS THAN ZERO!\n");
        printf("ERROR! THE RADICAL EXPRESSION IS LESS THAN ZERO!\n");

        dumpInfo->nameOfFunct = __func__;
        dumpInfo->nameOfFile = __FILE__;
        dumpInfo->numOfLine = __LINE__;
        stackDump(&(processor->stk), dumpFile, *dumpInfo);
        stackDump(&(processor->stk), stdout, *dumpInfo);

        return badSqrt;
    }

    stackElement_t result = (stackElement_t)sqrt(operand);
    STACK_PUSH(&(processor->stk), result, dumpFile, dumpInfo);

    processor->pc++;
    return 0;
}

int outFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpInfo);
    assert(dumpFile);
    (void)executableCommand;

    stackElement_t element = 0;
    STACK_POP(&(processor->stk), &element, dumpFile, dumpInfo);
    printf("%d\n", element);

    processor->pc++;
    return 0;
}

int inFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);
    (void)executableCommand;

    int pushValue = 0;

    if (scanf("%d", &pushValue) != 1) {
        fprintf(dumpFile, "ERROR IN COMMAND! ENTER ONLY INTEGER VALUES!\n");
        printf("ERROR IN COMMAND! ENTER ONLY INTEGER VALUES!\n");
        return 1;
    }

    STACK_PUSH(&(processor->stk), pushValue, dumpFile, dumpInfo);
    processor->pc++;

    return 0;
}

int jumpAndCallFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;

    if (((processor->commandCode)[processor->pc + 3] >= (int)MAX_BUFFER_SIZE) || ((processor->commandCode)[processor->pc + 3] < 0)){
        fprintf(dumpFile, "ERROR JMP COMMAND! BAD NO JMP VALUE!\n");
        printf("ERROR JMP COMMAND! BAD OR NO JMP VALUE!\n");
        return 1;
    }

    if (executableCommand == CALLcmd) {
        STACK_PUSH(&(processor->regAddr), (processor->pc + 1), dumpFile, dumpInfo);
    }

    processor->pc = (processor->commandCode)[processor->pc + 3];
    return 0;
}

int comparingJumpFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;
    if (((processor->commandCode)[processor->pc + 3] >= (int)MAX_BUFFER_SIZE) || ((processor->commandCode)[processor->pc + 3] < 0)){
        printf("ERROR COMPARING JUMP COMMAND! BAD OR NO COMPARING JUMP VALUE!\n");
        return 1;
    }

    int a = 0;
    int b = 0;

    STACK_POP(&(processor->stk), &a, dumpFile, dumpInfo);
    STACK_POP(&(processor->stk), &b, dumpFile, dumpInfo);

    if (executableCommand == JBcmd) {
        if (a < b)
            processor->pc = (processor->commandCode)[processor->pc + 3];
        else processor->pc++;
    }

    if (executableCommand ==  JBEcmd) {
        if (a <= b)
            processor->pc = (processor->commandCode)[processor->pc + 3];
        else processor->pc++;
    }

    if (executableCommand == JAcmd) {
        if (a > b)
            processor->pc = (processor->commandCode)[processor->pc + 3];
        else processor->pc++;
    }

    if (executableCommand == JAEcmd) {
        if (a >= b)
            processor->pc = (processor->commandCode)[processor->pc + 3];
        else processor->pc++;
    }

    if (executableCommand == JEcmd) {
        if (a == b)
            processor->pc = (processor->commandCode)[processor->pc + 3];
        else processor->pc++;
    }

    if (executableCommand == JNEcmd) {
        if (a != b)
            processor->pc = (processor->commandCode)[processor->pc + 3];
        else processor->pc++;
    }

    return 0;
}

int retFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);
    (void)executableCommand;

    int retAddress = 0;
    STACK_POP(&(processor->regAddr), &retAddress, dumpFile, dumpInfo);

    processor->pc = retAddress;

    return 0;
}

int hltFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    (void)executableCommand;
    (void)processor;
    (void)dumpFile;
    (void)dumpInfo;

    return 1;
}

int errorMessageFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);
    (void)executableCommand;

    printf("ERROR!, UNKNOWN COMMAND!\n");
    fprintf(dumpFile, "ERROR!, UNKNOWN COMMAND!\n");

    processorDump(processor, dumpFile, *dumpInfo);
    processorDump(processor, stdout, *dumpInfo);

    return 1;
}

int drawFunc (commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    (void)executableCommand;
    (void)processor;
    (void)dumpFile;
    (void)dumpInfo;

    if (!(processor->drawON))
        processor->drawON = 1;

    processor->pc++;
    return 0;
}







