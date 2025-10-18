#ifndef STRUCTS_AND_ENUMS_H
#define STRUCTS_AND_ENUMS_H

typedef int stackElement_t;

#define CANARY_PROTECTION
#define HASH_PROTECTION

const int NUM_OF_REGS = 8;

enum stackErr_t {
    noErrors = 0b0,
    badStackPtr= 0b1,
    badDataPtr = 0b10,
    badSize = 0b100,
    badCapacity = 0b1000,
    badElementType = 0b10000,
    badRealloc = 0b100000,
    noElementsForPop = 0b1000000,
    deadFirstCanary = 0b10000000,
    deadSecondCanary = 0b100000000,
    divisionByZero = 0b1000000000,
    badSqrt = 0b10000000000,
    incorrectHash = 0b100000000000,
};

enum processorErr_t {
    badCodeSignature = 0b1000000000000,
    badVersion = 0b10000000000000,
    unknownCommand = 0b100000000000000,
    badPc = 0b1000000000000000,
    codeBufferOverflow = 0b10000000000000000,
    badRegsPtr = 0b100000000000000000,
    badCommandCodePtr = 0b1000000000000000000,
};


struct info {
    const char* nameOfFile;
    const char* nameOfFunct;
    unsigned int numOfLine;
};

struct stack {
    stackElement_t* data;
    ssize_t size;
    ssize_t capacity;

    const char* nameOfType;
    const char* nameOfStack;
    int errorCode;

    #ifdef HASH_PROTECTION
        unsigned long long hash;
    #endif

    struct info stackInfo;
};


typedef struct stack stack_t;

struct spu {
    int* commandCode;
    size_t pc;
    stack_t stk;
    int regs[NUM_OF_REGS];

    const char* nameOfSpu;
    struct info spuInfo;
    int spuErrorCode;

    stack_t regAddr;

    int* ram;
    int drawON;
};

#endif

