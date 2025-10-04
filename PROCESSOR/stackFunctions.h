#ifndef STACK_FUNCTIONS_H
#define STACK_FUNCTIONS_H

#include "structsAndEnums.h"
#include <assert.h>

#define STACK_CTOR(stackName, stackInfoName, capacity)\
    stackInfoName.nameOfFile = __FILE__;\
    stackInfoName.nameOfFunct = __func__;\
    stackInfoName.numOfLine = __LINE__;\
    stackCtor(&stackName, capacity, #stackName, stackInfoName);\

#define STACK_ERRORS_CHECK(stackAddress, filePtr, dumpInfoAddress) \
    assert(filePtr);\
    assert(dumpInfoAddress);\
    dumpInfoAddress->nameOfFunct = __func__; \
    if(stackVerifier(stackAddress)) { \
        stackDump(stackAddress, filePtr, *dumpInfoAddress);\
        stackDump(stackAddress, stdout, *dumpInfoAddress);\
        return stackAddress->errorCode;\
    }\

#define STACK_PUSH(stackAddress, value, filePtr, dumpInfoAddress)\
    assert(filePtr);\
    assert(dumpInfoAddress);\
    (dumpInfoAddress)->nameOfFile = __FILE__; \
    (dumpInfoAddress)->numOfLine = __LINE__;\
    stackPush(stackAddress, value, filePtr, dumpInfoAddress);\

//   if (stackPush(stackAddress, value, filePtr, dumpInfoAddress))
//     return (stackAddress)->errorCode;

#define STACK_POP(stackAddress, ptrToVariable, filePtr, dumpInfoAddress)\
    assert(filePtr);\
    assert(dumpInfoAddress);\
    (dumpInfoAddress)->nameOfFile = __FILE__; \
    (dumpInfoAddress)->numOfLine = __LINE__;\
    stackPop(stackAddress, ptrToVariable, filePtr, dumpInfoAddress);\

//    if (stackPop(stackAddress, ptrToVariable, filePtr, dumpInfoAddress))
//        return (stackAddress)->errorCode;

int stackCtor (stack_t* stack, ssize_t capacity, const char* nameOfStack, struct info stackCreationInfo);

int stackPush (stack_t* stack, stackElement_t value, FILE* file, struct info* dumpInfo);

int fprintfElement (FILE* file, stackElement_t element);

int stackPop (stack_t* stack, stackElement_t* ptrToVariable, FILE* file, struct info* dumpInfo);

int stackVerifier (stack_t* stack);

void stackDump (stack_t* stack, FILE* file, struct info dumpInfo);

void fprintfErrorForDump (stack_t* stack, FILE* file);

void fprintfStackData (FILE* file, stack_t stack);

int stackDtor (stack_t* stack, FILE* file, struct info* dumpInfo);

int stackPtrIsNull (stack_t* stack, FILE* file);

unsigned long long djb2Hash (stack_t* stack);

void pouringPoison (stack_t* stack, ssize_t numOfStartElement);

#endif
