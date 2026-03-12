#include <stdio.h>
#include <stdlib.h>

#define INC(x) x+1

int counter;

void run()
{
    int i;
    int *data;

    data = malloc(sizeof(int)*10);

    if(data)
    {
        for(i=0;i<10;i++)
        {
            data[i] = INC(i);
        }
    }

    if(counter)
        printf("Counter active\n");

    for(i=0;i<5;i++);

    if(data)
    {
        free(data);
    }

    goto end;

    counter = 0;

end:
    printf("Run finished\n");
}

int main()
{
    run();
    return 0;
}
