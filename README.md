# SimulationOfDiscreteEvents
Repository for the discrete events simulation code examples given in the book 'Simulation Modeling and Analysis, Averill M. Law, fifth edition, 2014.

- To compile, execute and see the output of the basic queuing model code, go to the uppermost folder of the repo and type on your terminal: "cd basic_queue_model ; gcc -c -I../rand_gen main.c ; ./a.out ; cat mm1.out"

My ~/.bashrc commands to compile, execute and see the output of the models:

```
alias comprunrandgen="cd rand_gen ; gcc -c lcgrand.o ; cd .."
alias comprunmm1model="comprunrandgen ; cd basic_queue_model ; gcc -c mm1.c ; gcc ../rand_gen/lcgrand.o mm1.o -lm -o mm1 ; ./mm1 ; cat mm1.out ; cd .."
alias compruninvmodel="comprunrandgen ; cd inventory_model ; gcc .c inv.c ; gcc ../rand_gen/lcgrand.o inv.o -lm -o inv ; ./inv ; cat inv.out ; cd .."
```
