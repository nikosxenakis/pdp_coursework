#include "actor_factory.h"

int Actor_factory::cell_number = 0;

Actor* Actor_factory::create(int id, int type, int master_pid, int worker_pid, void* v_data) {
	Actor *actor = nullptr;
	Input_data *data = (Input_data *) v_data;

	if(type == ACTOR_TYPE_CELL){
		actor = new Cell(id, master_pid, worker_pid, Actor_factory::cell_number);
		Actor_factory::cell_number++;
	}
	if(type == ACTOR_TYPE_CLOCK)
		actor = new Clock(id, master_pid, worker_pid, data->max_months);
	if(type == ACTOR_TYPE_SQUIRREL)
		actor = new Squirrel(id, master_pid, worker_pid, data->x, data->y);

	if(actor == nullptr || actor->get_type() == ACTOR_TYPE_NONE){
		cout << "Error in Actor_factory for type = " << type << "\n";
		exit(1);
	}

	return actor;
}
