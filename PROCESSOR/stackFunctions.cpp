#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <typeinfo>
#include <errno.h>


#include "stackFunctions.h"
#include "structsAndEnums.h"

const stackElement_t POISON = 0xBADBABE;
const int MAX_CAPACITY = 100000000;

#ifdef CANARY_PROTECTION
    const int CANARY = 0xBEEFFACE;
    #define ON_CANARY(code)  code
#else
    #define ON_CANARY(code)
#endif

#ifdef HASH_PROTECTION
    #define ON_HASH_PROTECTION(code) code
#else
    #define ON_HASH_PROTECTION(code)
#endif

int stackCtor (stack_t* stack, ssize_t capacity, const char* nameOfStack, struct info creationInfo) {
    if (stackPtrIsNull (stack, stdout))
        return badStackPtr;

    stack->size = 0;
    stack->capacity = capacity;
    stack->nameOfStack = nameOfStack;
    stack->errorCode = noErrors;
    stack->nameOfType = txDemangle(typeid(stackElement_t).name()).c_str();

    stack->stackInfo = creationInfo;

    if ((capacity <= 0) || (capacity > MAX_CAPACITY)) {
        stack->errorCode |= badCapacity;
        return stack->errorCode;
    }


    stack->data = (stackElement_t*)calloc((capacity ON_CANARY (+2)), sizeof(stackElement_t));

    pouringPoison(stack, 0 ON_CANARY(+1));

    #ifdef CANARY_PROTECTION
        stack->data[0] = CANARY;
        stack->data[capacity + 1] = CANARY;
    #endif

    ON_HASH_PROTECTION (stack->hash = djb2Hash(stack));

    return stack->errorCode;
}

int stackPush (stack_t* stack, stackElement_t value, FILE* file, struct info* dumpInfo) {
    assert(file);
    assert(dumpInfo);

    if (stackPtrIsNull (stack, file))
        return badStackPtr;
    STACK_ERRORS_CHECK(stack, file, dumpInfo);

    stack->data[stack->size ON_CANARY (+1)] = value;
    stack->size++;

    if (stack->size == stack->capacity) {
        stack->capacity *= 2;
        realloc(stack->data, stack->capacity ON_CANARY (+2));

        if (stack->data == NULL)
            stack->errorCode = badRealloc;

        else {
            pouringPoison(stack, stack->size ON_CANARY (+1));
            ON_CANARY (stack->data[stack->capacity + 1] = CANARY);
        }
    }

    ON_HASH_PROTECTION (stack->hash = djb2Hash(stack));

    STACK_ERRORS_CHECK(stack, file, dumpInfo);

    return stack->errorCode;
}

int fprintfElement (FILE* file, stackElement_t element) {
    assert(file);

    #ifdef  CANARY_PROTECTION
        if (element == CANARY) {
            fprintf(file, "\"%X\" (CANARY)\n", (int)element);
            return 0;
        }
    #endif

    if (element == POISON) {
        fprintf(file, "\"%X\" (POISON)\n", (int)element);
        return 0;
    }
    if (typeid(stackElement_t) == typeid(int)) {
        fprintf(file, "\"%d\"\n", (int)element);
        return 0;
    }
    if (typeid(stackElement_t) == typeid(double)) {
        fprintf(file, "\"%lf\"\n", (double)element);
        return 0;
    }
    if (typeid(stackElement_t) == typeid(float)) {
        fprintf(file, "\"%f\"\n", (float)element);
        return 0;
    }
    if (typeid(stackElement_t) == typeid(char)) {
        fprintf(file, "\"%c\"\n", (char)element);
        return 0;
    }
    if (typeid(stackElement_t) == typeid(char*)) {
        fprintf(file, "\"%s\"\n", (char*)element);
        return 0;
    }

    return 1;
}

