// Here we go
#include <stdio.h>
#include <stdlib.h>

int main () 
{     
    char mas[10][20];
    int i;

    sprintf(mas[0], "###################");

    sprintf(mas[9], "###################");

    for(i=0; i<10; i++)
        printf("%s\n", mas[i]);

    return 0;

}
