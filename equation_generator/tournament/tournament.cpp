/************************************** COMPILING *****************************************

    g++ -o3 -march=native tournament.cpp -o prog -lm

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

// MATING MODEL: tournament selection

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define POP_SIZE 10
#define GENES 1
#define REPEAT 1
#define MUTE 0.1
#define GENERATION 50


typedef struct{

    float c[GENES];
    float fit;

} ind;

void initialize_population(ind *pop);

void fittness_function (ind *pop);

void order (ind *pop);

void choose_parents (ind *pop, int *p1, int *p2);

void mutate(float *gene);

ind cross_over (ind *pop, int p1, int p2);

float Average_fitness(ind population[]);

int main(){

    FILE *Arq1, *Arq2;  //variables: x -> generation, y -> fitness

    Arq1 = fopen("./T_fittest.nb","w+");
    Arq2 = fopen("./T_avarage.nb","w+");

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

        fprintf(Arq1,"\n\nv%d = {",r);
        fprintf(Arq2,"\n\nv%d = {",r);

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
            
            float avarage = Average_fitness(pop);
            if(generation == (GENERATION-1)){
                fprintf(Arq1,"{%d,%lf}}",generation, pop[0].fit);
                fprintf(Arq2,"{%d,%f}}",generation, avarage);
            }
            else{
                fprintf(Arq1,"{%d,%lf},",generation, pop[0].fit);
                fprintf(Arq2,"{%d,%f},",generation, avarage);

            }
            
            // CHOOSE PARENTS AND MATE: tournament

            for( s = 1; s < POP_SIZE; s++){   //pop[0] is preserved == fittest

                choose_parents(pop, &p1, &p2);
                ind child;
                child = cross_over(pop, p1, p2);
                pop[s] = child;
            }
        }
    }

    fprintf(Arq1,"\nShow[{");
        fprintf(Arq2,"\nShow[{");

        for(r=0; r<REPEAT; r++){
            if(r == REPEAT-1){
                fprintf(Arq1,"ListPlot[v%d]}, PlotRange -> All]",r);
                fprintf(Arq2,"ListPlot[v%d]}, PlotRange -> All]",r);
            }
            else{
                fprintf(Arq1,"ListPlot[v%d],",r);
                fprintf(Arq1,"ListPlot[v%d],",r);
            }
        }


    fclose(Arq1);
    fclose(Arq2);

    return 0;
}

void initialize_population(ind *pop){
    int i,j;

    srand((unsigned)(time(0)));

    for ( i = 0; i < POP_SIZE; i++){
        for ( j = 0; j < GENES; j++){
            pop[i].c[j] = rand()%10;  // range 0 to 9
            
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

    // FUNTION, whose MAXIMUN POINT shall be calculated
    // f(x) = x , 0 <= x < 5
    // f(x) = 10-x , 5 <= x < 10
    // f(x) = fittness

    int s, g;
    for( s=0 ; s<POP_SIZE ; s++){
        for( g=0 ; g<GENES ; g++){
            if(pop[s].c[g] < 5){
                pop[s].fit = pop[s].c[g];
            }
            else{
                pop[s].fit = 10 - pop[s].c[g];
            }
        }
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

void choose_parents (ind *pop, int *p1, int *p2){

    //choosing parent 1
    int c11, c12; //candidates
    c11 = rand()%POP_SIZE;
    c12 = rand()%POP_SIZE;
    if(pop[c11].fit > pop[c12].fit){
        *p1 = c11;
    } 
    else{
        *p1 = c12;
    }
    
    //choosing parent 2
    int c21, c22; //candidates
    c21 = rand()%POP_SIZE;
    c22 = rand()%POP_SIZE;
    if(pop[c21].fit > pop[c22].fit)
        *p2 = c21;
    else
        *p2 = c22;
}


ind cross_over (ind *pop, int p1, int p2){
    ind child;

    int i;

    for( i = 0; i < GENES; i++){
        // mean value from parents genes
        child.c[i] = (pop[p1].c[i] + pop[p2].c[i])/2.0;

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