# Maximum Value Calculator

The objective of this evolutionary algorithm is to find the maximum value of a one variable function, in this case
the function used was:   
<p align="center">
f(x) = 2 cos(0.39x) + 5 sin(0.5x) + 0.5 cos(0.1x) + 10 sin(0.7x) + 5 sin(x) + 5 sin(0.35x)
</p>

on the interval 0 ≤ x ≤ 100.

<p align="center">
  <img width="460" height="300" src="https://github.com/AliceDeLorenci/EVOLUTIONARY-ALGORITHMS/blob/master/maximum_value_calculator/function.png">
</p>

### Selection Method

This program implements three diferent selection methods:
- roulette wheel selection;
> A fitness proportionate selection in which the fitter an individual is, the greater probability it has of 
> being chosen as a parent.
- tournament selection;
> For each of the two potential parents, N random individuals are chosen. These individuals then "compete" 
> and the fitter shall be one of the parents to the child chromosome. This program implements the tournament of two (N = 2).
- best mates.
> The fittest individual mates with all other individuals in order to generate offspring.   

Regardless of the method implemented, the fittest individual is always clones into the next generation so that its genome doesn't get lost.

The desired method must be chosen in the command line:
- tournament    
```
    $ g++ -Wall -o3 -march=native -D TOURNAMENT main.cpp maxvalue.cpp -o prog -lm
```
- roulette 
```
    $ g++ -Wall -o3 -march=native -D ROULETTE T main.cpp maxvalue.cpp -o prog -lm
```
- best mates 
```
    $ g++ -Wall -o3 -march=native -D BESTMATES main.cpp maxvalue.cpp -o prog -lm
```

### Mutation Rates

The mutation strategy is set by macros, so that the algorithm can be run either with static or dynamic mutation rates.
```
    #define VARIABLE 1		
    // 0 -> static mutation rates
    // 1 -> variable mutation rates
    
    #define FULLCYCLE 1		
    // 0 -> conditioinal (breakable) variable mutation cycle
    // 1 -> full mutation cycle
    
    #define MUTE 1.0 
    // normal mutation value, this value shall be incresed or decreased in mutated gene value

    // mutation rates for the four stages of the variable mutation cycle
    #define STAGE1 0.1*MUTE
    #define STAGE2 10*MUTE
    #define STAGE3 30*MUTE
    #define STAGE4 50*MUTE
    
    #define INTERVAL 5      
    // duration of a mutation stage
```
For the dynamic mutation, the variable rates cycle starts when the algorithm detects that the fittest individual has locked, which means that it isn't varying anymore, so that the algorithm is stuck in a local (or global) maximum.   
In the implemented strategy, first the rates are reduced, so that the the individual is locally improved aiming for accuracy. Then the rates are progressively enlarged, increasing the population's diversity so that other peaks can be found, and possibly the global maximum is reached.

The effect of such mutation cycles can be seen in the following graphic. When the rates reduce the average fitness curve ( here the fitness score is the value of f(x) ) aproaches the fittest individual curve and when the mutation is increased the average fitness curve falls.

<p align="center">
  <img src="https://github.com/AliceDeLorenci/EVOLUTIONARY-ALGORITHMS/blob/master/maximum_value_calculator/data/both_best3.png">
</p>

Still in the dynamic mutation context there are two implemented posibilities, the cycle can either be broken as soon as the fittest individual starts to vary again, or the it runs till the end of stage 4, regardeless of the fittest individual's behavior.

### Results

For the following configuration, 

```
    #define VARIABLE 1		
    
    #define FULLCYCLE 1		
    
    #define MUTE 1.0 

    #define STAGE1 0.1*MUTE
    #define STAGE2 10*MUTE
    #define STAGE3 30*MUTE
    #define STAGE4 50*MUTE
    
    #define INTERVAL 5      
```
the three selection methods had different error rates (percentage of tests, out of 10000, that didn't reach the function's global maximum after 100 generations):
- best mates: 0.04%
- roulette: 6.17%
- tournament: 0.04%

And this were the graphs plotted for specific test cases where the algorithm did find the global maximum.

<p align="center">
  <img src="https://github.com/AliceDeLorenci/EVOLUTIONARY-ALGORITHMS/blob/master/maximum_value_calculator/data/both_best3.png">
</p>
<p align="center"> 
  <img src="https://github.com/AliceDeLorenci/EVOLUTIONARY-ALGORITHMS/blob/master/maximum_value_calculator/data/both_roulette3.png">
</p>
<p align="center">  
  <img src="https://github.com/AliceDeLorenci/EVOLUTIONARY-ALGORITHMS/blob/master/maximum_value_calculator/data/both_tour3.png">
</p>
