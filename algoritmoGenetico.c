#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct Cromossomo{		//definindo a estrutura do cromossomo
	short int bit[44];
	int aptidao;
}cromossomo;

void inicializaPop(cromossomo popAtual[6]);

int main(){
	srandom(time(NULL));
	int i, j;
	
	cromossomo popAtual[6];		//criamos cromossos para população atual
	cromossomo popProxima[6];	//criamos cromossos para prox população
	
	inicializaPop(popAtual);	//pop iniciada com bits aleatórios e sua aptidão definida
}

void inicializaPop(cromossomo popAtual[6]){
	int i, j;
	int numAleatorio;
	
	for (i = 0; i < 6; i++){
		for (j = 0; j < 44; j++){
			numAleatorio = random()% 2;
			popAtual[i].bit[j] = numAleatorio;
		}
		printf("cromossomo[%d] = ", i);
		for (j = 0; j < 44; j++)
			printf("%d", popAtual[i].bit[j]);
		printf("\n");
	}
}
