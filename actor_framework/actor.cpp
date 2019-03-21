#include "actor.h"

Actor::Actor(int id, int type, int master_pid, int worker_pid, int workers_num) {
	this->id = id;
	this->type = type;
	this->master_pid = master_pid;
	this->worker_pid = worker_pid;
	this->workers_num = workers_num;
	this->state = 0;
}

Actor::~Actor() {
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
	if(this->compute_map[this->state])
		this->compute_map[this->state](this);
}

void Actor::process(Message message) {
	if(this->parse_message_map[this->state])
		this->parse_message_map[this->state](this, message);
}

void Actor::create_actor(Message message) {
	message.set(COMMAND, SPAWN_ACTOR_COMMAND);
	message.set(WORKERS_NUM, workers_num);
	message.set(ACTOR_TYPE, this->type);
	Messenger::send_message(this->master_pid, message);
}

void Actor::kill_actor() {
	// cout << "kill_actor send to " << this->worker_pid << endl;
	Message message;
	message.set(COMMAND, KILL_ACTOR_COMMAND);
	message.set(ACTOR_ID, this->get_id());
	Messenger::send_message(this->worker_pid, message);
	Messenger::send_message(this->master_pid, message);
}

void Actor::kill_all() {
	Message message;
	message.set(COMMAND, KILL_ALL_ACTORS_COMMAND);
	Messenger::send_message(this->master_pid, message);
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

void Actor::set_state(int state) {
	this->state = state;
}

void Actor::register_state(int state, void (f)(Actor*)) {
	this->compute_map[state] = f;
}

void Actor::register_state(int state, void (f)(Actor*, Message)) {
	this->parse_message_map[state] = f;
}

int Actor::get_worker(int actor_id) {
	return actor_id%this->workers_num + 1;
}
