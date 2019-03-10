/*
 * Example code to run and test the process pool. To compile use something like mpicc -o test test.c pool.c
 */
#include "simulation.h"

// void init_actors(Input_data *input_data);

// void (*init_actors)(Input_Data *);

// static void init_actors(Input_data *input_data) {
// 	for (int i = 0; i < input_data->clocks; ++i)
// 		Actor_framework::spawn_actor(ACTOR_TYPE_CLOCK);
// 	for (int i = 0; i < input_data->squirells; ++i)
// 		Actor_framework::spawn_actor(ACTOR_TYPE_SQUIRREL);
// 	for (int i = 0; i < input_data->cells; ++i)
// 		Actor_framework::spawn_actor(ACTOR_TYPE_CELL);
// }

int main(int argc, char* argv[]) {

	// void (*ptr)(Input_data *input_data) = &init_actors; 

	// Actor_framework::register_init_actors(ptr);

	Actor_framework::actor_framework(argc, argv);

	return 0;
}
