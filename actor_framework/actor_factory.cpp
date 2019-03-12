#include "actor_factory.h"

int Actor_factory::cell_number = 0;

Actor* Actor_factory::create(int id, int type, int master_pid, int worker_pid, int workers_num, void* v_data) {
	Actor *actor = nullptr;
	Input_data *data = (Input_data *) v_data;

	// cout << id << " " << type << " " << master_pid << " " << worker_pid << " ";
	// data->print();

	if(type == ACTOR_TYPE_CELL){
		actor = new Cell(id, master_pid, worker_pid, workers_num, Actor_factory::cell_number, data->max_months);
		Actor_factory::cell_number++;
	}
	else if(type == ACTOR_TYPE_CLOCK){
		actor = new Clock(id, master_pid, worker_pid, workers_num, data->max_months);
	}
	else if(type == ACTOR_TYPE_SQUIRREL){
		actor = new Squirrel(id, master_pid, worker_pid, workers_num, data->x, data->y, data->healthy);
		cout << actor->get_type() << endl;
	}

	if(actor == nullptr || actor->get_type() == ACTOR_TYPE_NONE){
		cout << "Error in Actor_factory for type = " << type << "\n";
		exit(1);
	}

	return actor;
}
