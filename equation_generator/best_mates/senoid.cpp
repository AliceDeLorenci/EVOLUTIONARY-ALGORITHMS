/************************************** COMPILING *****************************************

    g++ -o3 -march=native best_mates.cpp -o prog -lm

******************************************************************************************/

/******************************** EVOLUTIONARY ALGORITHMS *********************************
 * 
 * 1 - initialize random population
 * 
 * 2 - avaliation
 * 
 *      2.1 - ordering
 *          for the sake of eficiency, ordering must be kept at the minimum necessary level
 * 
 * 3rd step: selection
 *      the best inviduals must have greatest probabilities of surviving
 *      exemple: * the best individual mates with all the rest  
 *               * elitism
 *               * tournment
 *               * roulette 
 * 
 * 4th step: crossover
 *      concatanate one piece of choromosssome from each parent
 *      for each gene, prbabilistic choose if it will come from father or mother
 * 
 * 5th step: mutation
 *      probabilistically choose an indiviudal to mutate
 *      probabilistically choose a gene to mutate 
 * 
 * 6th step: rearange population
 *      maintain best individuals, otherwise your solution might get worst over time
 *      eliminate old population and substitute for offspring 
 * 
********************************************************************************************/

// MATING MODEL: fittest individual mates with the rest of the population


/*******************************************************************************************
 TEST CASES:
 
 * 1,2
   MUTE == 0.1  -> converges to quickly, and to local solution

 * 3,4
   MUTE == 1 -> still converging to local solutions

 * 5
   MUTE == 5 -> still converging...

 * 6
   MUTE == 10 
   GENERATION == 100
   -> still converging... PROBLEM !!!
  
*******************************************************************************************/


// CHAGES TO FPRINTF  -> avarage and fittness together, same color, joined true, joined false

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define POP_SIZE 10     // population size
#define GENES 1         // number of genes per individual
#define REPEAT 10        // how many times GA will run
#define MUTE 10        // increse / decrease in mutated gene value
#define GENERATION 100   // number of generations that will be created
#define RANGE 100       // range for x
#define UPPER_BOUND 30  // upper bound for f(x)

#define fittest_notebook "./B_senoid_fittest6.nb"
#define avarage_notebook "./B_senoid_avarage6.nb"
#define genes_text "./B_senoid_gene6.txt"

//typedef int tipo;

typedef struct{

    float c[GENES];
    float fit;

} ind;

void initialize_population(ind *pop);

void fittness_function (ind *pop);

void order (ind *pop);

void choose_parents (int *p2);

void mutate(float *gene);

ind cross_over (ind *pop, int p1, int p2);

float Average_fitness(ind population[]);

