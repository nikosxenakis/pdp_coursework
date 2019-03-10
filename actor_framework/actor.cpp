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

void Actor::kill_actor(Actor *actor) {
	Message message = Message(KILL_ACTOR_COMMAND, actor->get_id(), actor->get_type());
	Messenger::send_message(actor->master_pid, message);
}

void Actor::kill_all_actors(){
   	for( const auto& worker : this->known_actors )
    {
        for( const auto& actor : worker.second )
        {
			this->kill_actor(actor);
        }
    }
}

Actor* Actor::get_actor(int actor_id) {
    for( const auto& worker : this->known_actors )
    {
        for( const auto& actor : worker.second )
        {
			if(actor->get_id() == actor_id) {
				return actor;
			}  
        }
    }
	return nullptr;
}

vector<Actor*> Actor::get_actors_by_type(int actor_type) {
	vector<Actor*> v;
    for( const auto& worker : this->known_actors ) {
        for( const auto& actor : worker.second ) {
			if(actor->get_type() == actor_type) {
				v.push_back(actor);
			}  
        }
    }
	return v;
}

void Actor::discover_actor(int worker_pid, Actor *actor) {
	// cout << "Actor " << this->get_id() << ": discoved Actor " << actor->get_id() << " in worker " << worker_pid << endl;
	this->known_actors[worker_pid].push_back(actor);
}

void Actor::forget_actor(int actor_id) {
    for( auto& worker : this->known_actors ) {
		for (int i = 0; i < worker.second.size(); ++i)
		{
			if(worker.second[i]->get_id() == actor_id){
				// cout << "Actor " << this->get_id() << " forgot Actor " << actor_id << "\n";
				worker.second.erase(worker.second.begin() + i);
			}
		}
	}
}

int Actor::find_worker(int actor_id) {
    for( const auto& worker : this->known_actors )
    {
        for( const auto& actor : worker.second )
        {
			if(actor->get_id() == actor_id)
				return worker.first;
        }
    }
    return -1;
}

void Actor::send_msg(int actor_id, Message message) {
	int worker_pid = this->find_worker(actor_id);
	if(worker_pid != -1) {
		message.actor_id_dest = actor_id;
		Messenger::send_message(worker_pid, message);
	}
	else {
		// cout << "Error from Actor " << this->get_id() << " in ";
		message.print();
	}
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
