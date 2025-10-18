#ifndef EXECUTABLE_COMMANDS_H
#define EXECUTABLE_COMMANDS_H

#include "../COMMON/commandsNames.h"
#include "structsAndEnums.h"

int pushFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int popFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int calcFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int sqrtFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int outFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int inFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int jumpAndCallFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int comparingJumpFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int retFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int hltFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int errorMessageFunc(commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

int drawFunc (commandsNames executableCommand, struct spu* processor, FILE* dumpFile, struct info* dumpInfo);

#endif
