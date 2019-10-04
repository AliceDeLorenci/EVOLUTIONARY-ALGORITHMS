/*********************************  COMPILING  ************************************
 * 
 * g++ -Wall -o3 -march=native -D BESTMATES main.cpp maxvalue.cpp -o prog -lm
 * 
***********************************************************************************/

// IDEA: instead of plotting fitness x generation, I could plot fitter_gene X generation
// to have a better idea of concentration points

#include "maxvalue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int main(){

    FILE *Arq1, *Arq2, *Arq3, *Arq4, *Arq5;  //variables: x -> generation, y -> fitness

    /*
    Arq1  - fittest plot
    Arq2  - avarage plot
    Arq3  - inicial and final best genes
    Arq4  - plot both
    Arq5  - temp to be transfered to Arq4
    */

    Arq5 = fopen(temporary,"w+");

#ifdef ROULETTE

    Arq1 = fopen(fittest_roulette,"w+");
    Arq2 = fopen(avarage_roulette,"w+");
    Arq3 = fopen(genes_roulette,"w+");
    Arq4 = fopen(both_roulette,"w+");

#endif

#ifdef BESTMATES

    Arq1 = fopen(fittest_bestmates,"w+");
    Arq2 = fopen(avarage_bestmates,"w+");
    Arq3 = fopen(genes_bestmates,"w+");
    Arq4 = fopen(both_bestmates,"w+");

#endif

#ifdef TOURNAMENT

    Arq1 = fopen(fittest_tournament,"w+");
    Arq2 = fopen(avarage_tournament,"w+");
    Arq3 = fopen(genes_tournament,"w+");
    Arq4 = fopen(both_tournament,"w+");

#endif

    if(Arq1 == NULL || Arq2 == NULL || Arq3 == NULL || Arq4 == NULL || Arq5 == NULL){
        printf("WARNING");
        exit(0);
    } 

    int s; // size
    //int g; // genes
    int generation;

    ind pop[POP_SIZE];

    //variables that will keep an individual's parents indexes...
    int p1 = 0;
    int p2 = 0;

    int r; 
    for(r=0; r < REPEAT; r++){

        srand((unsigned)(time(0)+r));

        fprintf(Arq1,"\n\nv%d = {",r);
        fprintf(Arq4,"\n\nv%d = {",r);

        fprintf(Arq2,"\n\nv%d = {",r);
        fprintf(Arq5,"\n\nv%d = {",(r+REPEAT));

        fprintf(Arq3,"\n*************************** TEST %d ************************\n",r);

        initialize_population(pop);

        for(generation = 0; generation < GENERATION; generation++){

            fittness_function(pop);

            order(pop);  // pop[0] == fittest individual

            /* // PRINT ON TERMINAL

            printf("******************* GENERATION %d *******************\n",generation);

            for( s = 0; s < POP_SIZE; s++){
                printf("Individual %3d:", s);
                for( g = 0; g < GENES; g++){
                    printf("    %.2f",pop[s].c[g]);
                }
                printf("    Fitness: %.2f\n",pop[s].fit);
            }

            printf("Individual 0 is the fittest: %.2f   Fitness: %.2f\n\n",pop[0].c[0],pop[0].fit);

            */
            
            if(generation == GENERATION-1 || generation == 0)
                fprintf(Arq3,"Generation: %3d\tFittest gene: %3f\tScore: %3f\n",generation, pop[0].c[0],pop[0].fit);

            float avarage = Average_fitness(pop);

            if(generation == (GENERATION-1)){
                fprintf(Arq1,"{%d,%lf}}",generation, pop[0].fit);
                fprintf(Arq4,"{%d,%lf}}",generation, pop[0].fit);

                fprintf(Arq2,"{%d,%f}}",generation, avarage);
                fprintf(Arq5,"{%d,%f}}",generation, avarage);
            }else{
                fprintf(Arq1,"{%d,%lf},",generation, pop[0].fit);
                fprintf(Arq4,"{%d,%lf},",generation, pop[0].fit);

                fprintf(Arq2,"{%d,%f},",generation, avarage);
                fprintf(Arq5,"{%d,%f},",generation, avarage);
            }
            
            // CHOOSE PARENTS AND MATE: fittest individual mates with all of the population

            for( s = 1; s < POP_SIZE; s++){   //pop[0] is preserved == fittest

                #ifdef ROULETTE
                choose_parents_roulette(pop, &p1, &p2);
                ind child;
                child = cross_over(pop, p1, p2);
                pop[s] = child;
                #endif

                #ifdef BESTMATES
                p1 = 0; //one parent will always be the fittest individual
                choose_parents_best_mates(&p2);
                ind child;
                child = cross_over(pop, p1, p2);
                pop[s] = child;
                #endif

                #ifdef TOURNAMENT
                choose_parents_tournament(pop, &p1, &p2);
                ind child;
                child = cross_over(pop, p1, p2);
                pop[s] = child;
                #endif
            }
        }
    }

    // must close (save) before reading
    fclose(Arq5);
    Arq5 = fopen(temporary,"r");

    char letra;
    fscanf(Arq5,"%c",&letra);
    while(feof(Arq5)==0){ 
        //  OBS: if scanf was here, it would print twice the last character, because only after printing
        //  would it verify if EOF was reached
        fprintf(Arq4,"%c",letra);
        fscanf(Arq5,"%c",&letra);
    }


        fprintf(Arq1,"\nShow[{ListPlot[{");
        fprintf(Arq2,"\nShow[{ListPlot[{");
        fprintf(Arq4,"\nShow[{{ListPlot[{");


        for(r=0; r<REPEAT; r++){
            if(r == REPEAT-1){
                fprintf(Arq1,"v%d},Joined -> True]}, PlotRange -> {{0,%d},{0,%d}}, AxesOrigin ->{0,0}]",r, generation, UPPER_BOUND);
                fprintf(Arq4,"v%d},Joined -> True,PlotStyle -> Dashed]}",r);
                fprintf(Arq2,"v%d},Joined -> True]}, PlotRange -> {{0,%d},{0,%d}}, AxesOrigin ->{0,0}]",r, generation, UPPER_BOUND);
            }

            else{
                fprintf(Arq1,"v%d,",r);
                fprintf(Arq4,"v%d,",r);
                fprintf(Arq2,"v%d,",r);
            }
        }

        fprintf(Arq4,",{ListPlot[{");

        for(r=REPEAT ; r<(2*REPEAT); r++){
            if(r == (2*REPEAT)-1){
                fprintf(Arq4,"v%d},Joined -> True]}}, PlotRange -> {{0,%d},{0,%d}}, AxesOrigin ->{0,0}]",r, generation, UPPER_BOUND);
            }

            else{
                fprintf(Arq1,"v%d,",r);
                fprintf(Arq4,"v%d,",r);
                fprintf(Arq2,"v%d,",r);
            }
        }

        


    fclose(Arq1);
    fclose(Arq2);
    fclose(Arq3);
    fclose(Arq4);
    fclose(Arq5);

    return 0;
}