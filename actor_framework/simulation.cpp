/*
 * Example code to run and test the process pool. To compile use something like mpicc -o test test.c pool.c
 */
#include "simulation.h"

static int cell_number = 0;

Actor* spawn_actor(int id, int type, int master_pid, int worker_pid, int workers_num, void* v_data) {
	Actor *actor = nullptr;
	Input_data *data = (Input_data *) v_data;

	// cout << id << " " << type << " " << master_pid << " " << worker_pid << " \n";
	// data->print();

	if(type == ACTOR_TYPE_CELL){
		actor = new Cell(id, master_pid, worker_pid, workers_num, cell_number, data->max_months);
		cell_number++;
	}
	else if(type == ACTOR_TYPE_CLOCK){
		actor = new Clock(id, master_pid, worker_pid, workers_num, data->max_months);
	}
	else if(type == ACTOR_TYPE_SQUIRREL){
		actor = new Squirrel(id, master_pid, worker_pid, workers_num, data->x, data->y, data->healthy);
	}

	if(actor == nullptr || actor->get_type() == ACTOR_TYPE_NONE){
		cout << "Error in Actor_factory for type = " << type << "\n";
		exit(1);
	}

	return actor;
}

void init_actors(void *v_input_data) {
	Input_data *input_data = (Input_data *)v_input_data;
	Message message;

	for (int i = 0; i < input_data->cells; ++i){
		message.message_data.actor_type = ACTOR_TYPE_CELL;
		Actor_framework::spawn_actor(message);
	}
	for (int i = 0; i < input_data->clocks; ++i){
		message.message_data.actor_type = ACTOR_TYPE_CLOCK;
		Actor_framework::spawn_actor(message);
	}
	for (int i = 0; i < input_data->squirells; ++i){
		message.message_data.healthy = 1;
		message.message_data.x = 0;
		message.message_data.y = 0;
		if(input_data->infection_level > 0 ) {
			message.message_data.healthy = 0;
			input_data->infection_level--;
		}
		message.message_data.actor_type = ACTOR_TYPE_SQUIRREL;
		Actor_framework::spawn_actor(message);
	}
}

int main(int argc, char* argv[]) {

	Input_data *input_data = new Input_data(argc, argv);

	Actor_framework::register_init_actors(init_actors);

	Actor_framework::register_create_actor(spawn_actor, input_data);

	Actor_framework::actor_framework(input_data, input_data->max_actors_num, input_data->init_actors_num);

	return 0;
}
