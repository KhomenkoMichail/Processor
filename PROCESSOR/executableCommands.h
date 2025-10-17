#ifndef EXECUTABLE_COMMANDS_H
#define EXECUTABLE_COMMANDS_H

#include "../COMMON/commandsNames.h"
#include "structsAndEnums.h"

int stackAdd (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackSub (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackMul (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackMod (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackDiv (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackPow (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackSqrt (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackOut (stack_t* stack, FILE* file, struct info* dumpInfo);

int jumpCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int jumpB (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int jumpBE (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int jumpA (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int jumpAE (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int jumpE (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int jumpNE (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int pushCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int pushRegCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int popRegCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int inCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int callCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int retCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int pushMCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int popMCmd (struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int pushFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int popFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int calcFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int sqrtFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int outFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int inFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int jumpAndCallFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int comparingJumpFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int retFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int hltFunc(comandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

#endif
