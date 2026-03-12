#include <stdio.h>

int flag;
int counter;

void checkFlag()
{
    int i;

    for(i=0;i<20;i++)
    {
        counter++;
    }

    if(flag == 1)
        printf("Flag is set\n");

    if(flag == 0)
        printf("Flag not set\n");

    switch(flag)
    {
        case 1:
            printf("One\n");
        case 2:
            printf("Two\n");
            break;
        default:
            printf("Default\n");
    }

    goto end;

    counter = 0;

end:
    printf("Done\n");
}

int main()
{
    checkFlag();
    return 0;
}
