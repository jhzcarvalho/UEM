#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    float fim = 0;
    float resultado = 0;
    float i = 0;
    float x = 0;
    float a = 0;
    int exe;
    int aux;
    float passo;
    do
    {
        printf("\n\t EXERCICIO 15:limite[1/2x+3] qdo x->[+inf]");
        printf("\n\t EXERCICIO 16:limite[3x+5/x-4] qdo x->[+inf]");
        printf("\n\t EXERCICIO 26:limite[x + sqrt(x^2 + 2x)] qdo x-> [-inf]");
        printf("\n\t EXERCICIO 29:limite[x^4-3x^2+x/x^3-x+2]qdo x->[+inf]");
        printf("\n\t EXERCICIO 32:limite[1+x^6/x^4+1] qdo x->[-inf]");
        printf("\n\t EXERCICIO 31:limite[x^4+x^5] qdo x->[+inf]");
        printf("\n\t EXERCICIO 36:limite[sin(x)^2/(x^2 + 1)] qdo x tende +inf\n");
        printf("0:sair do programa\n");

        printf("\n\t digite o numero do exercicio que deseja explorar:\n");
        scanf("%d", &exe);
        switch (exe)
        {
        case 15:
        {
            printf("\n\tEXERCICIO 15 LIMITE [1/2X + 3] qdo x tende +inf\n");
            printf("\n\t digite um valor inicial para o x: ");
            scanf(" %f", &x);
            printf("\n\t digite um valor final para o x: ");
            scanf(" %f", &fim);
            printf("\n\t digite um valor para o a de x: ");
            scanf(" %f", &a);
            for (i = x; i <= fim; i = i + a)
            {

                resultado = 1 / (2 * i + 3);
                printf("\n\to valor da funcao em x = %.6f eh %.6f\n", i, resultado);
            }
            printf("pressione 0 para ir ao proximo exercicio: ");
            scanf("%d", &aux);
            break;
        }
        case 16:
        {
            printf("\n\t EXERCICIO 16 LIMITE [2X + 5/X-4] qdo x tende +inf\n");
            printf("\n\t digite um valor inicial para o x: ");
            scanf(" %f", &x);
            printf("\n\t digite um valor final para o x: ");
            scanf(" %f", &fim);
            printf("\n\t digite um valor para o a de x: ");
            scanf(" %f", &a);
            for (i = x; i <= fim; i = i + a)
            {
                resultado = (2 * i + 5) / (i - 4);
                printf("\n\to valor da funcao em x = %.6f eh %.6f\n", i, resultado);
            }
            printf("pressione 0 para ir ao proximo exercicio: ");
            scanf("%d", &aux);
            break;
        }
        case 26:
        {
            printf("\n\t EXERCICIO 26 LIMITE [x + sqrt(x^2 + 2x)] qdo x tende -inf\n");
            printf("\n\tdigite um valor inicial para o x: ");
            scanf(" %f", &x);
            printf("\n\tdigite um valor final para o x: ");
            scanf(" %f", &fim);
            printf("\n\tdigite um valor para o a de x: ");
            scanf(" %f", &a);
            for (i = x; i <= fim; i = i + a)
            {
                resultado = (i + (sqrt(pow(i, 2) + (2 * i))));
                printf("\n\t o valor da funcao em x = %.6f eh %.6f\n", i, resultado);
            }
            printf("pressione 0 para ir ao proximo exercicio: ");
            scanf("%d", &aux);
            break;
        }
        case 29:
        {
            printf("\n\t EXERCICIO 29 limite[x^4-3x^2+x/x^3-x+2] qdo x tende +inf\n");
            printf("\n\t digite o valor inicial para o x:");
            scanf("%f", &x);
            printf("\n\t digite um valor final para o x: ");
            scanf(" %f", &fim);
            printf("\n\t digite um valor para o a de x: ");
            scanf(" %f", &a);
            for (i = x; i <= fim; i = i + a)
            {
                resultado = (pow(i, 4) - 3 * pow(i, 2) + i) / (pow(i, 3) - i + 2);
                printf("\n\t o valor da funcao em x = %.6f eh %.6f\n", i, resultado);
            }
            printf("pressione 0 para ir ao proximo exercicio: ");
            scanf("%d", &aux);
            break;
        }
        case 32:
        {
            printf("\n\t EXERCICIO 32 limite[1+x^6/x^4+1] qdo x tende -inf\n");
            printf("\n\t digite o valor inicial para o x:");
            scanf("%d", &x);
            printf("\n\t digite um valor final para o x: ");
            scanf(" %f", &fim);
            printf("\n\t digite um valor para o a de x: ");
            scanf(" %f", &a);
            scanf(" %f", &passo);
            for (i = x; i <= fim; i = i + a)
            {
                resultado = (1 + pow(i, 6)) / (pow(i, 4) + 1);
                printf("\n\to valor da funcao em x = %.6f eh %.6f\n", i, resultado);
            }
            printf("pressione 0 para ir ao proximo exercicio: ");
            scanf("%d", &aux);
            break;
        }
        case 31:
        {
            printf("\n\tEXERCICIO 31 LIMITE [x^4+x^5] qdo x tende +inf\n");
            printf("\n\tdigite um valor inicial para o x: ");
            scanf(" %f", &x);
            printf("\n\tdigite um valor final para o x: ");
            scanf(" %f", &fim);
            printf("\n\tdigite um valor para o passo de x: ");
            scanf(" %f", &a);
            for (i = x; i <= fim; i = i + a)
            {
                resultado = pow(i, 4) + pow(i, 5);
                printf("\n\to valor da funcao em x = %.6f eh %.6f\n", i, resultado);
            }
            printf("pressione 0 para ir ao proximo exercicio: ");
            scanf("%d", &aux);
            break;
        }
        case 36:
        {
            printf("\n\t EXERCICIO 36 LIMITE [sin(x)^2/(x^2 + 1)] qdo x tende +inf\n");
            printf("\n\tdigite um valor inicial para o x: ");
            scanf(" %f", &x);
            printf("\n\tdigite um valor final para o x: ");
            scanf(" %f", &fim);
            printf("\n\tdigite um valor para o passo de x: ");
            scanf(" %f", &passo);
            for (i = x; i <= fim; i = i + passo)
            {
                resultado = pow(sin(i), 2) / (pow(x, 2) + 1);
            }
        }
        case 0:
            printf("\n\tvoce optou por sair do programa\t\n");
            break;
        }

    } while (exe != 0);

    return 0;
}