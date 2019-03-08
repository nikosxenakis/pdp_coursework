#include "actor.h"

Actor::Actor(int id, int master_pid) {
	this->id = id;
	this->master_pid = master_pid;
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
	int command = SPAWN_ACTOR_COMMAND;
	Message message = Message(command, -1, actor_type);
	Messenger::send_message(this->master_pid, message);
}

void Actor::die() {
	Message message = Message(KILL_ACTOR_COMMAND, this->get_id(), this->get_type());
	Messenger::send_message(this->master_pid, message);
}
