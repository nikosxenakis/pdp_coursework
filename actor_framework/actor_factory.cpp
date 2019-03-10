#include "actor_factory.h"

Actor* Actor_factory::create(int id, int type, int master_pid, int worker_pid, void* v_data) {
	Actor *actor = nullptr;
	Input_data *data = (Input_data *) v_data;

	int x = 0;
	int y = 0;
	static int cell_number = 1;

	if(type == ACTOR_TYPE_CLOCK)
		actor = new Clock(id, master_pid, worker_pid, data->max_months);
	if(type == ACTOR_TYPE_SQUIRREL)
		actor = new Squirrel(id, master_pid, worker_pid, x, y);
	if(type == ACTOR_TYPE_CELL){
		actor = new Cell(id, master_pid, worker_pid, cell_number);
		cell_number++;
	}

	if(actor == nullptr || actor->get_type() == ACTOR_TYPE_NONE){
		cout << "Error in Actor_factory for type = " << type << "\n";
		exit(1);
	}

	return actor;
}
