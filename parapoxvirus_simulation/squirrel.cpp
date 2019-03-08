#include "squirrel.h"

Squirrel::Squirrel(int id): Actor(id) {
	this->type = ACTOR_TYPE_SQUIRREL;
}

void Squirrel::compute() {

	static int happen = 1;

	if(happen == 1) {
		this->create_actor(ACTOR_TYPE_SQUIRREL);
		happen=0;
	}
}
