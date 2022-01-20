
#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    int i;
    for (i = 0; i < 1000000; i++) {
        if(rand()%(1000000) < 10)
            printf("%d\n",i);
    }
    getchar();
}

