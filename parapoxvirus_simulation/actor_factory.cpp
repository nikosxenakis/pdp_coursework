#include "actor_factory.h"

Actor* Actor_factory::create(int id, int type) {
	Actor *actor = nullptr;

	if(type == ACTOR_TYPE_SQUIRREL)
		actor = new Squirrel(id);

	return actor;
}

Actor* Actor_factory::create(int type) {
	Actor *actor = nullptr;

	if(type == ACTOR_TYPE_SQUIRREL)
		actor = new Squirrel(-1);

	return actor;
}
