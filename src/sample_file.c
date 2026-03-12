

#include <stdio.h>     
#include <stdlib.h>    

#define SQUARE(x) x*x 

int global_var;        

static int counter;    

void foo();            

void foo()             
{
    int i;

    goto label;        

    for (i = 0; i < 10; i++) 
    {
        counter++;
    }

label:
    return;            
}

int main()             
{
    int x;
    int *ptr;

    if (x == 10)       
    {
        printf("x is 10\n");
    }

    ptr = (int *)malloc(sizeof(int));

    if (ptr != NULL)
    {
        *ptr = 5;
    }

    free(ptr);         

    x = SQUARE(1 + 2); 

    if (x > 0)
    {
        return 0;
    }
    else
    {
        return 1;     
    }
}
