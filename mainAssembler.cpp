#include <stdio.h>
#include <assert.h>

#include "textStructs.h"
#include "assemblerFunctions.h"


int main (void) {

    struct comands Cmd = {};
    getStructComands (&Cmd, "source.asm");

    int* commandBuffer = commandRewriter (&Cmd, "source.asm");

    for(size_t i = 0; buffer[i] != 0; i++)
        printf("%d\n", buffer[i]);

}
