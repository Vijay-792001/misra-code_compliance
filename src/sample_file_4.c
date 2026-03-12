#include <stdio.h>

#define MULT(a,b) a*b

int total;

void compute()
{
    int x = 5;
    int y = 10;

    int result;

    result = MULT(x+1,y+1);

    if(result > 0)
    {
        printf("Positive\n");
    }
    else
    {
        printf("Negative\n");
    }

    int i;

    for(i=0;i<10;i++)
    {
        total++;
    }

    if(total)
        printf("Total updated\n");

    goto end;

    total = 0;

end:
    printf("Finished\n");
}

int main()
{
    compute();
    return 0;
}
