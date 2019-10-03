/***********************************************************************************************
 * C program to find user set target string starting from random generated string,
 * using EVOLUTIONARY ALGORITHM
 * 
 * SELECTION METHOD: tournament
 * Each parent is defined by choosing the fittest individual in a tournament between
 * two randomly selected individuals
 * 
 * COMPILE AND RUN
 * g++ -Wall tournament.cpp -o tournament.exe
 * ./tournament.exe
 * 
 * OUTPUT
 * Each generation's best guessed string is printed on the terminal
************************************************************************************************/


#define REPEAT 1      // number of tests that will be run

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Number of individuals in each generation 
#define POPULATION_SIZE 100 
  
// Valid Genes 
const char GENES[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890, .-;:_!\"#%&/()=?@${[]}"; 
  
// Target string to be generated 
const char TARGET[] = "Evolutionary Algorithms"; 

// Struct representing individual in population 
typedef struct
{ 
    char chromosome[strlen(TARGET)]; 
    int fitness; 
}Individual;
  
// Function to generate random numbers in given range  
int random_num(int start, int end);
  
// Create random genes for mutation 
char mutated_genes();
  
// Create string of genes (chromosome)
void create_gnome(char *created_gnome);
  
// Calculate fittness score (number of characters in string which differ from target string)
int cal_fitness(char genes[]); 

//Order population in crescent order of fitness score
void Sort (Individual population[]);
  
// Perform crossover and produce new offspring 
Individual mate (Individual *par1, Individual *par2);

//Calculates a generation's average fitness
float Average_fitness(Individual population[]);

void choose(Individual *pop, int *p1, int *p2);

int main() 
{ 

    FILE *Arq1, *Arq2, *Arq3;

    // fittest individual score per generation
    Arq1 = fopen("./fittest.txt","w+");

    // average fitness per generation
    Arq2 = fopen("./average.txt","w+");

    // number of generations needed per test to find target string
    Arq3 = fopen("./generation.txt","w+");


    if(Arq1 == NULL || Arq2 == NULL || Arq3 == NULL){
        printf("WARNING");
        exit(0);
    }    


    int r;
    for(r=0; r<REPEAT; r++){

    
        fprintf(Arq1,"\n\nv%d = {",r);
        fprintf(Arq2,"\n\nv%d = {",r);
        

        srand((unsigned)(time(0)+r)); //unsigned -> only positive
    
        float average;
        
        // current generation 
        int generation = 0; 
    
        Individual population[POPULATION_SIZE]; 
        bool found = false; 
    
        // create initial population 
        for(int i = 0; i<POPULATION_SIZE; i++) 
        { 
            char created_gnome[strlen(TARGET)];
            create_gnome(created_gnome);
            strcpy(population[i].chromosome,created_gnome);

            population[i].fitness = cal_fitness(population[i].chromosome);
        } 

        while(! found) 
        { 
            
            Sort(population); 
    
            // if the individual having lowest fitness score is 
            // 0 then we know that we have reached to the target 
            // and break the loop 
            if(population[0].fitness <= 0) 
            { 
                found = true; 
                break; 
            } 
            //Print fittest
            printf("Generation: %d\t",generation);
            printf("String: %s\t",population[0].chromosome);
            printf("Fitness: %d\n",population[0].fitness);

            average = Average_fitness(population);
            fprintf(Arq1,"{%d,%d},",generation, population[0].fitness);
            fprintf(Arq2,"{%d,%f},",generation, average);
    
            // Otherwise generate new offsprings for new generation 
            Individual new_generation[POPULATION_SIZE]; 
    
            // 10% of fittest population goes to the next generation 
            int s = (10*POPULATION_SIZE)/100; 
            int i;
            for(i = 0; i<s; i++){
                new_generation[i] = population[i];   
            } 
    

            // 90% of offspring is produced by TOURNAMENT 
            for(i = s; i<POPULATION_SIZE; i++) 
            { 
                int p1, p2; //index of parents
                choose(population,&p1,&p2);
                Individual parent1 = population[p1];
                Individual parent2 = population[p2];
                new_generation[i] = mate(&parent1,&parent2);
            } 


            for(i = 0; i<POPULATION_SIZE; i++){
                population[i] = new_generation[i]; 
                population[i].fitness = cal_fitness(population[i].chromosome);
            }

            generation++; 
        } 
        
        printf("Generation: %d\t",generation);
        printf("String: %s\t",population[0].chromosome);
        printf("Fitness: %d\n",population[0].fitness);
        average = Average_fitness(population);

        fprintf(Arq1,"{%d,%d}}",generation, population[0].fitness);
        fprintf(Arq2,"{%d,%f}}",generation, average);
        fprintf(Arq3,"%d\n",generation);

    }

    fclose(Arq1);
    fclose(Arq2);
    fclose(Arq3);

    return 0;
} 

//TOURNAMENT
void choose(Individual *pop, int *p1, int *p2){
    //choosing parent 1
    int c11, c12; //candidates
    c11 = rand()%POPULATION_SIZE;
    c12 = rand()%POPULATION_SIZE;
    if(pop[c11].fitness > pop[c12].fitness)
        *p1 = c11;
    else
        *p1 = c12;
    
    //choosing parent 2
    int c21, c22; //candidates
    c21 = rand()%POPULATION_SIZE;
    c22 = rand()%POPULATION_SIZE;
    if(pop[c21].fitness > pop[c22].fitness)
        *p2 = c21;
    else
        *p2 = c22;
}

// Function to generate random numbers in given range  
int random_num(int start, int end) 
{ 
    int range = (end-start)+1; 
    int random_int = start+(rand()%range); 
    return random_int; 
} 
  
// Create random genes for mutation 
char mutated_genes() 
{ 
    int len = strlen(GENES); 
    int r = random_num(0, len-1); 
    return GENES[r]; 
} 
  
// Create string of genes (chromosome)
void create_gnome(char *created_gnome) 
{ 
    int len = strlen(TARGET);

    int i;
    for(i = 0; i < len; i++){ 
        created_gnome[i] = mutated_genes(); 
    }
}  
  
// Calculate fittness score (number of characters in string which differ from target string)
int cal_fitness(char genes[]) 
{ 
    int len = strlen(TARGET); 
    int fitness = 0; 
    for(int i = 0;i<len;i++) 
    { 
        if(genes[i] != TARGET[i]) 
            fitness++; 
    } 
    return fitness;     
} 

//Order population in crescent order of fitness score
//Lower scores indicate fitter individuals
void Sort (Individual population[])
{
    int i, j;

    for(i=0; i<(POPULATION_SIZE-1); i++){
        for(j=i+1; j<POPULATION_SIZE; j++){
            if(population[i].fitness > population[j].fitness){

                char buffer[strlen(TARGET)];
                strcpy(buffer, population[i].chromosome);
                strcpy(population[i].chromosome, population[j].chromosome);
                strcpy(population[j].chromosome, buffer);

                int temp;
                temp = population[i].fitness;
                population[i].fitness = population[j].fitness;
                population[j].fitness = temp;
            }
        }
    }
}

  
// Perform crossover and produce new offspring 
Individual mate (Individual *par1, Individual *par2)
{
    // chromosome for offspring 
    char child_chromosome[strlen(TARGET)]; 
  
    int len = strlen(TARGET);
    int i;

    for(i = 0; i<len; i++) 
    { 
        // random probability  
        float p = random_num(0, 100)/100; 
  
        // if prob is less than 0.45, insert gene from parent 1  
        if(p < 0.45) 
            child_chromosome[i] = par1->chromosome[i]; 
  
        // if prob is between 0.45 and 0.90, insert gene from parent 2 
        else if(p < 0.90) 
            child_chromosome[i] = par2->chromosome[i]; 
  
        // otherwise insert random gene(mutate), for maintaining diversity 
        else
            child_chromosome[i] = mutated_genes(); 
    } 
      
    // create offspring using generated chromosome for offspring 

    Individual child;
    strcpy(child.chromosome, child_chromosome);

    return child; 
}

//Calculates a generation's average fitness
float Average_fitness(Individual population[]){
    int i;
    float average = 0;
    for(i=0; i<POPULATION_SIZE; i++){
        average += population[i].fitness;
    }
    average = average/POPULATION_SIZE;

    return average;
}