#include "actor_factory.h"

Actor* Actor_factory::create(int id, int type) {
	Actor *actor = nullptr;

	if(type == ACTOR_TYPE_SQUIRREL)
		Actor *actor = new Squirrel(id);

	return actor;
}
