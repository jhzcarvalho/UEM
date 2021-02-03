from typing import List

ex1 = [1, 2, 5, 1]



def rotaciona(lista: List[int]):
    '''    
    Rotaciona uma determinada lista
    
    Exemplo:
    transforma [1, 2, 3] em [2, 3, 1]

    '''
    x = lista.pop(0)
    lista.append(x)

    return lista


x = rotaciona(ex1)
print(x)
