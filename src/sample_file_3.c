#include <stdio.h>
#include <stdlib.h>

#define ADD(a,b) a+b

int *ptr;

void allocateMemory()
{
    int i;

    ptr = malloc(sizeof(int)*5);

    if(ptr)
    {
        for(i=0;i<5;i++)
        {
            ptr[i] = i;
        }
    }

    if(ptr != NULL)
    {
        printf("Memory allocated\n");
    }

    int result;

    result = ADD(2,3) * 5;

    printf("Result %d\n", result);
}

int main()
{
    allocateMemory();

    if(ptr)
    {
        free(ptr);
    }

    return 0;
}
