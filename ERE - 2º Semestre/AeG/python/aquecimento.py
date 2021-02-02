import typing import list
import sys

# Exemplos:
ex1 = [1, 2, 5, 1]
ex2 = [2, 5, 1, 2]
ex3 = [1, 5, 2, 1]
ex4 = [1, 2, 1]
ex5 = [2]

def __main__():
    print(verifica(ex1, ex2))

def verifica(c1: List[int], c2: List[int]):
    '''
    Verifica se o ciclo de dois vetores são iguais
    Exemplos:
        - Os caminhos [1, 2, 5, 1] e [2, 5, 1, 2] formam o mesmo ciclo
        - Os caminhos [1, 5, 2, 1] e [2, 5, 1, 2] não forma o mesmo ciclo
        - Os caminhos [1, 2, 1] e [2] não formam o mesmo ciclo

    '''
    # Remove o ultimo item da lista (por que é igual ao primeiro)
    c1.pop()
    c2.pop()

    for 


def rotaciona(x: List[int]):
    '''    
    Rotaciona uma determinada lista
    
    Exemplo:
    transforma [1, 2, 3] em [3, 1, 2]

    '''

# Chama a função principal
if __name__ == "__main__":
    main()