int main(){

    FILE *Arq1, *Arq2, *Arq3;  //variables: x -> generation, y -> fitness

    Arq1 = fopen(fittest_notebook,"w+");
    Arq2 = fopen(avarage_notebook,"w+");
    Arq3 = fopen(genes_text,"w+");

    if(Arq1 == NULL || Arq2 == NULL){
        printf("WARNING");
        exit(0);
    } 

    int s; // size
    int g; // genes
    int generation;

    ind pop[POP_SIZE];

    //variables that will keep an individual's parents indexes...
    int p1 = 0;
    int p2 = 0;

    int r; 
    for(r=0; r < REPEAT; r++){

        srand((unsigned)(time(0)+r));

        fprintf(Arq1,"\n\nv%d = {",r);
        fprintf(Arq2,"\n\nv%d = {",r);
        fprintf(Arq3,"\n******************* TEST %d ****************\n",r);

        initialize_population(pop);

        for(generation = 0; generation < GENERATION; generation++){

            fittness_function(pop);

            order(pop);  // pop[0] == fittest individual

            printf("****** GENERATION %d ******\n",generation);

            for( s = 0; s < POP_SIZE; s++){
                printf("Individual %d:", s);
                for( g = 0; g < GENES; g++){
                    printf("    %.2f",pop[s].c[g]);
                }
                printf("    Fitness: %.2f\n",pop[s].fit);
            }
            // if more genes (variables) are included modify here...

            printf("Individual 0 is the fittest: %.2f   Fitness: %.2f\n\n",pop[0].c[0],pop[0].fit);
            
            if(generation == GENERATION-1 || generation == 0)
                fprintf(Arq3,"Generation: %d\tFittest gene: %f\n",generation, pop[0].c[0]);

            float avarage = Average_fitness(pop);
            if(generation == (GENERATION-1)){
                fprintf(Arq1,"{%d,%lf}}",generation, pop[0].fit);
                fprintf(Arq2,"{%d,%f}}",generation, avarage);
            }
            else{
                fprintf(Arq1,"{%d,%lf},",generation, pop[0].fit);
                fprintf(Arq2,"{%d,%f},",generation, avarage);

            }
            
            // CHOOSE PARENTS AND MATE: fittest individual mates with all of the population

            for( s = 1; s < POP_SIZE; s++){   //pop[0] is preserved == fittest

                p1 = 0; //one parent will always be the fittest individual
                choose_parents(&p2);
                ind child;
                child = cross_over(pop, p1, p2);
                pop[s] = child;
            }
        }
    }

        fprintf(Arq1,"\nShow[{ListPlot[{");
        fprintf(Arq2,"\nShow[{ListPlot[{");


        for(r=0; r<REPEAT; r++){
            if(r == REPEAT-1){
                fprintf(Arq1,"v%d},Joined -> True]}, PlotRange -> {{0,%d},{0,%d}}, AxesOrigin ->{0,0}]",r, generation, UPPER_BOUND);
                fprintf(Arq2,"v%d},Joined -> True]}, PlotRange -> {{0,%d},{0,%d}}, AxesOrigin ->{0,0}]",r, generation, UPPER_BOUND);
            }

            else{
                fprintf(Arq1,"v%d,",r);
                fprintf(Arq2,"v%d,",r);
            }
        }


    fclose(Arq1);
    fclose(Arq2);
    fclose(Arq3);

    return 0;
}

void initialize_population(ind *pop){
    int i,j;

    for ( i = 0; i < POP_SIZE; i++){
        for ( j = 0; j < GENES; j++){
            pop[i].c[j] = rand()%RANGE;  
            
            /* 
            // test the evolutionary model with and without mutation
            // the following if clause guarantees the optimun solution
            // won't be present on the first population, therefore,
            // if there is no mutation, it shall never be reached!
            */
            if(pop[i].c[j] == 5){
                pop[i].c[j] = pop[i].c[j] - 0.5;
            }
            
        }
    }
}

void fittness_function (ind *pop){

    int s;
    for( s=0 ; s<POP_SIZE ; s++){
        float x = pop[s].c[0];
        pop[s].fit = 2*cos(0.39*x) + 5*sin(0.5*x) + 0.5*cos(0.1*x) + 10*sin(0.7*x) + 5*sin(1*x) + 5*sin(0.35*x);
    }

}

void order (ind *pop){

    // this ordering funtion has the sole purpose of storing 
    // fittest individual in pop[0] 

    int s;
    for( s=1 ; s<POP_SIZE ; s++){
        if(pop[0].fit < pop[s].fit){
            ind temp;
            temp = pop[0];
            pop[0] = pop[s];
            pop[s] = temp;
        }
    }
}

void choose_parents (int *p2){

    *p2 = rand()%POP_SIZE;

}

ind cross_over (ind *pop, int p1, int p2){
    ind child;

    int i;

    for( i = 0; i < GENES; i++){
        // mean value from parents genes
        child.c[i] = (pop[p1].c[i] + pop[p2].c[i])/2;

        // all children shall mutate
        mutate(&(child.c[i]));
    }
    

    return child;
}

void mutate(float *gene){

    int p = rand()%10;
    if(p<5){
        *gene = *gene + MUTE;
    }
    else{
        *gene = *gene - MUTE;
    }
}

float Average_fitness(ind population[]){
    int i;
    float avarage = 0;
    for(i=0; i<POP_SIZE; i++){
        avarage += population[i].fit;
    }
    avarage = avarage/POP_SIZE;

    return avarage;
}