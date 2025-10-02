#ifndef TEXT_STRUCTS_H
#define TEXT_STRUCTS_H

struct line {
    char* ptrToString;
    size_t lengthOfString;
};

struct comands {
    char* text;
    struct line* arrOfStringStructs;
    size_t sizeOfText;
    size_t numberOfStrings;
};

#endif
