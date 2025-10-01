#include <stdio.h>
#include <typeinfo>

#include "structsAndEnums.h"
#include "stackFunctions.h"
#include "calcFunctions.h"

int main(void) {
    struct stack stk = {};
    struct info stackInfo = {};
    struct info dumpInfo = {};

    FILE* logFile = fopen("writeDump.log", "w");

    STACK_CTOR(stk, stackInfo, 1);

    readCommands(&stk, logFile, &dumpInfo);

    return 0;
}
