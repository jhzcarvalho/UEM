#include <iostream>

int max_kadane(int *rev, int size) 
{ 
    int max = 0;
    int aux = 0; 
  
    for (int i = 0; i < size; i++) { 
        aux += rev[i]; 
        if (max < aux) max = aux; 
        if (aux < 0) aux = 0; 
    } 

    return max; 
} 

int max_it_n2(int *A, int n) {
	int soma; 
	int max = 0; 
	for (int i = 0; i < n; i++) {
		soma = 0;
		for (int j = i; j < n; j++) {
			soma += A[j];
			if (soma > max)
				max = soma;
		}
	}

	return max;
}
 
int main() 
{ 
	int n;

	while (std::cin >> n) {
   		short costPerDay;
   		std::cin >> costPerDay;

   		int rev[n];
   		for (int i = 0; i < n; ++i) 
   			std::cin >> rev[i];

   		for (int i = 0; i < n; ++i)
   			rev[i] -= costPerDay;

   		std::cout << "Kadane: " << max_kadane(rev, n) << std::endl; 		
   		std::cout << "Forca bruta: " << max_it_n2(rev, n) << std::endl;
	}
    
   	return 0; 
} 