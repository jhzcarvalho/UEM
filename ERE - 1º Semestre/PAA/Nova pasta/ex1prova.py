# Programa: Simulação da Estratégia Reducionista no Problema do Caixeiro Viajante

# Autor: Anderson Fabiano Dums

# Revisor: Manoel Vilela~

# Data: 16/05/2015

# Objetivo:

# Simulação de tempo que o computador que está executando o programa

# levaria para calcular algumas rotas para o caixeiro viajante sem

# utilizar Otimização Combinatória



import time


# Calcula o tempo que o processador da máquina leva para fazer 10 milhões

# de adições, cada edição é o calculo de um caminho (ligação entre duas

# cidade)

tempo = time.time()

i = sum(range(1, 10000000))

tempo = time.time() - tempo

print("Tempo para processar 10 milhões de adicoes (segundos): ", tempo)

adicoes = 10000000 / tempo

print("Adicoes por segundo: ", int(adicoes))



# Simula quanto tempo levaria para calcular a menor rota para o caixeiro

# passar por 5, 10, 15, 20 e 25 cidades e retornar a cidade origem

for contador in range(5,30,5):

    print("\nCidades", contador)

    rotasSeg = (adicoes / (contador -1))

    # Rotas são quantos caminhos completos são testados por segundo

    print("Rotas por segundo: ", int(rotasSeg))

    rotas = 1

    for i in range(contador - 1,1,-1):

        rotas = rotas * i

    print("Rotas possiveis: ", rotas)

    #Armazena o tempo em segundos

    tempoCalculo = (rotas / rotasSeg)

    if tempoCalculo < 0.001:
      print("Tempo para calculo: insignificante")
    elif tempoCalculo < 1:
      print("Tempo para calculo: ", int(tempoCalculo * 1000), " milisegundos")
    elif tempoCalculo < 1000:
      print("Tempo para calculo: ", int(tempoCalculo), " segundos")
    elif tempoCalculo < 60 * 60:
      print("Tempo para calculo: ", (tempoCalculo / 60), " minutos")
    elif tempoCalculo < 60 * 60 * 24:
      print("Tempo para calculo: ", tempoCalculo / (60 * 60), " horas")
    elif tempoCalculo < 60 * 60 * 24 * 365:
      print("Tempo para calculo: ", int(tempoCalculo / (60 * 60 * 24)), " dias")
    elif tempoCalculo < 60 * 60 * 24 * 365 * 1000 * 1000:
      print("Tempo para calculo: ", int(tempoCalculo / ( 60 * 60 * 24 * 365)), " anos")
    elif tempoCalculo < 60 * 60 * 24 * 365 * 1000 * 1000 * 1000:
      print("Tempo para calculo: ", int(tempoCalculo / ( 60 * 60 * 24 * 365 * 1000 * 1000)), " milhões de anos")
    else:
      print("Tempo para calculo: ", int(tempoCalculo / ( 60 * 60 * 24 * 365 * 1000 * 1000 * 1000)), " bilhões de anos")