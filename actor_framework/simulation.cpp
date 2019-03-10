/*
 * Example code to run and test the process pool. To compile use something like mpicc -o test test.c pool.c
 */
#include "simulation.h"

void init_actors(void *v_input_data) {
	Input_data *input_data = (Input_data *)v_input_data;

	for (int i = 0; i < input_data->clocks; ++i)
		Actor_framework::spawn_actor(ACTOR_TYPE_CLOCK);
	for (int i = 0; i < input_data->squirells; ++i)
		Actor_framework::spawn_actor(ACTOR_TYPE_SQUIRREL);
	for (int i = 0; i < input_data->cells; ++i)
		Actor_framework::spawn_actor(ACTOR_TYPE_CELL);
}

int main(int argc, char* argv[]) {

	Input_data *input_data = new Input_data(argc, argv);



	Actor_framework::register_init_actors(init_actors);

	// Actor_data actor_data = {
	// 	24
	// };

	Actor_framework::register_create_actor(Actor_factory::create, input_data);

	Actor_framework::actor_framework(input_data, input_data->max_actors_num, input_data->init_actors_num);

	return 0;
}
