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
            
        }
    }
}

ind random_individual(){
    ind newind;
    int j;
    for ( j = 0; j < GENES; j++){          
        newind.c[j] = rand()%RANGE;           
    }
    return newind;
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
    // worse individual in pop[POP_SIZE-1]

    int s;
    for( s=1 ; s<POP_SIZE ; s++){
        if(pop[0].fit < pop[s].fit){
            ind temp;
            temp = pop[0];
            pop[0] = pop[s];
            pop[s] = temp;
        }
    }
    for( s=0 ; s<(POP_SIZE-1) ; s++){
        if(pop[POP_SIZE-1].fit > pop[s].fit){
            ind temp;
            temp = pop[POP_SIZE-1];
            pop[POP_SIZE-1] = pop[s];
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

    // TOURNMENT of N individuals
    // smaller N -> more clusters
    // greater N -> quickest convergence

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

void initialize_genealogy (genealogy* historic){
    historic->gens = 0;
    historic->ptr = 0;
    historic->counter = 0;

}

ind cross_over (ind *pop, int p1, int p2, int mute){
    ind child;

    int i;
    //int p;

    for( i = 0; i < GENES; i++){
        // mean value from parents genes
        child.c[i] = (pop[p1].c[i] + pop[p2].c[i])/2;

        //p = rand()%100;
        //if(p < 50){
            //printf("BEFORE:%2.6f\t",child.c[i]);
            mutate(&(child.c[i]),mute);
        //}
    }
    

    return child;
}

float mutation_value(genealogy* historic, ind *pop, float *variation){

    //  LOW MUTATION RATES: quick convergence, low diversity
    //  HIGH MUTATION RATES: slow convergence, high diversity

    //  IDEA: before enlarging mutation rate, reduce it, to find actual local maximum

    float mute = MUTE;
    float deviation = 0, average = 0, temp;
    int i;

    historic->ptr = historic->gens%HISTORIC;        // where new fitness score will be stored

    historic->fit[historic->ptr] = pop[0].fit;

    if(historic->gens >= HISTORIC){     // if enough generations have already been generated...
        for(i=0; i<HISTORIC; i++){
            average += historic->fit[i];
        }
        average = average/HISTORIC;
        for(i=0; i<HISTORIC; i++){
            temp = average - historic->fit[i];
            if(temp > 0){
                deviation = deviation + temp;
            }
            else{
                deviation = deviation - temp;
            }
        }
        deviation = deviation/HISTORIC;
        
        *variation = deviation;

        if(FULLCYCLE == 1){

        // WARNING: growth of deviation will NOT exit routine 

            if(deviation < 0.01){
                historic->variablemute = 1;
            }

            if(historic->variablemute == 1){
                /*if(historic->counter == 0){
                    printf("Generation: %d  -  BEGINNING OF CYCLE\n",historic->gens);
                }*/
                if(historic->counter < INTERVAL){
                    mute = STAGE1;
                    historic->counter++;
                }
                else if(historic->counter < 2*INTERVAL){
                    mute = STAGE2;
                    historic->counter++;
                }
                else if(historic->counter < 3*INTERVAL){
                    mute = STAGE3;
                    historic->counter++;
                }
                else if(historic->counter < 4*INTERVAL){
                    mute = STAGE4;
                    historic->counter++;
                }
                if(historic->counter == 4*INTERVAL){
                    //printf("Generation: %d  -  END OF CYCLE\n\n",historic->gens);
                    historic->counter = 0;
                    historic->variablemute = 0;
                }
            }
        }
        else if(FULLCYCLE == 0){
            
            // WARNING: growth of deviation will exit routine 

            if(deviation < 0.01){
                if(historic->counter < INTERVAL){
                    mute = STAGE1;
                    historic->counter++;
                }
                else if(historic->counter < 2*INTERVAL){
                    mute = STAGE2;
                    historic->counter++;
                }
                else if(historic->counter < 3*INTERVAL){
                    mute = STAGE3;
                    historic->counter++;
                }
                else if(historic->counter < 4*INTERVAL){
                    mute = STAGE4;
                    historic->counter++;
                }
                if(historic->counter == 4*INTERVAL){
                    historic->counter = 0;
                }
            }
            else{
                historic->counter = 0;
            }                
        }
        
    }

    return mute;
}

void mutate(float *gene, int mute){

    int p = rand()%10;
    if(p<5){
        *gene = *gene + mute;
        if(*gene > RANGE){
            *gene = 100;
        }
    }
    else{
        *gene = *gene - mute;
        if(*gene < 0){
            *gene = (-1)*(*gene);
            //*gene = 0;
        }
    }
    //printf("AFTER: %2.6f\n\n",*gene);
}

float average_fitness(ind population[]){
    int i;
    float average = 0;
    for(i=0; i<POP_SIZE; i++){
        average += population[i].fit;
    }
    average = average/POP_SIZE;

    return average;
}