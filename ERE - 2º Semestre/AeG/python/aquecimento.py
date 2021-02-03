from typing import List
import sys

# Exemplos:
ex1 = [1, 2, 5, 1]
ex2 = [2, 5, 1, 2]
ex3 = [1, 5, 2, 1]
ex4 = [1, 2, 1]
ex5 = [2]
ex6 = [5, 1, 2, 5]

def main():
    if verifica(ex4, ex5):
        print("True")
    else:
        print("False")

def verifica(c1: List[int], c2: List[int]):
    '''
    Verifica se o ciclo de dois vetores são iguais
    Exemplos:
        - Os caminhos [1, 2, 5, 1] e [2, 5, 1, 2] formam o mesmo ciclo
        - Os caminhos [1, 5, 2, 1] e [2, 5, 1, 2] não forma o mesmo ciclo
        - Os caminhos [1, 2, 1] e [2] não formam o mesmo ciclo

    '''

    if len(c1) != len(c2):
        return False


    # Remove o ultimo item da lista (por que é igual ao primeiro)
    c1.pop()
    c2.pop()


    # Verifica se c1 e c1 são iguais
    flag = False
    for i in range(len(c1)):
    # Rotaciona por todas as possbilidades de c2

        for j in range(len(c1)):
        # Testa se a lista c1 é igual a lista c2, elemento por elemento
            if c1[j] != c2[j]:
                break

            if j == (len(c1) - 1):
                return True

        rotaciona(c2)

    return False
    

def rotaciona(lista: List[int]):
    '''    
    Rotaciona uma determinada lista
    
    Exemplo:
    transforma [1, 2, 3] em [2, 3, 1]

    '''
    x = lista.pop(0)
    lista.append(x)

    return lista


# Chama a função principal
if __name__ == "__main__":
    main()
