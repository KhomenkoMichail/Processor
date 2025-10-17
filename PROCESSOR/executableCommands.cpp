#include <stdio.h>
#include <math.h>
#include <string.h>

#include "stackFunctions.h"
#include "structsAndEnums.h"
#include "executableCommands.h"
#include "../COMMON/commandsNames.h"
#include "../COMMON/commonFunctions.h"
#include "processorFunctions.h"

int stackAdd (stack_t* stack, FILE* file, struct info* dumpInfo) {
    assert(file);
    assert(dumpInfo);

    stackElement_t firstOperand = 0;

    stackElement_t secondOperand = 0;

    STACK_POP(stack, &firstOperand, file, dumpInfo);
    STACK_POP(stack, &secondOperand, file, dumpInfo);

    stackElement_t sum = secondOperand + firstOperand;

    STACK_PUSH(stack, sum, file, dumpInfo);

    return noErrors;
}

int stackSub (stack_t* stack, FILE* file, struct info* dumpInfo) {
    assert(file);
    assert(dumpInfo);

    stackElement_t firstOperand = 0;
    stackElement_t secondOperand = 0;

    STACK_POP(stack, &firstOperand, file, dumpInfo);
    STACK_POP(stack, &secondOperand, file, dumpInfo);

    stackElement_t diff = secondOperand - firstOperand;

    STACK_PUSH(stack, diff, file, dumpInfo);

    return noErrors;
}

int stackMul (stack_t* stack, FILE* file, struct info* dumpInfo) {
    assert(file);
    assert(dumpInfo);

    stackElement_t firstOperand = 0;
    stackElement_t secondOperand = 0;

    STACK_POP(stack, &firstOperand, file, dumpInfo);
    STACK_POP(stack, &secondOperand, file, dumpInfo);

    stackElement_t product = secondOperand * firstOperand;

    STACK_PUSH(stack, product, file, dumpInfo);

    return noErrors;
}

int stackMod (stack_t* stack, FILE* file, struct info* dumpInfo) {
    assert(file);
    assert(dumpInfo);

    stackElement_t firstOperand = 0;
    stackElement_t secondOperand = 0;

    STACK_POP(stack, &firstOperand, file, dumpInfo);
    STACK_POP(stack, &secondOperand, file, dumpInfo);

    stackElement_t product = secondOperand % firstOperand;

    STACK_PUSH(stack, product, file, dumpInfo);

    return noErrors;
}


int stackDiv (stack_t* stack, FILE* file, struct info* dumpInfo) {
    assert(file);
    assert(dumpInfo);

    stackElement_t firstOperand = 0;
    stackElement_t secondOperand = 0;

    STACK_POP(stack, &firstOperand, file, dumpInfo);
    STACK_POP(stack, &secondOperand, file, dumpInfo);

    if (firstOperand == 0) {
        STACK_PUSH(stack, secondOperand, file, dumpInfo);
        STACK_PUSH(stack, firstOperand, file, dumpInfo);

        fprintf(file, "ERROR! DIVISION BY ZERO IS IMPOSSIBLE!\n");
        printf("ERROR! DIVISION BY ZERO IS IMPOSSIBLE!\n");

        dumpInfo->nameOfFunct = __func__;
        dumpInfo->nameOfFile = __FILE__;
        dumpInfo->numOfLine = __LINE__;
        stackDump(stack, file, *dumpInfo);
        stackDump(stack, stdout, *dumpInfo);

        return divisionByZero;
    }


    stackElement_t quotient  = secondOperand / firstOperand;

    STACK_PUSH(stack, quotient, file, dumpInfo);

    return noErrors;
}

int stackPow (stack_t* stack, FILE* file, struct info* dumpInfo) {
    assert(file);
    assert(dumpInfo);

    stackElement_t firstOperand = 0;
    stackElement_t secondOperand = 0;

    STACK_POP(stack, &firstOperand, file, dumpInfo);
    STACK_POP(stack, &secondOperand, file, dumpInfo);

    stackElement_t result = 1;
    for (int i = 0; i < firstOperand; i++)
        result *= secondOperand;

    STACK_PUSH(stack, result, file, dumpInfo);

    return noErrors;
}

