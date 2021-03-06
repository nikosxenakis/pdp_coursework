#include "actor.h"

Actor::Actor(int id, int type, int workers_num) {
	assert(id >= 0 && workers_num >=1);
	this->id = id;
	this->type = type;
	this->workers_num = workers_num;
	this->state = 0;
}

Actor::~Actor() {}

int Actor::get_id() {
	return this->id;
}

void Actor::set_state(int state) {
	this->state = state;
}

void Actor::compute() {
	if(this->compute_map[this->state])
		this->compute_map[this->state](this);
}

void Actor::process(Message message) {
	if(this->parse_message_map[this->state])
		this->parse_message_map[this->state](this, message);
}

void Actor::send_msg(int actor_id, Message message) {
	if(message.get(COMMAND) <= 4) {
		cerr << "Invalid message error: COMMAND code should be greater that 4\n";
		exit(1);
	}
	message.set(ACTOR_ID, this->get_id());
	message.set(ACTOR_ID_DEST, actor_id);
	Messenger::send_message(this->get_worker(actor_id), message);
}

int Actor::get_worker(int actor_id) {
	return actor_id%this->workers_num + 1;
}

void Actor::register_state(int state, void (compute_f)(Actor*)) {
	this->compute_map[state] = compute_f;
}

void Actor::register_state(int state, void (parse_message_f)(Actor*, Message)) {
	this->parse_message_map[state] = parse_message_f;
}

void Actor::create_actor(Message message) {
	message.set(COMMAND, SPAWN_ACTOR_COMMAND);
	message.set(ACTOR_TYPE, this->type);
	Messenger::send_message(MASTER_PID, message);
}

void Actor::kill_actor() {
	int actor_id = this->get_id();
	Message message;
	message.set(COMMAND, KILL_ACTOR_COMMAND);
	message.set(ACTOR_ID, actor_id);
	Messenger::send_message(this->get_worker(actor_id), message);
	Messenger::send_message(MASTER_PID, message);
}

void Actor::kill_all() {
	Message message;
	message.set(COMMAND, KILL_ALL_ACTORS_COMMAND);
	Messenger::send_message(MASTER_PID, message);
}
