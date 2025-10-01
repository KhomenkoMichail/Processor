#ifndef CALC_FUNCTIONS_H
#define CALC_FUNCTIONS_H


int stackAdd (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackSub (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackMul (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackDiv (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackPow (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackSqrt (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackOut (stack_t* stack, FILE* file, struct info* dumpInfo);

void readCommands (stack_t* stack, FILE* file, struct info* dumpInfo);

int executeCommand (const char* command, stack_t* stack, FILE* file, struct info* dumpInfo);

#endif
