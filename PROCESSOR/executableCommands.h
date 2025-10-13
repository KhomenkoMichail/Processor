#ifndef EXECUTABLE_COMMANDS_H
#define EXECUTABLE_COMMANDS_H

int stackAdd (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackSub (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackMul (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackDiv (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackPow (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackSqrt (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackOut (stack_t* stack, FILE* file, struct info* dumpInfo);

void readCommands (stack_t* stack, FILE* file, struct info* dumpInfo);

int executeCommand (const char* command, stack_t* stack, FILE* file, struct info* dumpInfo);

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

#endif
