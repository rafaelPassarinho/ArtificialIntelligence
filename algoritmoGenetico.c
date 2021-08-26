#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define POP 6

typedef struct Cromossomo{		//definindo a estrutura do cromossomo
	short int bit[44];
	int aptidao;
}cromossomo;

void inicializaPop(cromossomo popAtual[POP]);
int x(cromossomo popAtual);
int y(cromossomo popAtual);
int z(int x, int y);		

int main(){
	srandom(time(NULL));
	int i, j;
	
	cromossomo popAtual[POP];	//criamos cromossos para população atual
	cromossomo popProxima[POP];	//criamos cromossos para prox população
	
	inicializaPop(popAtual);	//pop iniciada com bits aleatórios e sua aptidão definida
}

void inicializaPop(cromossomo popAtual[POP]){
	int i, j;
	int numAleatorio;
	
	for (i = 0; i < POP; i++){
		for (j = 0; j < 44; j++){	//popula os cromossomos com 1-0 aleatório
			numAleatorio = random()% 2;
			popAtual[i].bit[j] = numAleatorio;
		}
		printf("cromossomo[%d] = ", i);
		for (j = 0; j < 44; j++)
			printf("%d", popAtual[i].bit[j]);
		int vx = x(popAtual[i]);
		int vy = y(popAtual[i]);
		printf(" x = %d, y = %d", vx, vy);
		printf("\n");
	}
}

int x(cromossomo popAtual){		//função responsavel por retornar o x do cromossomo
	int i;
	int valorX = 0;
	int expoente = 0;
	
	for (i = 21; i >= 0; i--){	//aplica mudança de base binária para decimal no índice 21 até 0, menos significativo para mais sig.
		if(popAtual.bit[i] == 1)
			valorX += pow(2, expoente);
		expoente++;
	}
	return valorX;
}

int y(cromossomo popAtual){
	int i;
	int valorY = 0, expoente = 0;
	
	for(i = 43; i >= 22; i--){
		if (popAtual.bit[i] == 1)
			valorY += pow(2, expoente);
		expoente++;
	}
	return valorY;
}
