#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define POP 100
#define NUM_GER 400
#define TAXA_CROSSOVER 0.65
#define TAXA_MUTACAO 0.008

typedef struct Cromossomo{		//definindo a estrutura do cromossomo
	short int bit[44];
	float aptidao;
}cromossomo;

void inicializaPop(cromossomo popAtual[POP]);
float x(cromossomo popAtual);
float y(cromossomo popAtual);
float z(float x, float y);		
void listaRoleta(int *, cromossomo popAtual[POP]);	//função que aplica seleção por roleta e "retorna" um vetor de pares de pais
void crossover(int *, cromossomo *, cromossomo *);	//função que aplica o crossover baseado na lista de pares da roleta
void mutacao(cromossomo *);							//função que aplica a mutação de bits dependendo da TAXA_MUTACAO
void melhorCromossomo(cromossomo *, cromossomo *);	//função que retorna o melhor cromossomo de uma população
void elitismo(cromossomo *, cromossomo *); 			//aplica conceito de elitismo à pop de filhos

int main(){
	srandom(time(NULL));
	int i, j;
	int roleta[POP];
	float valorX, valorY, f6, mediaPop = 0;
	
	cromossomo popAtual[POP];	//criamos cromossomos para população atual
	cromossomo popProxima[POP];	//criamos cromossomos para prox população
	cromossomo mCromossomo;
	
	inicializaPop(popAtual);	//pop iniciada com bits aleatórios e sua aptidão definida
	melhorCromossomo(&mCromossomo, popAtual);

	printf("Melhor cromossomo: ");
	for (i = 0; i < 44; i++)
		printf("%d", mCromossomo.bit[i]);
	for (i = 0; i < POP; i++)
		mediaPop += popAtual[i].aptidao;
	mediaPop /= POP;
	printf(" médiaPop: %f aptidão: %f\n", mediaPop, mCromossomo.aptidao);

	for (i = 0; i < NUM_GER; i++){
		mediaPop = 0;
		printf("Geração %d:\n", i);
		listaRoleta(roleta, popAtual);	
		crossover(roleta, popAtual, popProxima); 
		mutacao(popProxima);

		for(j = 0; j < POP; j++){	//define a aptidão da população de filhos
			valorX = x(popProxima[j]);
			valorY = y(popProxima[j]);
			f6 = z(valorX, valorY);

			popProxima[j].aptidao = f6;
		}
		elitismo(&mCromossomo, popProxima);

		for (j = 0; j < POP; j++)	//passa os cromossomos de popProxima para popAtual
			popAtual[j] = popProxima[j];
		melhorCromossomo(&mCromossomo, popAtual);

		printf("Melhor cromossomo: ");
		for (j = 0; j < 44; j++)
			printf("%d", mCromossomo.bit[j]);
		valorX = x(mCromossomo);
		valorY = y(mCromossomo);
		for(j = 0; j < POP; j++)
			mediaPop += popAtual[j].aptidao;
		mediaPop /= POP;
		printf(" mediaPop: %f, x = %f, y = %f aptidão: %f\n", mediaPop, valorX, valorY, mCromossomo.aptidao);
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
		
		float vx = x(popAtual[i]);	//retorna o valor de x e y, insere o valor de f6(x,y) em aptidão
		float vy = y(popAtual[i]);
		float f6 = z(vx,vy);
		popAtual[i].aptidao = f6;
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
		numAleatorio = ((float)random()/(float)(RAND_MAX)) * maxAptidao;	//numAle recebe um float random no intervalo [0-somaAptid]
		
		somaAptidao = 0;
		j = 0;
		while(somaAptidao < numAleatorio){
			somaAptidao += popAtual[j].aptidao;
			j++;
		}
		vetor[i] = j-1;
	}
	
}

void crossover(int *lista, cromossomo *popAtual, cromossomo *popProxima){
	int i, j, pontoCorte;
	float  numCrossover, valorX, valorY, valorZ;
	for (i = 0; i < POP; i++){	//for que percorre cada cromossomo da população
		if(i % 2 == 0){
			pontoCorte = (random()%44) + 1;	//numAleatorio será o ponto de corte para a cabeça e cauda do crossover
			numCrossover = ((float)random()/(float)(RAND_MAX));
			
			if(numCrossover <= TAXA_CROSSOVER){
				for (j = 0; j < pontoCorte; j++){	//faz o crossover da "cabeça" do crossomo
					popProxima[i].bit[j] = popAtual[lista[i]].bit[j];	//crossover filho 1
					popProxima[i+1].bit[j] = popAtual[lista[i+1]].bit[j];	//crossover filho 2		
				}
				for (j = pontoCorte; j < 44; j++){	//faz o crossover da "cauda" do cromossomo
					popProxima[i].bit[j] = popAtual[lista[i+1]].bit[j];	//crossover filho 1
					popProxima[i+1].bit[j] = popAtual[lista[i]].bit[j];	//crossover filho 2
				}
			}else{				//copia os genitores
				popProxima[i] = popAtual[lista[i]];
				popProxima[i+1] = popAtual[lista[i]];
			}
		}
	}
}

void mutacao(cromossomo *popAtual){
	int i, j;
	float numMutacao;

	for (i = 0; i < POP; i++){
		for(j = 0; j < 44; j++){
			numMutacao = ((float)random()/(float)(RAND_MAX));	//numMutação define se ocorre a troca de bits do cromossomo
			if(numMutacao <= TAXA_MUTACAO){						//se numMutacao for <= a TAXA, os bits devem ser trocados
				if(popAtual[i].bit[j] == 0){
					popAtual[i].bit[j] = 1;
				}else{
					popAtual[i].bit[j] = 0;
				}
			}
		}
	}
}

void melhorCromossomo(cromossomo *mCromossomo,cromossomo *populacao){	//func que percorre o vetor de struct da geração e retorna
	int i, j;															//o cromossomo com maior aptidão

	mCromossomo->aptidao = populacao[0].aptidao;
	for (i = 0; i < POP; i++){
		if (mCromossomo->aptidao < populacao[i].aptidao){
			for (j = 0; j < 44; j++)
				mCromossomo->bit[j] = populacao[i].bit[j];
			mCromossomo->aptidao = populacao[i].aptidao;
		}
		
	}
}

void elitismo(cromossomo *melhorPai, cromossomo *popAtual){	//função que aplica o conceito de elitismo. Caso o melhor cromossomo da
	cromossomo mFilho;										//geração passada seja melhor que o da geração atual. Ele será inserido dentro
	int numAleatorio, i;									//da proxima geração

	melhorCromossomo(&mFilho, popAtual);

	if(mFilho.aptidao < melhorPai->aptidao){
		numAleatorio = random()%POP;

		for(i = 0; i < 44; i++)
			popAtual[numAleatorio].bit[i] = melhorPai->bit[i];
		popAtual[numAleatorio].aptidao = melhorPai->aptidao;
	}
}