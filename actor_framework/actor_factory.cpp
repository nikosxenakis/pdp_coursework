#include "actor_factory.h"

Actor* Actor_factory::create(int id, int type, int master_pid, int worker_pid) {
	Actor *actor = nullptr;

	if(type == ACTOR_TYPE_CLOCK)
		actor = new Clock(id, master_pid, worker_pid);
	if(type == ACTOR_TYPE_SQUIRREL)
		actor = new Squirrel(id, master_pid, worker_pid);
	if(type == ACTOR_TYPE_CELL)
		actor = new Cell(id, master_pid, worker_pid);

	if(actor == nullptr || actor->get_type() == ACTOR_TYPE_NONE){
		cout << "Error in Actor_factory for type = " << type << "\n";
		exit(1);
	}

	return actor;
}
