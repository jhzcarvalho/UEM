#!/usr/bin/env python3
#
# Implementação de um algoritmo de programação dinâmica para resolver o
# problema do escalonamento de intervalos com peso.
#
# O programa pode ou não receber um nome de arquivo na linha de comando.
# - Se o nome do arquivo não for fornecido, o programa executa um teste.
# - Se o nome do arquivo for fornecido, o programa lê os intervalos
#   do arquivo e faz o escalonamento. Veja a função le_intervalos para
#   um descrição do formato do arquivo.

from typing import List
from dataclasses import dataclass

# Representa um intervalo.
# Requer que s <= f e v > 0.
@dataclass
class Inter:
    s: int # início
    f: int # término
    v: int # peso


# Encontra um subconjunto disjunto do intervalos no subarranjo A[0 : n] cuja a
# soma dos pesos seja máxima. A resposta é dada em ordem de início dos
# intervalos.
def escalona_intervalos(A: List[Inter], n: int) -> List[Inter]:
    # ESCREVA A FUNÇÃO PRINCIPAL AQUI.
    # Você vai precisar criar funções auxiliares.
    # Lembre-se que o arranjo é indexado de 0 e o último elemento é n-1.
    return A


# Verifica se a função escalona_intervalos funciona corretamente
# para o exemplo dos slides
def check():
    A = [Inter(1, 6, 8), Inter(0, 2, 4), Inter(4, 8, 6), Inter(7, 9, 2), Inter(3, 5, 3)]
    R = escalona_intervalos(A, 5)
    assert A == [Inter(0, 2, 4), Inter(4, 8, 6)]


# Lê os intervalos a partir do arquivo arq.
# O arquivo deve ter um intervalo por linha no formato
# s f v
def le_intervalos(arq: str) -> List[Inter]:
    inter = []
    for linha in open(arq):
        s, f, v = map(int, linha.split())
        inter.append(Inter(s, f, v))
    return inter


# Mostra o conjunto de intervalos e a soma dos seus pesos
def exibe_solucao(A: List[Inter]):
    soma = sum(i.v for i in A)
    print(f'Soma dos pesos: {soma}')
    print('Intervalos:')
    for i in A:
        print(f'{i.s} {i.f} {i.v}')


def main():
    import sys
    if len(sys.argv) == 1:
        print('Executando o teste com o exemplo do slide')
        check()
        print('Ok')
    if len(sys.argv) == 2:
        A = le_intervalos(sys.argv[1])
        r = escalona_intervalos(A, len(A))
        exibe_solucao(r)
    else:
        print('Número de parâmetros inválido')
        print('Modo de usar')
        print(f'{sys.argv[0]} [nome-do-arquivo]')

if __name__ == "__main__":
    main()
