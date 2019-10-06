
		#define POP_SIZE 10     // population size
		#define GENES 1         // number of genes per individual
		#define REPEAT 100        // how many times GA will run
		#define GENERATION 100  // number of generations that will be created
		#define RANGE 100       // range for x
		#define UPPER_BOUND 30  // upper bound for f(x) - necessary for PlotRange
		#define LOWER_BOUND -20
		#define HISTORIC 5      // over how many generations fittest individuals will be analised to determine deviation

		// mutation configuration
		#define VARIABLE 1		// 0 => static mutation --- 1 => variable mutation
		#define FULLCYCLE 0		// 0 => conditioinal (breakable) variable mutation cycle --- 1 => full mutation cycle

		#define MUTE 1.0        // increse / decrease in mutated gene value

		#define STAGE1 0.1*MUTE
		#define STAGE2 10*MUTE
		#define STAGE3 30*MUTE
		#define STAGE4 50*MUTE
		#define INTERVAL 5      // how long a mutation stage lasts


#define temporary "./data/temp.txt"

#define fittest_roulette "./data/fit_roulette4.nb"
#define average_roulette "./data/av_roulette4.nb"
#define both_roulette "./data/both_roulette4.nb"
#define genes_roulette "./data/genes_roulette4.md"
#define data_roulette "./data/data_roulette4.txt"

#define fittest_bestmates "./data/fit_best4.nb"
#define average_bestmates "./data/av_best4.nb"
#define both_bestmates "./data/both_best4.nb"
#define genes_bestmates "./data/genes_best4.md"
#define data_bestmates "./data/data_bestmates4.txt"

#define fittest_tournament "./data/fit_tour4.nb"
#define average_tournament "./data/av_tour4.nb"
#define both_tournament "./data/both_tour4.nb"
#define genes_tournament "./data/genes_tour4.md"
#define data_tournament "./data/data_tour4.txt"



//typedef int TipoGene;

typedef struct{

    float c[GENES];
    float fit;

} ind;

typedef struct{     // variables that shall be used to determine mutation value
    float fit[HISTORIC];    //fittest individuals will be analised over <HISTORIC> generations to determine mutation rates
    int gens;       // keeps track of how many generations since beginning
    int ptr;        // keeps track of where next fittness value will be stored
    int counter;
    int variablemute;
} genealogy;


void initialize_population(ind *pop);

void fittness_function (ind *pop);

void order (ind *pop);              // pop[0] == fittest

void order_whole_pop (ind *pop);    // decrescent fitness order 


void choose_parents_roulette (ind *pop, int *p1, int *p2);

void choose_parents_best_mates (int *p2);

void choose_parents_tournament (ind *pop, int *p1, int *p2);


void initialize_genealogy (genealogy* historic);

float mutation_value(genealogy* historic, ind *pop, float *variation);

void mutate(float *gene, int mute);

ind cross_over (ind *pop, int p1, int p2, int mute);

ind random_individual();


float average_fitness(ind population[]);