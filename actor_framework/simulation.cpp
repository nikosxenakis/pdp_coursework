/*
 * Example code to run and test the process pool. To compile use something like mpicc -o test test.c pool.c
 */
#include "simulation.h"

void init_actors(void *v_input_data) {
	Input_data *input_data = (Input_data *)v_input_data;
	Message message;
	message.message_data.x = 0.5;
	message.message_data.y = 0.5;
	message.message_data.healthy = 1;

	if(input_data->infection_level > 0 ) {
		message.message_data.healthy = 0;
		input_data->infection_level--;
	}

	for (int i = 0; i < input_data->cells; ++i){
		message.message_data.actor_type = ACTOR_TYPE_CELL;
		Actor_framework::spawn_actor(message);
	}
	for (int i = 0; i < input_data->clocks; ++i){
		message.message_data.actor_type = ACTOR_TYPE_CLOCK;
		Actor_framework::spawn_actor(message);
	}
	for (int i = 0; i < input_data->squirells; ++i){
		message.message_data.actor_type = ACTOR_TYPE_SQUIRREL;
		Actor_framework::spawn_actor(message);
	}
}

int main(int argc, char* argv[]) {

	Input_data *input_data = new Input_data(argc, argv);

	// Actor_data actor_data = {
	// 	24
	// };

	Actor_framework::register_initialiseRNG(initialiseRNG);

	Actor_framework::register_init_actors(init_actors);

	Actor_framework::register_create_actor(Actor_factory::create, input_data);

	Actor_framework::actor_framework(input_data, input_data->max_actors_num, input_data->init_actors_num);

	return 0;
}
