#include "actor.h"

Actor::Actor(int id, int master_pid, int worker_pid) {
	this->id = id;
	this->master_pid = master_pid;
	this->worker_pid = worker_pid;
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
 //    std::cout << it->first << " => " << it->second << '\n';

	// for (auto actor : this->known_actors) {
	// 	if(actor->get_id() == actor_id)
	// 		return actor;
	// }
	return nullptr;
	// int command = DISCOVER_ACTOR_COMMAND;
	// Message send_message = Message(command, actor_id);
	// Messenger::send_blocking_message(this->worker_pid, send_message);
	// Message recv_message = Messenger::receive_message(this->worker_pid);
	// recv_message.print();
	// this->parse_message(recv_message);
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
