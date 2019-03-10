/*
 * Example code to run and test the process pool. To compile use something like mpicc -o test test.c pool.c
 */
#include "actor_framework.h"

int main(int argc, char* argv[]) {

	Actor_framework::actor_framework(argc, argv);

	return 0;
}
