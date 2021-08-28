#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define POP 6
#define NUM_GER 1

typedef struct Cromossomo{		//definindo a estrutura do cromossomo
	short int bit[44];
	float aptidao;
}cromossomo;

void inicializaPop(cromossomo popAtual[POP]);
float x(cromossomo popAtual);
float y(cromossomo popAtual);
float z(float x, float y);		
void listaRoleta(int *, cromossomo popAtual[POP]);

int main(){
	srandom(time(NULL));
	int i, j;
	int roleta[POP];
	
	cromossomo popAtual[POP];	//criamos cromossos para população atual
	cromossomo popProxima[POP];	//criamos cromossos para prox população
	
	inicializaPop(popAtual);	//pop iniciada com bits aleatórios e sua aptidão definida

	for (i = 0; i < NUM_GER; i++){
		printf("Geração %d:\n", i);

		listaRoleta(roleta, popAtual);	//função que aplica seleção por roleta e "retorna" um vetor de pares de pais
	}
	
	return 0;
}

void inicializaPop(cromossomo popAtual[POP]){	//função responsável por popular os cromossomos e atribuir valor aptidão a cada um
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
		
		float vx = x(popAtual[i]);	//retorna o valor de x e y, insere o valor de f6(x,y) em aptidão
		float vy = y(popAtual[i]);
		float f6 = z(vx,vy);
		popAtual[i].aptidao = f6;

		printf(" x = %f, y = %f, z = %f ", vx, vy, popAtual[i].aptidao);
		printf("\n");
	}
}

float x(cromossomo popAtual){	//função responsavel por transformar os bits[0-21](x) em real
	int i;
	float valorX = 0;
	int expoente = 0;
	
	for (i = 21; i >= 0; i--){	//aplica mudança de base binária para decimal no índice 21 até 0, menos significativo para mais sig.
		if(popAtual.bit[i] == 1)
			valorX += pow(2, expoente);
		expoente++;
	}
	valorX *= 200/(pow(2,22)-1);
	valorX += -100;

	return valorX;
}

float y(cromossomo popAtual){	//função responsavel por converter bit[22-43](y) para real
	int i;
	float valorY = 0;
	int expoente = 0;
	
	for(i = 43; i >= 22; i--){	//aplica mudança de base binária para decimal no índice 43 até 22, menos significativo para mais sig.
		if (popAtual.bit[i] == 1)
			valorY += pow(2, expoente);
		expoente++;
	}
	valorY *= 200/(pow(2,22)-1);
	valorY += -100;

	return valorY;
}

float z(float x, float y){	//retorna o resultado de f6(x,y)
	float resultado;
	float dividendo, divisor;

	dividendo = pow(sin((double)sqrtf(x*x + y*y)),2) - 0.5;
	divisor = pow(1.0 +0.001*(x*x + y*y),2);
	resultado = 0.5 - dividendo/divisor;
	
	return resultado;
}

void listaRoleta(int *vetor, cromossomo popAtual[POP]){	//função responsavel por aplicar seleção por roleta
	int i, j;
	float maxAptidao = 0, numAleatorio = 0, somaAptidao = 0;

	for (i = 0; i < POP; i++)
		maxAptidao += popAtual[i].aptidao;
	for (i = 0; i < POP; i++){
		numAleatorio = ((float)rand()/(float)(RAND_MAX)) * maxAptidao;	//numAle recebe um float random no intervalo [0-somaAptid]
		
		somaAptidao = 0;
		j = 0;
		while(somaAptidao < numAleatorio){
			somaAptidao += popAtual[j].aptidao;
			j++;
		}
		vetor[i] = j-1;
	}
	
}