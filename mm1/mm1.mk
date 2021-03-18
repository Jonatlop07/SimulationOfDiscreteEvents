link_exec_mm1: compile_mm1
	@gcc -I../rand_gen ../rand_gen/lcgrand.o mm1.o -lm -o mm1
	@./mm1
compile_mm1:
	@gcc -c mm1.c
