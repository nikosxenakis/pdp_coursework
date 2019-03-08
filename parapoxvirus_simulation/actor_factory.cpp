#include "actor_factory.h"

Actor* Actor_factory::create(int id, int type, int master_pid) {
	Actor *actor = nullptr;

	if(type == ACTOR_TYPE_SQUIRREL)
		actor = new Squirrel(id, master_pid);

	return actor;
}

Actor* Actor_factory::create(int type) {
	Actor *actor = nullptr;

	if(type == ACTOR_TYPE_SQUIRREL)
		actor = new Squirrel(-1,-1);

	return actor;
}
