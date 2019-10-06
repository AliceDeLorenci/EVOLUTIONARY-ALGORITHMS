# DATA ANALYSIS

### VERSIONS 1, 2, 3, 4

		#define POP_SIZE 10     // population size
		#define GENES 1         // number of genes per individual
		#define REPEAT          // how many times GA will run
		#define GENERATION 100  // number of generations that will be created
		#define RANGE 100       // range for x
		#define UPPER_BOUND 30  // upper bound for f(x) - necessary for PlotRange
		#define HISTORIC 5      // over how many generations fittest individuals will be analised to determine deviation

		// mutation configuration
		#define VARIABLE 1		// 0 => static mutation --- 1 => variable mutation
		#define FULLCYCLE  		// 0 => conditioinal (breakable) variable mutation cycle --- 1 => full mutation cycle

		#define MUTE 1.0        // increse / decrease in mutated gene value

		#define STAGE1 0.1*MUTE
		#define STAGE2 10*MUTE
		#define STAGE3 30*MUTE
		#define STAGE4 50*MUTE
		#define INTERVAL 5      // how long a mutation stage lasts

Versions 1 and 2 are respectively with full cycle and breakable cycle, for data and statistics purposes, version 3 and 4, respectively with full cycle and breakable cycle, have the same macros, for graphing purposes.
