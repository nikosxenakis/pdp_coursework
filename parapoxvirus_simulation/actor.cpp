#include "actor.h"

Actor::Actor(int id) {
	this->id = id;
	this->type = ACTOR_TYPE_NONE;
}

int Actor::get_id() {
	return this->id;
}

void Actor::print() {
	cout << "Actor: id = " << this->id << ", type = " << this->type << endl;
}

int Actor::get_type() {
	return this->type;
}

void Actor::create_actor(int actor_type) {
	// this->worker->create_actor(actor_type);
}
