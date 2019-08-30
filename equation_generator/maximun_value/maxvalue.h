
#define POP_SIZE 10     // population size
#define GENES 1         // number of genes per individual
#define REPEAT 5       // how many times GA will run
#define MUTE 1          // increse / decrease in mutated gene value
#define GENERATION 100  // number of generations that will be created
#define RANGE 100       // range for x
#define UPPER_BOUND 30  // upper bound for f(x) - necessary for PlotRange

#define temporary "./data/temp.txt"

#define fittest_roulette "./data/fit_roulette3.nb"
#define avarage_roulette "./data/av_roulette3.nb"
#define both_roulette "./data/both_roulette3.nb"
#define genes_roulette "./data/genes_roulette3.txt"

#define fittest_bestmates "./data/fit_best3.nb"
#define avarage_bestmates "./data/av_best3.nb"
#define both_bestmates "./data/both_best3.nb"
#define genes_bestmates "./data/genes_best3.txt"

#define fittest_tournament "./data/fit_tour3.nb"
#define avarage_tournament "./data/av_tour3.nb"
#define both_tournament "./data/both_tour3.nb"
#define genes_tournament "./data/genes_tour3.txt"



//typedef int TipoGene;

typedef struct{

    float c[GENES];
    float fit;

} ind;

void initialize_population(ind *pop);


void fittness_function (ind *pop);

void order (ind *pop);              // pop[0] == fittest

void order_whole_pop (ind *pop);    // decrescent fitness order 


void choose_parents_roulette (ind *pop, int *p1, int *p2);

void choose_parents_best_mates (int *p2);

void choose_parents_tournament (ind *pop, int *p1, int *p2);


void mutate(float *gene);

ind cross_over (ind *pop, int p1, int p2);

float Average_fitness(ind population[]);