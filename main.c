// Here we go
#include <stdio.h>
#include <stdlib.h>

int main () 
{     
    char mas[10][21];
    int i;

    sprintf(mas[0], "###################");
    for(i=1; i<9; i++)
 
        sprintf(mas[i], "#                 #");

    sprintf(mas[9], "###################");

    for(i=0; i<10; i++)

        printf("%s\n", mas[i]);

    return 0;

}
