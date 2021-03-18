link_exec_inv: compile_inv
	@gcc -I../rand_gen ../rand_gen/lcgrand.o inv.o -lm -o inv
	@./inv
compile_inv:
	@gcc -c inv.c
