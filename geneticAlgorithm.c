#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define POP 100
#define NUM_GEN 4000
#define CROSSOVER_RATE 0.65
#define MUTATION_RATE 0.08

typedef struct Chrom{		//defining Chromosome struct
	short int bit[44];
	float fitness;
}chrom;

void popInitialization(chrom *);
float x(chrom currentPop);
float y(chrom currentPop);
float z(float x, float y);		
void listRoulette(int *, chrom *);	//function that applies roulette selection and "returns" a vector with parents couples
void crossover(int *, chrom *, chrom *);	//function that applies crossover based upon a parents couples list
void mutation(chrom *);						//function that applies mutation based on MUTATION_RATE
void bestChrom(chrom *, chrom *);	//function that returns the best chromosome of a population
void elitism(chrom *, chrom *); 			//applies elitism to the next population

int main(){				//main function
	srandom(time(NULL));
	int i, j;
	int roulette[POP];
	float xValue, yValue, f6, popMean = 0;
	
	chrom currentPop[POP];	//we create choromosomes for current population
	chrom nextPop[POP];	//we create chromosomes for next population
	chrom bChrom;
	
	popInitialization(currentPop);	//population initialized with random bits and thei fitness is calculated for each chrom
	bestChrom(&bChrom, currentPop);	//best chromosome returned from current population

	printf("Best Chromosome: ");	//print out the best chromosome from the original population
	for (i = 0; i < 44; i++)
		printf("%d", bChrom.bit[i]);
	for (i = 0; i < POP; i++)
		popMean += currentPop[i].fitness;
	popMean /= POP;
	printf(" Population mean: %f fitness: %f\n", popMean, bChrom.fitness);

	for (i = 0; i < NUM_GEN; i++){
		popMean = 0;
		printf("Generation %d:\n", i);
		listRoulette(roulette, currentPop);	
		crossover(roulette, currentPop, nextPop); 
		mutation(nextPop);

		for(j = 0; j < POP; j++){	//defines fitness for next population
			xValue = x(nextPop[j]);
			yValue = y(nextPop[j]);
			f6 = z(xValue, yValue);

			nextPop[j].fitness = f6;
		}
		elitism(&bChrom, nextPop);

		for (j = 0; j < POP; j++)	//passes chromosomes from nextPop[] to currentPop[]
			currentPop[j] = nextPop[j];
		bestChrom(&bChrom, currentPop);

		printf("Best Chromosome: ");
		for (j = 0; j < 44; j++)
			printf("%d", bChrom.bit[j]);
		xValue = x(bChrom);
		yValue = y(bChrom);
		for(j = 0; j < POP; j++)
			popMean += currentPop[j].fitness;
		popMean /= POP;
		printf(" Population mean: %f, x = %f, y = %f fitness: %f\n", popMean, xValue, yValue, bChrom.fitness);
	}
	
	return 0;
}

void popInitialization(chrom *currentPop){	//function responsible for populate the chromosomes and atribute a fitness value for each one
	int i, j;
	int randomNum;
	
	for (i = 0; i < POP; i++){
		for (j = 0; j < 44; j++){	//populates each chromosome with random 0-1
			randomNum = random()% 2;
			currentPop[i].bit[j] = randomNum;
		}
		
		float xValue = x(currentPop[i]);	//return x, y values and insert f6(x,y) in fitness
		float yValue = y(currentPop[i]);
		float f6 = z(xValue,yValue);
		currentPop[i].fitness = f6;
	}
}

float x(chrom currentPop){	//function responsible for converting the bits from [0-21](x) to decimal
	int i;
	float xValue = 0;
	int exponent = 0;
	
	for (i = 21; i >= 0; i--){	//applies base change from index 21 to 0. From least significant to most significant
		if(currentPop.bit[i] == 1)
			xValue += pow(2, exponent);
		exponent++;
	}
	xValue *= 200/(pow(2,22)-1);
	xValue += -100;

	return xValue;
}

float y(chrom currentPop){	//function responsible for converting the bits from [22-43](y) to decimal
	int i;
	float yValue = 0;
	int exponent = 0;
	
	for(i = 43; i >= 22; i--){	//applies base change from index 43 to 22. From least significant to most significant
		if (currentPop.bit[i] == 1)
			yValue += pow(2, exponent);
		exponent++;
	}
	yValue *= 200/(pow(2,22)-1);
	yValue += -100;

	return yValue;
}

