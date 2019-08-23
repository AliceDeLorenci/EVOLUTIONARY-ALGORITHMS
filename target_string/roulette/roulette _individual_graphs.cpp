// https://www.geeksforgeeks.org/genetic-algorithms/

// C++ program to create target string, starting from 
// random string using Genetic Algorithm 

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
Individual mate (Individual pop[], int par1, int par2);

//Calculates a generation's avarage fitness
float Average_fitness(Individual population[]);

//Chooses parents - ROULETTE method
void choose_parents (Individual pop[], int *p1, int *p2);

int main() 
{ 
    FILE *Arq1, *Arq2, *Arq3;  //variables: x -> generation, y -> fitness

    Arq1 = fopen("./fittest_i.nb","a+");
    Arq2 = fopen("./avarage_i.nb","a+");
    Arq3 = fopen("./generation_i.txt","a+");

    if(Arq1 == NULL || Arq2 == NULL || Arq3 == NULL){
        printf("WARNING");
        exit(0);
    }    

    int r;
    for(r=0; r<100; r++){

        fprintf(Arq1,"\n\nListPlot[{");
        fprintf(Arq2,"\n\nListPlot[{");

        srand((unsigned)(time(0)+r)); // unsigned -> only positive
                                      // r : guarantees a new seed for each loop
    
        float avarage;
        
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

        while(! found) // while found is false
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

            avarage = Average_fitness(population);
            fprintf(Arq1,"{%d,%d},",generation, population[0].fitness);
            fprintf(Arq2,"{%d,%f},",generation, avarage);
    
            // Otherwise generate new offsprings for new generation 
            Individual new_generation[POPULATION_SIZE]; 
    
            // 10% of fittest population goes to the next generation 
            int s = (10*POPULATION_SIZE)/100; 
            int i;
            for(i = 0; i<s; i++){
                new_generation[i] = population[i];   
            } 
    

            // ROULETTE WHEEL - fitness proportionate probability

            for(i = s; i<POPULATION_SIZE; i++) 
            { 
                int p1, p2;  // parents index

                choose_parents(population,&p1,&p2);

                new_generation[i] = mate(population,p1, p2);
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
        avarage = Average_fitness(population);
        fprintf(Arq1,"{%d,%d}}, PlotLabel -> \"FITTEST\"]",generation, population[0].fitness);
        fprintf(Arq2,"{%d,%f}}, PlotLabel -> \"AVARAGE FITNESS\"]",generation, avarage);
        fprintf(Arq3,"%d\n",generation);

    }

    fclose(Arq1);
    fclose(Arq2);
    fclose(Arq3);


    return 0;
} 


// ROULETTE WHEEL METHOD: probabilities proportional to an individual's fitness
// lower fitness score -> fitter individual

void choose_parents (Individual pop[], int *p1, int *p2){

    int i, sum = 0;
    int reverse[POPULATION_SIZE];  // higher score -> fitter individual
    int s = strlen(TARGET);

    for(i=0; i<POPULATION_SIZE; i++){
        reverse[i] = (s - pop[i].fitness);
        sum += reverse[i];
    }

    // bounds for each individual's share of the roulette wheel
    float sup[POPULATION_SIZE];
    float inf[POPULATION_SIZE];
    float share;

    int pp1, pp2;
    pp1 = rand()%100; // probability for parent 1
    pp2 = rand()%100; // probability for parent 2

    int end = 0;

    for( i=0; i<POPULATION_SIZE; i++){
        share = (reverse[i] / sum)*100;
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
    for(int i = 0; i<len; i++) 
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
                Individual temp;
                temp = population[i];
                population[i] = population[j];
                population[j] = temp;
/*
                char buffer[strlen(TARGET)];
                strcpy(buffer, population[i].chromosome);
                strcpy(population[i].chromosome, population[j].chromosome);
                strcpy(population[j].chromosome, buffer);

                int temp;
                temp = population[i].fitness;
                population[i].fitness = population[j].fitness;
                population[j].fitness = temp;
*/
            }
        }
    }
}

  
// Perform crossover and produce new offspring 
Individual mate (Individual pop[], int par1, int par2)
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
            child_chromosome[i] = pop[par1].chromosome[i]; 
  
        // if prob is between 0.45 and 0.90, insert gene from parent 2 
        else if(p < 0.90) 
            child_chromosome[i] = pop[par2].chromosome[i]; 
  
        // otherwise insert random gene(mutate), for maintaining diversity 
        else
            child_chromosome[i] = mutated_genes(); 
    } 
      
    // create offspring using generated chromosome for offspring 

    Individual child;
    strcpy(child.chromosome, child_chromosome);

    return child; 
}

//Calculates a generation's avarage fitness
float Average_fitness(Individual population[]){
    int i;
    float avarage = 0;
    for(i=0; i<POPULATION_SIZE; i++){
        avarage += population[i].fitness;
    }
    avarage = avarage/POPULATION_SIZE;

    return avarage;
}