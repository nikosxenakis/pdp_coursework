#include "worker.h"

Worker::Worker(int pid, int master_pid) {
	this->pid = pid;
	this->master_pid = master_pid;
	this->actors = vector<Actor*>();
	this->known_actors = map<int, vector<Actor*>>();
}

int Worker::get_load() {
	return this->actors.size();
}

int Worker::get_pid() {
	return this->pid;
}

void Worker::print() {
	cout << "Worker " << this->pid << " with load = " << this->actors.size() << endl;
}

void Worker::start_simulation() {
	int command = -1;
	Message message;

	do {
		message = Messenger::receive_message(this->master_pid);
	} while(message.command != START_WORKER_COMMAND);
}

void Worker::run() {

	this->start_simulation();

	int outstanding, source_pid, exit_command = 0;
	MPI_Status status;

	do
	{
		MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &outstanding, &status);
		if(outstanding) {
			source_pid=status.MPI_SOURCE;
			Message message = Messenger::receive_message(source_pid);
			exit_command = this->parse_message(message);
		}
		this->compute();
	} while(!exit_command);
}

int Worker::parse_message(Message message) {
	int command = 0;
	int ret_val = 0;

	if(message.command == KILL_WORKER_COMMAND) {
    	Messenger::send_blocking_message(this->master_pid, Message(KILL_WORKER_COMMAND));
    	ret_val = 1;
	}
	else if(message.command == SPAWN_ACTOR_COMMAND) {
		// cout << "SPAWN_ACTOR_COMMAND\n";
		Actor *actor = Actor_factory::create(message.actor_id, message.actor_type, this->master_pid, this->get_pid());
		this->add_actor(actor);
	}
	else if(message.command == KILL_ACTOR_COMMAND) {
		// cout << "KILL_ACTOR_COMMAND\n";
		this->remove_actor(message.actor_id);
	}
	else if(message.command == DISCOVER_ACTOR_COMMAND) {
		// cout << "DISCOVER_ACTOR_COMMAND\n";
		Actor *actor = Actor_factory::create(message.actor_id, message.actor_type, this->master_pid, this->get_pid());
		this->discover_actor(message.worker_pid, actor);

		for (auto tmp_actor : this->actors) {
			tmp_actor->discover_actor(message.worker_pid, actor);
		}
	}
	else {
		Actor *actor = this->find_actor(message.actor_id_dest);
		if(actor)
			actor->parse_message(message);
	}

	return ret_val;
}

void Worker::compute() {

	for (auto actor : this->actors) {
		actor->compute();
	}

}

void Worker::finalize() {
	cout << "Worker finalize" << endl;
}

void Worker::add_actor(Actor *actor) {
	this->actors.push_back(actor);
}

void Worker::remove_actor(int actor_id) {
	for (int i = 0; i < this->actors.size(); ++i)
	{
		if(this->actors[i]->get_id() == actor_id)
			this->actors.erase(this->actors.begin() + i);
	}
}

void Worker::discover_actor(int worker_pid, Actor *actor) {
	this->known_actors[worker_pid].push_back(actor);
}

Actor* Worker::find_actor(int actor_id) {
    for (auto actor : this->actors) {
    	if(actor->get_id() == actor_id)
    		return actor;
    }
    return nullptr;
}

void Worker::kill_all_actors() {
	for (auto actor : this->actors) {
		Message message = Message(KILL_ACTOR_COMMAND, actor->get_id(), actor->get_type());
		Messenger::send_message(master_pid, message);
	}
}

Worker::~Worker() {

}