int stackPop (stack_t* stack, stackElement_t* ptrToVariable, FILE* file, struct info* dumpInfo) {
    assert(ptrToVariable);
    assert(file);
    assert(dumpInfo);

    if (stackPtrIsNull (stack, file))
        return badStackPtr;

    STACK_ERRORS_CHECK(stack, file, dumpInfo);

    if (stack->size == 0) {
        stack->errorCode |= noElementsForPop;
        stackDump(stack, file, *dumpInfo);
        stackDump(stack, stdout, *dumpInfo);
        return stack->errorCode;
    }

    stack->size--;

    *ptrToVariable = stack->data[stack->size ON_CANARY (+1)];
    stack->data[stack->size ON_CANARY (+1)] = POISON;

/*
    if ((stack->size == stack->capacity/4) && stack->capacity > 4) {
        stack->capacity /= 2;
        int lastValue = (stack->data)[stack->size - 1 ON_CANARY (+1)];
        realloc(stack->data, stack->capacity ON_CANARY (+2));
        (stack->data)[stack->size - 1 ON_CANARY (+1)] = lastValue;


        if (stack->data == NULL)
            stack->errorCode = badRealloc;

        else
            ON_CANARY (stack->data[stack->capacity + 1] = CANARY);
    }
*/

    ON_HASH_PROTECTION (stack->hash = djb2Hash(stack));

    STACK_ERRORS_CHECK(stack, file, dumpInfo);

    return stack->errorCode;
}

int stackVerifier (stack_t* stack) {
    if (stackPtrIsNull (stack, stdout))
        return badStackPtr;

    if (stack->data == NULL)
        stack->errorCode |= badDataPtr;

    if ((stack->size < 0) || ((stack->size > stack->capacity) && (stack->capacity >= 0))  || (stack->size > MAX_CAPACITY))
        stack->errorCode |= badSize;

    if ((stack->capacity <= 0) || (stack->capacity > MAX_CAPACITY))
        stack->errorCode |= badCapacity;

    #ifdef CANARY_PROTECTION
        if (stack->data[0] != CANARY)
            stack->errorCode |= deadFirstCanary;

        if (stack->data[stack->capacity + 1] != CANARY)
            stack->errorCode |= deadSecondCanary;
    #endif

    #ifdef HASH_PROTECTION
        if (stack->hash != djb2Hash(stack))
            stack->errorCode |= incorrectHash;
    #endif

    return stack->errorCode;
}

void stackDump (stack_t* stack, FILE* file, struct info dumpInfo) {
    assert(file);

    fprintf(file, "stackDump() from %s at %s:%d\n", dumpInfo.nameOfFunct, dumpInfo.nameOfFile, dumpInfo.numOfLine);

    if (stackPtrIsNull (stack, stdout))
        return;
    fprintf(file, "stack <%s> [%p] from %s at %s:%d\n", stack->nameOfType, stack, stack->stackInfo.nameOfFunct, stack->stackInfo.nameOfFile, stack->stackInfo.numOfLine);

    fprintfErrorForDump(stack, file);
    fprintf(file, "{\n");

    fprintf(file, "     size = %d", stack->size);
    if (stack->errorCode & badSize)
        fprintf(file, "  (BAD SIZE!)\n");
    else fprintf(file, "\n");

    fprintf(file, "     capacity = %d", stack->capacity);
    if (stack->errorCode & badCapacity)
        fprintf(file, "  (BAD CAPACITY!)\n");
    else fprintf(file, "\n");

    fprintf(file, "     data [%p]", stack->data);
    if (stack->errorCode & badDataPtr)
        fprintf(file, "  (NULL DATA POINTER!)\n");
    else fprintf(file, "\n");

    fprintf(file, "     {\n");
    if (!(stack->errorCode & badCapacity))
        fprintfStackData(file, *stack);

    fprintf(file, "     }\n");
    fprintf(file, "}\n");
}


