from dataclasses import dataclass
from typing import Union

@dataclass
class Link:
    primeiro: int
    resto: 'Lista'

    def __repr__(self):
        return f'Link({self.primeiro}, {self.resto})'

Lista = Union[None, Link]

#Projete um algoritmo que receba como entrada uma Lista lst e conte quantos elementos lst tem
def somaLista(lst: Lista) -> int:
    if lst is None:
        return 0
    else:
        return lst.primeiro + somaLista(lst.resto)

assert somaLista(None) == 0
assert somaLista(Link(10, None)) == 10
assert somaLista(Link(2, Link(4, Link(5, None)))) == 11
print('ex1 - certo')

#Projete um algoritmo que receba como entrada uma Lista lst e verifique se todos os elementos
# de lst são pares.
def verificaPar(lst: Lista) -> bool:
    if lst is None:
        return True
    else:
        if (lst.primeiro % 2) == 0:
            return verificaPar(lst.resto)
        else:
            return False

assert verificaPar(None) == True
assert verificaPar(Link(10, None)) == True
assert verificaPar(Link(2, Link(4, Link(5, None)))) == False
print('ex2 - certo')

#Projete um algoritmo que receba como entrada uma Lista lst e um elemento 'a' e conte quantas 
# vezes 'a' aparece em lst.
def contaElemento(lst: Lista, elemento: int) -> int:
    if lst is None:
        return 0
    else:
        if lst.primeiro == elemento:
            return 1 + contaElemento(lst.resto, elemento)
        else:
            return contaElemento(lst.resto, elemento)

assert contaElemento(None, 2) == 0
assert contaElemento(Link(10, None), 2) == 0
assert contaElemento(Link(2, Link(4, Link(5, None))), 2) == 1
print('ex3 - certo')

#Projete um algoritmo que receba como entrada uma Lista lst e um elemento a e devolva uma Lista
#que é como lst mas sem as ocorrências dos valores maiores que a.

def retiraMaiorQue(lst: Lista, elemento: int) -> Lista:
    if lst is None:
        return None
    else:
        if (lst.primeiro > elemento):
            return retiraMaiorQue(lst.resto, elemento)
        else:
            return Link(lst.primeiro, retiraMaiorQue(lst.resto, elemento))

assert retiraMaiorQue(None, 2) == None
assert retiraMaiorQue(Link(1, Link(2, None)), 2) == Link(1, Link(2, None))
assert retiraMaiorQue(Link(1, Link(3, None)), 2) == Link(1, None)
print('ex4 - certo')