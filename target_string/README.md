# Overview

The __target string searcher__ evolutionary algorithm tries to guess a user set string starting from a randomly generated string.   
It prints on the terminal the best guess per generation, and the other outputs consist on text files that list the fittest individual score per generation, the avarage fitness per generation and the number of generations needed to find the target string.   
An individual's score is based on the number of letters that differ from the target string, therefore, the optimum score is 0.

The terminal output shows nicely the evolution of the program and looks like the following:

  > Generation:  0	Fittest individual: /C#z_e;un9m%f&LgsJf_/E%
  >
  > Generation: 50	Fittest individual: brP@WtiHlk AjT;gvTitZ@S
  >
  > Generation: 100	Fittest individual: bvop=tiHuk AvAlgorith@S
  >
  > Generation: 200	Fittest individual: Evolutio0SrAvAlgorithms
  >
  > Generation: 250	Fittest individual: Evolutio0arA Algorithms
  >
  > Generation: 300	Fittest individual: Evolutio0arA Algorithms
  >
  > Generation: 350	Fittest individual: Evolutio0arA Algorithms
  >
  > Generation: 400	Fittest individual: Evolutio0ary Algorithms
  >
  > Generation: 450	Fittest individual: Evolutio0ary Algorithms
  >
  > Generation: 490	Fittest individual: Evolutio0ary Algorithms
  
 This algorithm was implemented for three different selection methods:
 - roulette wheel selection;
 - tournament selection;
 - elitism ("best mates");   
 
 and uses a static mutation rate, each child chromosome gene has a 10% chance of mutating (in this context mutation is the   insertion of a random character).
 