int stackSqrt (stack_t* stack, FILE* file, struct info* dumpInfo) {
    assert(file);
    assert(dumpInfo);

    stackElement_t operand = 0;

    STACK_POP(stack, &operand, file, dumpInfo);

    if (operand < 0) {
        STACK_PUSH(stack, operand, file, dumpInfo);

        fprintf(file, "ERROR! THE RADICAL EXPRESSION IS LESS THAN ZERO!\n");
        printf("ERROR! THE RADICAL EXPRESSION IS LESS THAN ZERO!\n");

        dumpInfo->nameOfFunct = __func__;
        dumpInfo->nameOfFile = __FILE__;
        dumpInfo->numOfLine = __LINE__;
        stackDump(stack, file, *dumpInfo);
        stackDump(stack, stdout, *dumpInfo);

        return badSqrt;
    }

    stackElement_t result = (stackElement_t)sqrt(operand);
    STACK_PUSH(stack, result, file, dumpInfo);

    return noErrors;
}

int stackOut (stack_t* stack, FILE* file, struct info* dumpInfo) {
    assert(file);
    assert(dumpInfo);

    stackElement_t element = 0;
    STACK_POP(stack, &element, file, dumpInfo);
    printf("%d\n", element);

    return noErrors;
}


int jumpB (struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;
    if (((processor->commandCode)[processor->pc + 3] == END_OF_COMMANDS) || ((processor->commandCode)[processor->pc + 3] >= (int)MAX_BUFFER_SIZE) || ((processor->commandCode)[processor->pc + 3] < 0)){
        printf("ERROR JB COMMAND! BAD OR NO JB VALUE!\n");
        return 1;
    }
    int a = 0;
    int b = 0;

    STACK_POP(&(processor->stk), &a, dumpFile, dumpInfo);
    STACK_POP(&(processor->stk), &b, dumpFile, dumpInfo);

    if (a < b)
        processor->pc = (processor->commandCode)[processor->pc + 3];
    else processor->pc++;

    return 0;
}

int jumpCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;

    if (((processor->commandCode)[processor->pc + 3] == END_OF_COMMANDS) || ((processor->commandCode)[processor->pc + 3] >= (int)MAX_BUFFER_SIZE) || ((processor->commandCode)[processor->pc + 3] < 0)){
        fprintf(dumpFile, "ERROR JMP COMMAND! BAD NO JMP VALUE!\n");
        printf("ERROR JMP COMMAND! BAD OR NO JMP VALUE!\n");
        return 1;
    }

    processor->pc = (processor->commandCode)[processor->pc + 3];

    return 0;
}

int jumpBE (struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;
    if (((processor->commandCode)[processor->pc + 3] == END_OF_COMMANDS) || ((processor->commandCode)[processor->pc + 3] >= (int)MAX_BUFFER_SIZE) || ((processor->commandCode)[processor->pc + 3] < 0)){
        printf("ERROR JBE COMMAND! BAD OR NO JBE VALUE!\n");
        return 1;
    }
    int a = 0;
    int b = 0;

    STACK_POP(&(processor->stk), &a, dumpFile, dumpInfo);
    STACK_POP(&(processor->stk), &b, dumpFile, dumpInfo);

    if (a <= b)
        processor->pc = (processor->commandCode)[processor->pc + 3];
    else processor->pc++;

    return 0;
}

int jumpA (struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;
    if (((processor->commandCode)[processor->pc + 3] == END_OF_COMMANDS) || ((processor->commandCode)[processor->pc + 3] >= (int)MAX_BUFFER_SIZE) || ((processor->commandCode)[processor->pc + 3] < 0)){
        printf("ERROR JA COMMAND! BAD OR NO JA VALUE!\n");
        return 1;
    }
    int a = 0;
    int b = 0;

    STACK_POP(&(processor->stk), &a, dumpFile, dumpInfo);
    STACK_POP(&(processor->stk), &b, dumpFile, dumpInfo);

    if (a > b)
        processor->pc = (processor->commandCode)[processor->pc + 3];
    else processor->pc++;

    return 0;
}

int jumpAE (struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;
    if (((processor->commandCode)[processor->pc + 3] == END_OF_COMMANDS) || ((processor->commandCode)[processor->pc + 3] >= (int)MAX_BUFFER_SIZE) || ((processor->commandCode)[processor->pc + 3] < 0)){
        printf("ERROR JAE COMMAND! BAD OR NO JAE VALUE!\n");
        return 1;
    }
    int a = 0;
    int b = 0;

    STACK_POP(&(processor->stk), &a, dumpFile, dumpInfo);
    STACK_POP(&(processor->stk), &b, dumpFile, dumpInfo);

    if (a >= b)
        processor->pc = (processor->commandCode)[processor->pc + 3];
    else processor->pc++;

    return 0;
}

