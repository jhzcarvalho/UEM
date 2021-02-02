from typing import List
import sys

# Programa para verificar se dois caminhos formam o mesmo ciclo

def main():
    # Se nenhum argumento for especificado, executa os testes;
    # Senão é necessário dois argumentos, o nome do arquivo com os pares e o
    # nome da pessoal inicial.
    if len(sys.argv) == 1:
        test()
    elif len(sys.argv) == 2:
        run(sys.argv[1])
    else:
        print("Número de argumentos inválidos!")
        print(f"  use python3 {sys.argv[0]} arquivo-de-pares nome-pessoa-inicial")
        sys.exit(1)

# Verifica se dois caminhos distintos formam um mesmo ciclo, retornando o resultado da função em booleano
def verifica_ciclos(caminho1: List[int], caminho2: List[int]) -> bool:
    if len(caminho1) == 1 or len(caminho2) == 1:
        # Caso o tamanho de algum dos caminhos seja igual a 1, o caminho não forma ciclo
        return False
        
    elif len(caminho1) == len(caminho2):
        pos2 = 0
        for i in range(len(caminho1)):
            if caminho1[0] == caminho2[i]:
                pos2 = i
                break
        
        for i in range(len(caminho1)):
            if pos2 + i >= len(caminho2):
                pos2 = pos2 - len(caminho2) + 1
            if caminho1[i] != caminho2[pos2 + i]:
                return False
        return True
    
    return False

def test():
    
    teste1 = [1, 2, 5, 1]
    teste2 = [2, 5, 1, 2] #True
    teste3 = [1, 2, 5, 1] #True
    teste4 = [5, 1, 2, 5] #True
    teste5 = [5, 2, 1, 5] #False -- Ciclos diferentes
    teste6 = [5] #False -- Não forma ciclo
    
    esperado1 = True
    esperado2 = True
    esperado3 = True
    esperado4 = False
    esperado5 = False

    resultado1 = verifica_ciclos(teste1, teste2)
    resultado2 = verifica_ciclos(teste1, teste3)
    resultado3 = verifica_ciclos(teste1, teste4)
    resultado4 = verifica_ciclos(teste1, teste5)
    resultado5 = verifica_ciclos(teste1, teste6)

    assert resultado1 == esperado1
    assert resultado2 == esperado2
    assert resultado3 == esperado3
    assert resultado4 == esperado4
    assert resultado5 == esperado5

    print("Testes executados com sucesso.")

def run(arquivo: str):
    caminhos = abrirArquivo(arquivo)
    if verifica_ciclos(caminhos[0], caminhos[1]):
        print("Os caminhos formam o mesmo ciclo! :)")
    else:
        print("Os caminhos não formam o mesmo ciclo! :(")

    pass

def abrirArquivo(arquivo: str) -> List[List]:
    return list(open(arquivo).read().split())

if __name__ == "__main__":
    main()