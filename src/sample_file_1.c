#include <stdio.h>
#include <stdlib.h>

#define SQR(x) x*x

int global_count;

void funcA();

void funcA()
{
    int i;
    int arr[10];

    for(i = 0; i < 10; i++)
    {
        arr[i] = i;
    }

    if(global_count)
        printf("Global active\n");

    global_count = SQR(1+2);

    for(i=0;i<5;i++);

    if(arr[0] == 0)
    {
        goto label;
    }

    printf("Array initialized\n");

label:
    printf("End of function\n");
}

int main()
{
    int x;

    if(x == 5)
    {
        printf("Uninitialized variable\n");
    }

    funcA();

    return 0;
}