int jumpE (struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;
    if (((processor->commandCode)[processor->pc + 3] == END_OF_COMMANDS) || ((processor->commandCode)[processor->pc + 3] >= (int)MAX_BUFFER_SIZE) || ((processor->commandCode)[processor->pc + 3] < 0)){
        printf("ERROR JE COMMAND! BAD OR NO JE VALUE!\n");
        return 1;
    }
    int a = 0;
    int b = 0;

    STACK_POP(&(processor->stk), &a, dumpFile, dumpInfo);
    STACK_POP(&(processor->stk), &b, dumpFile, dumpInfo);

    if (a == b)
        processor->pc = (processor->commandCode)[processor->pc + 3];
    else processor->pc++;

    return 0;
}

int jumpNE (struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;
    if (((processor->commandCode)[processor->pc + 3] == END_OF_COMMANDS) || ((processor->commandCode)[processor->pc + 3] >= (int)MAX_BUFFER_SIZE) || ((processor->commandCode)[processor->pc + 3] < 0)){
        printf("ERROR JNE COMMAND! BAD OR NO JNE VALUE!\n");
        return 1;
    }
    int a = 0;
    int b = 0;

    STACK_POP(&(processor->stk), &a, dumpFile, dumpInfo);
    STACK_POP(&(processor->stk), &b, dumpFile, dumpInfo);

    if (a != b)
        processor->pc = (processor->commandCode)[processor->pc + 3];
    else processor->pc++;

    return 0;
}

int pushCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;
    if ((processor->commandCode)[processor->pc + 3] == END_OF_COMMANDS) {
        fprintf(dumpFile, "ERROR PUSH COMMAND! BAD OR NO PUSH VALUE!\n");
        printf("ERROR PUSH COMMAND! BAD OR NO PUSH VALUE!\n");
        return 1;
    }

    STACK_PUSH(&(processor->stk), (processor->commandCode)[processor->pc + 3], dumpFile, dumpInfo);
    processor->pc++;

    return 0;
}

int pushRegCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;

    if (((processor->commandCode)[processor->pc + 3] == END_OF_COMMANDS) || ((processor->commandCode)[processor->pc + 3] >= (int)MAX_BUFFER_SIZE) || ((processor->commandCode)[processor->pc + 3] < 0)){
        fprintf(dumpFile, "ERROR PUSHREG COMMAND! BAD OR NO PUSHREG VALUE!\n");
        printf("ERROR PUSHREG COMMAND! BAD OR NO PUSHREG VALUE!\n");
        return 1;
    }

    STACK_PUSH(&(processor->stk), (processor->regs)[((processor->commandCode)[processor->pc + 3])], dumpFile, dumpInfo);
    processor->pc++;

    return 0;
}

int popRegCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;

    if (((processor->commandCode)[processor->pc + 3] == END_OF_COMMANDS) || ((processor->commandCode)[processor->pc + 3] >= (int)MAX_BUFFER_SIZE) || ((processor->commandCode)[processor->pc + 3] < 0)){
        fprintf(dumpFile, "ERROR POPREG COMMAND! BAD OR NO POPREG VALUE!\n");
        printf("ERROR POPREG COMMAND! BAD OR NO POPREG VALUE!\n");
        return 1;
    }
    STACK_POP(&(processor->stk), (processor->regs) + (processor->commandCode)[processor->pc + 3], dumpFile, dumpInfo);

    processor->pc++;

    return 0;
}

int inCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

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

int callCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;
    STACK_PUSH(&(processor->regAddr), (processor->pc + 1), dumpFile, dumpInfo);

    if (((processor->commandCode)[processor->pc + 3] == END_OF_COMMANDS) || ((processor->commandCode)[processor->pc + 3] >= (int)MAX_BUFFER_SIZE) || ((processor->commandCode)[processor->pc + 3] < 0)){
        fprintf(dumpFile, "ERROR CALL COMMAND! BAD NO CALL VALUE!\n");
        printf("ERROR CALL COMMAND! BAD OR NO CALL VALUE!\n");
        return 1;
    }

    processor->pc = (processor->commandCode)[processor->pc + 3];

    return 0;
}

int retCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    int retAddress = 0;
    STACK_POP(&(processor->regAddr), &retAddress, dumpFile, dumpInfo);

    processor->pc = retAddress;

    return 0;
}