void fprintfErrorForDump (stack_t* stack, FILE* file) {
    assert(file);

    if (stackPtrIsNull (stack, stdout))
        return;
    assert(file);

    if (stack->errorCode & badStackPtr)
        fprintf(file, "_______________________ERROR! NULL STACK POINTER(%d)\n", badStackPtr);

    if (stack->errorCode & badDataPtr)
        fprintf(file, "_______________________ERROR! NULL DATA POINTER(%d)\n", badDataPtr);

    if (stack->errorCode & badSize)
        fprintf(file, "_______________________ERROR! BAD SIZE(%d)\n", badSize);

    if (stack->errorCode & badCapacity)
        fprintf(file, "_______________________ERROR! BAD CAPACITY(%d)\n", badCapacity);

    if (stack->errorCode & badElementType)
        fprintf(file, "_______________________ERROR! UNSUPPORTED DATA TYPE!(%d)\n", badElementType);

    if (stack->errorCode & badRealloc)
        fprintf(file, "_______________________ERROR! REALLOC DID NOT WORK CORRECTLY!(%d)\n", badRealloc);

    if (stack->errorCode & noElementsForPop)
        fprintf(file, "_______________________ERROR! STACK HAS NO ELEMENTS FOR POP!(%d)\n", noElementsForPop);

    #ifdef CANARY_PROTECTION
        if (stack->errorCode & deadFirstCanary)
            fprintf(file, "_______________________ERROR! LEFT CANARY PROTECTION WAS BROKEN!(%d)\n", deadFirstCanary);

        if (stack->errorCode & deadSecondCanary)
            fprintf(file, "_______________________ERROR! RIGHT CANARY PROTECTION WAS BROKEN!(%d)\n", deadSecondCanary);
    #endif

    #ifdef HASH_PROTECTION
        if (stack->errorCode & incorrectHash)
            fprintf(file, "_______________________ERROR! INCORRECT HASH!(%d)\n", incorrectHash);
    #endif
}

int stackDtor (stack_t* stack, FILE* file, struct info* dumpInfo) {
    assert(file);
    assert(dumpInfo);

    if (stackPtrIsNull (stack, stdout))
        return badStackPtr;
    STACK_ERRORS_CHECK(stack, file, dumpInfo);

    free(stack->data);

    return(stack->errorCode);
}

void fprintfStackData (FILE* file, stack_t stack) {
    assert(file);

    #ifdef CANARY_PROTECTION
        fprintf(file, "          [0] = ");
        fprintfElement(file, (stack.data)[0]);
        if (stack.errorCode & deadFirstCanary)
            fprintf(file, "                ^^^^^^^ (CANARY PROTECTION WAS BROKEN!)\n");
    #endif

    for(ssize_t elementNum = 0 ON_CANARY (+1); elementNum < stack.capacity ON_CANARY (+1); elementNum++) {
        if((elementNum < stack.size ON_CANARY (+1)) && !(stack.errorCode & badSize)) {
            fprintf(file, "         *");
            fprintf(file, "[%d] = ", elementNum);
            fprintfElement(file, (stack.data)[elementNum]);
        }
        else {
            fprintf(file, "          [%d] = ", elementNum);
            fprintfElement(file, (stack.data)[elementNum]);
        }
    }

    #ifdef CANARY_PROTECTION
        fprintf(file, "          [%d] = ", (stack.capacity + 1));
        fprintfElement(file, (stack.data)[stack.capacity + 1]);
        if (stack.errorCode & deadSecondCanary)
            fprintf(file, "                ^^^^^^^ (CANARY PROTECTION WAS BROKEN!)\n");
    #endif
}

int stackPtrIsNull (stack_t* stack, FILE* file) {
    assert(file);

    if (stack == NULL) {
        printf("ERROR STACK POINTER IS NULL:\n");
        fprintf(file, "ERROR STACK POINTER IS NULL:\n");
        errno = EINVAL;
        perror("");
        return badStackPtr;
    }

    return 0;
}

#ifdef HASH_PROTECTION

unsigned long long djb2Hash (stack_t* stack) {
    if (stackPtrIsNull (stack, stdout))
        return 0;

    unsigned long long hash = 5381;

    #ifndef CANARY_PROTECTION
        for(ssize_t numOfElement = 0; numOfElement < stack->capacity; numOfElement++)
            hash = ((hash << 5) + hash) + stack->data[numOfElement];
    #endif

    #ifdef CANARY_PROTECTION
        for(ssize_t numOfElement = 0; numOfElement < stack->capacity + 2; numOfElement++)
            hash = ((hash << 5) + hash) + stack->data[numOfElement];
    #endif

    return hash;
}

#endif

void pouringPoison (stack_t* stack, ssize_t numOfStartElement) {
    if (stackPtrIsNull (stack, stdout))
        return;

    for(ssize_t numOfElement = numOfStartElement; numOfElement < stack->capacity ON_CANARY(+1); numOfElement++)
        stack->data[numOfElement] = POISON;
}

