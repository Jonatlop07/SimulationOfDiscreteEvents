# SimulationOfDiscreteEvents
Repository for the discrete events simulation code examples given in the book 'Simulation Modeling and Analysis, Averill M. Law, fifth edition, 2014.

My ~/.bashrc commands to compile, execute and see the output of the models:

```
alias randgen="cd rand_gen ; make -f lcgrand.mk ; cd .."
alias mm1="randgen ; cd mm1 ; make -f mm1.mk ; cat mm1.out ; cd .."
alias inv="randgen ; cd inv ; make -f inv.mk ; cat inv.out ; cd .."
```