int pushMCmd(struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;

    if (((processor->commandCode)[processor->pc + 3] == END_OF_COMMANDS) || ((processor->commandCode)[processor->pc + 3] < 0)){
        fprintf(dumpFile, "ERROR PUSHM COMMAND! BAD OR NO PUSHM VALUE!\n");
        printf("ERROR PUSHM COMMAND! BAD OR NO PUSHM VALUE!\n");
        return 1;
    }

    int numOfReg = (processor->commandCode)[processor->pc + 3];
    int numOfRamElem = (processor->regs)[numOfReg];

    STACK_PUSH(&(processor->stk), (processor->ram)[numOfRamElem], dumpFile, dumpInfo);

    drawRam(processor->ram);

    processor->pc++;

    return 0;
}

int popMCmd(struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;

    if (((processor->commandCode)[processor->pc + 3] == END_OF_COMMANDS) || ((processor->commandCode)[processor->pc + 3] < 0)){
        fprintf(dumpFile, "ERROR POPM COMMAND! BAD OR NO POPM VALUE!\n");
        printf("ERROR POPM COMMAND! BAD OR NO POPM VALUE!\n");
        return 1;
    }

    int numOfReg = (processor->commandCode)[processor->pc + 3];
    int numOfRamElem = (processor->regs)[numOfReg];

    STACK_POP(&(processor->stk), (processor->ram) + numOfRamElem, dumpFile, dumpInfo);

    drawRam(processor->ram);

    processor->pc++;

    return 0;
}



////////////////////

int pushFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
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
        drawRam(processor->ram);
    }

    processor->pc++;

    return 0;
}

int popFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
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
        drawRam(processor->ram);
    }

    processor->pc++;
    return 0;
}

int calcFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    int firstOperand = 0;
    int secondOperand = 0;
    int result = 0;

    STACK_POP(&(processor->stk), &firstOperand, dumpFile, dumpInfo);
    STACK_POP(&(processor->stk), &secondOperand, dumpFile, dumpInfo);

    switch (executableCommand) {
        case ADDcmd:
            result = secondOperand + firstOperand;
            break;
        case SUBcmd:
            result = secondOperand - firstOperand;
            break;
        case MULcmd:
            result = secondOperand * firstOperand;
            break;
        case MODcmd:
            result = secondOperand % firstOperand;
            break;
        case DIVcmd:
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
            break;
        case POWcmd:
            result = 1;
            for (int i = 0; i < firstOperand; i++)
                result *= secondOperand;
            break;
        default:
            return 1;
            break;
    }

    STACK_PUSH(&(processor->stk), result, dumpFile, dumpInfo);
    processor->pc++;

    return 0;
}

int sqrtFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
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

int outFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
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

int inFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
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

int jumpAndCallFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
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

int comparingJumpFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);

    processor->pc++;
    if (((processor->commandCode)[processor->pc + 3] >= (int)MAX_BUFFER_SIZE) || ((processor->commandCode)[processor->pc + 3] < 0)){
        printf("ERROR JB COMMAND! BAD OR NO JB VALUE!\n");
        return 1;
    }

    int a = 0;
    int b = 0;

    STACK_POP(&(processor->stk), &a, dumpFile, dumpInfo);
    STACK_POP(&(processor->stk), &b, dumpFile, dumpInfo);

    switch (executableCommand) {
        case JBcmd:
            if (a < b)
                processor->pc = (processor->commandCode)[processor->pc + 3];
            else processor->pc++;
            break;
        case JBEcmd:
            if (a <= b)
                processor->pc = (processor->commandCode)[processor->pc + 3];
            else processor->pc++;
            break;
        case JAcmd:
            if (a > b)
                processor->pc = (processor->commandCode)[processor->pc + 3];
            else processor->pc++;
            break;
        case JAEcmd:
            if (a >= b)
                processor->pc = (processor->commandCode)[processor->pc + 3];
            else processor->pc++;
            break;
        case JEcmd:
            if (a == b)
                processor->pc = (processor->commandCode)[processor->pc + 3];
            else processor->pc++;
            break;
        case JNEcmd:
            if (a != b)
                processor->pc = (processor->commandCode)[processor->pc + 3];
            else processor->pc++;
            break;
        default:
            return 1;
            break;
    }

    return 0;
}

int retFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    assert(processor);
    assert(dumpFile);
    assert(dumpInfo);
    (void)executableCommand;

    int retAddress = 0;
    STACK_POP(&(processor->regAddr), &retAddress, dumpFile, dumpInfo);

    processor->pc = retAddress;

    return 0;
}

int hltFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo) {
    (void)executableCommand;
    (void)processor;
    (void)dumpFile;
    (void)dumpInfo;

    return 1;
}




