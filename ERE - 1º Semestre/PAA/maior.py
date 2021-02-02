
def maior(A, min, max):
	if (min == max):
		return A[min]

	meio = (min + max) // 2

	if (A[meio] > A[meio + 1]):
		return maior(A, min, meio)
	else:
		return maior(A, meio+1, max)
	

A = [1, 2, 3, 4, 5, 6, 7, 3, 2, 1]

print (maior(A, 0, len(A)))