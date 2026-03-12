#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

#define MEM_SIZE 1024
#define BLOCK_COUNT 16

int memoryStatus;

struct Block
{
    int id;
    int size;
};

struct Block blocks[BLOCK_COUNT];

void initMemory();
void allocateMemory();
void freeMemory();

#define MEM_ADD(a,b) a+b

int computeMemory(int a, int b);

static int memCounter;

int memoryArray[50];

void printMemory()
{
}

int readBlock(int id);

int writeBlock(int id, int value);

#define MEM_ENABLE 1
#define MEM_DISABLE 0

int getMemoryStatus();

void setMemoryStatus(int status);

#define LIMIT_MEM 100

int resetMemory();

int checkMemory();

#define DOUBLE_MEM(x) x*2

int computeUsage(int a, int b);

int computeFree(int a, int b);

int computeFragment(int a, int b);

#endif
