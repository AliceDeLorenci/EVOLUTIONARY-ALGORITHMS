#include "maxvalue.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


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


void order_whole_pop (ind *pop){

    int i,j;
    for(i = 0; i < (POP_SIZE-1); i++){
        for(j = i+1; j < POP_SIZE; j++){
            if(pop[i].fit < pop[j].fit){
                ind temp;
                temp = pop[i];
                pop[i] = pop[j];
                pop[j] = temp;
            }
        }
    }
}

void choose_parents_best_mates (int *p2){

    *p2 = rand()%POP_SIZE;

}


void choose_parents_roulette (ind *pop, int *p1, int *p2){

    // ROULETTE WHEEL METHOD: probabilities proportional to an individual's fitness

    int i, sum = 0;
    for(i=0; i<POP_SIZE; i++){
        sum += pop[i].fit;
    }

    // bounds for each individual's share of the roulette wheel
    float sup[POP_SIZE];
    float inf[POP_SIZE];
    float share;

    int pp1, pp2;
    pp1 = rand()%100;
    pp2 = rand()%100;

    int end = 0;

    for( i=0; i<POP_SIZE; i++){
        share = (pop[i].fit / sum)*100;
        if(i == 0){
            inf[i] = 0;
        }
        else{
            inf[i] = sup[i-1];
        }
        sup[i] = inf[i] + share;
        if(pp1 >= inf[i] && pp1 < sup[i]){
            *p1 = i;
            end++;
        }
        if(pp2 >= inf[i] && pp2 < sup[i]){
            *p2 = i;
            end++;
        }
        if(end == 2){
            break;  // both parents have already been defined
        }

    }
}

void choose_parents_tournament (ind *pop, int *p1, int *p2){

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