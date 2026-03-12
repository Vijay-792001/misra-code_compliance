#include <stdio.h>

int value;

void process(int input)
{
    int i;
    int arr[10];

    for(i=0;i<10;i++)
    {
        arr[i] = i*input;
    }

    if(value == 1)
        printf("Value is 1\n");

    switch(input)
    {
        case 1:
            printf("Input 1\n");
        case 2:
            printf("Input 2\n");
            break;
        default:
            printf("Other\n");
    }

    goto label;

    value = 0;

label:
    printf("Done processing\n");
}

int main()
{
    process(2);
    return 0;
}
