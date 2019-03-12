#include "actor.h"

void (*Actor::initialiseRNG)(long *seed);

void Actor::register_initialiseRNG(void (initialiseRNG)(long *seed)) {
	Actor::initialiseRNG = initialiseRNG;
}

Actor::Actor(int id, int master_pid, int worker_pid, int workers_num) {
	this->id = id;
	this->master_pid = master_pid;
	this->worker_pid = worker_pid;
	this->workers_num = workers_num;
	this->type = ACTOR_TYPE_NONE;
	this->state = 0;
	this->seed = -id-1;
	Actor::initialiseRNG(&this->seed);
	// cout << "Seed: " << this->seed << endl;
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

void Actor::set_worker(int worker_pid) {
	this->worker_pid = worker_pid;
}

void Actor::compute() {
	this->compute_map[this->state](this);
}

void Actor::parse_message(Message message) {
	this->parse_message_map[this->state](this, message);
}

void Actor::create_actor(Message message) {
	message.message_data.command = SPAWN_ACTOR_COMMAND;
	message.message_data.actor_type = this->type;
	Messenger::send_message(this->master_pid, message);
}

void Actor::die() {
	Message message;
	message.message_data.command = KILL_ACTOR_COMMAND;
	message.message_data.actor_id = this->get_id();
	Messenger::send_message(this->worker_pid, message);
}

void Actor::kill_actor(Actor *actor) {
	cout << "Actor::kill_actor\n";
	exit(1);
	Message message;
	message.message_data.command = KILL_ACTOR_COMMAND;
	message.message_data.actor_id = actor->get_id();
	message.message_data.actor_type = actor->get_type();
	Messenger::send_message(actor->master_pid, message);
}

void Actor::send_msg(int actor_id, Message message) {
	message.message_data.actor_id = this->get_id();
	message.message_data.actor_id_dest = actor_id;
	Messenger::send_message(this->get_worker(actor_id), message);
}

void Actor::set_state(int state) {
	this->state = state;
}

void Actor::register_state(int type, int state, void (f)(Actor*)) {
	if(type == COMPUTE)
		this->compute_map[state] = f;
}

void Actor::register_state(int type, int state, void (f)(Actor*, Message)) {
	if(type == PARSE_MESSAGE)
		this->parse_message_map[state] = f;
}

int Actor::get_worker(int actor_id) {
	return actor_id%this->workers_num + 1;
}
