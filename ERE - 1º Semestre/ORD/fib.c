#include <stdio.h>
#include <stdlib.h>

int main(){
    
    int i,cont = 2;
    long x1,x2,x3;

    x1 = 0;
    x2 = 1;

    printf (" fibbonaci de 0 e:%ld \n",x1);
    printf (" fibbonaci de 1 e:%ld \n",x2);

    while (1){
        x3 = x1 + x2;
        printf (" fibbonaci de %d e:%ld \n",cont,x3);
        cont++;
        x1 = x2;
        x2 = x3;
        for (i = 0; i <= 32000; i = i + 2){
            i = i-1;
        }
        i = 0;
    }
}