float z(float x, float y){	//returns f6(x,y) value
	float result;
	float dividend, divisor;

	dividend = pow(sin((double)sqrtf(x*x + y*y)),2) - 0.5;
	divisor = pow(1.0 +0.001*(x*x + y*y),2);
	result = 0.5 - dividend/divisor;
	
	return result;
}

void listRoulette(int *vector, chrom *currentPop){	//function responsible for applying roulette selection
	int i, j;
	float maxFitness = 0, randomNum = 0, sumFitness = 0;

	for (i = 0; i < POP; i++)
		maxFitness += currentPop[i].fitness;

	for (i = 0; i < POP; i++){
		randomNum = ((float)random()/(float)(RAND_MAX)) * maxFitness;	//randomNum receives a random float in the range [0-sumFitness]
		
		sumFitness = 0;
		j = 0;
		while(sumFitness < randomNum){
			sumFitness += currentPop[j].fitness;
			j++;
		}
		vector[i] = j-1;
	}
	
}

void crossover(int *list, chrom *currentPop, chrom *nextPop){
	int i, j, crossoverPoint;
	float  crossoverNum;
	for (i = 0; i < POP; i++){	//walks each chromosome of the population
		if(i % 2 == 0){
			crossoverPoint = (random()%44) + 1;	//crossoverPoint will be the random cut point between "head" and "tail" from the chromosome
			crossoverNum = ((float)random()/(float)(RAND_MAX)); 
			
			if(crossoverNum <= CROSSOVER_RATE){	//if crossoverNum is between [0,CROSSOVER_RATE] the crossover happens
				for (j = 0; j < crossoverPoint; j++){	//crossing the bits above the cross point index
					nextPop[i].bit[j] = currentPop[list[i]].bit[j];	//crossover child 1
					nextPop[i+1].bit[j] = currentPop[list[i+1]].bit[j];	//crossover child 2		
				}
				for (j = crossoverPoint; j < 44; j++){	//crossing the bits below the cross point index 
					nextPop[i].bit[j] = currentPop[list[i+1]].bit[j];	//crossover child 1
					nextPop[i+1].bit[j] = currentPop[list[i]].bit[j];	//crossover child 2
				}
			}else{				//else, copy the parents
				nextPop[i] = currentPop[list[i]];
				nextPop[i+1] = currentPop[list[i]];
			}
		}
	}
}

void mutation(chrom *currentPop){
	int i, j;
	float mutationNum;

	for (i = 0; i < POP; i++){
		for(j = 0; j < 44; j++){
			mutationNum = ((float)random()/(float)(RAND_MAX));	//mutationNum defines if a bit from the chromossome will mutate
			if(mutationNum <= MUTATION_RATE){					//if mutationNum is less than MUTATION_RATE, the bit will be inverted for <= a TAXA, os bits devem ser trocados
				currentPop[i].bit[j] = 1 - currentPop[i].bit[j];
			}
		}
	}
}

void bestChrom(chrom *bChrom,chrom *population){	//function that walks the struct vector of population and returns the chromosome if greatest fitness
	int i, j;

	bChrom->fitness = population[0].fitness;
	for (i = 0; i < POP; i++){
		if (bChrom->fitness < population[i].fitness){
			for (j = 0; j < 44; j++)
				bChrom->bit[j] = population[i].bit[j];
			bChrom->fitness = population[i].fitness;
		}
		
	}
}

void elitism(chrom *bestParent, chrom *currentPop){	//function that applies the concept of elitism. If the best chromosome from current population has a better fitness than the one from next pop,
	chrom bestChild;								//he will be inserted in the next population
	int randomNum, i;

	bestChrom(&bestChild, currentPop);

	if(bestChild.fitness < bestParent->fitness){
		randomNum = random()%POP;

		for(i = 0; i < 44; i++)
			currentPop[randomNum].bit[i] = bestParent->bit[i];
		currentPop[randomNum].fitness = bestParent->fitness;
	}
}
