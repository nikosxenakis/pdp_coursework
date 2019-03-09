#include "actor.h"

Actor::Actor(int id, int master_pid, int worker_pid) {
	this->id = id;
	this->master_pid = master_pid;
	this->worker_pid = worker_pid;
	this->type = ACTOR_TYPE_NONE;
	this->state = 0;
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

void Actor::compute() {
	this->compute_map[this->state](this);
}

void Actor::parse_message(Message message) {
	this->parse_message_map[this->state](this, message);
}

void Actor::create_actor(int actor_type) {
	int command = SPAWN_ACTOR_COMMAND;
	Message message = Message(command, actor_type);
	Messenger::send_message(this->master_pid, message);
}

void Actor::die() {
	Message message = Message(KILL_ACTOR_COMMAND, this->get_id(), this->get_type());
	Messenger::send_message(this->master_pid, message);
}

Actor* Actor::get_actor(int actor_id) {
	for (map<int, vector<Actor*>>::iterator it = (this->known_actors).begin(); it != (this->known_actors.end()); ++it) {
		for (auto actor : it->second) {
			if(actor->get_id() == actor_id)
				return actor;
		}
	}
	return nullptr;
}

void Actor::discover_actor(int worker_pid, Actor *actor) {
	this->known_actors[worker_pid].push_back(actor);
}

int Actor::find_worker(int actor_id) {
  	for (map<int, vector<Actor*>>::iterator it = (this->known_actors).begin(); it != (this->known_actors.end()); ++it) {
		for (auto actor : it->second) {
			if(actor->get_id() == actor_id)
				return it->first;
		}
  	}
}

void Actor::send_msg(int actor_id, Message message) {
	int worker_pid = this->find_worker(actor_id);
	Messenger::send_message(worker_pid, message);
}

void Actor::set_state(int state) {
	this->state = state;
}
