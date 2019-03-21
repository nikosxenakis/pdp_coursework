#include "worker.h"

Actor* (*Worker::spawn_actor)(Message message);

Message Worker::input_data;

void Worker::register_spawn_actor(Actor* (spawn_actor)(Message message), Message input_data) {
	Worker::spawn_actor = spawn_actor;
	Worker::input_data = input_data;
}

Worker::Worker(int pid, int master_pid, int init_actors_num, int workers_num) {
	this->pid = pid;
	this->master_pid = master_pid;
	this->init_actors_num = init_actors_num;
	this->workers_num = workers_num;
	this->actors = vector<Actor*>();
	this->start_simulation = 0;
	this->actors_spawned = 0;
	this->actors_died = 0;
}

int Worker::get_pid() {
	return this->pid;
}

void Worker::print() {
	cout << "Worker " << this->pid << " with load = " << this->actors.size() << " its actors are: ";
	for (auto actor : this->actors)
		cout << actor->get_id() << " ";
	cout << endl;
}

void Worker::run() {
	int outstanding, source_pid;
	MPI_Status status;

	do {
		MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &outstanding, &status);
		if(!outstanding) {
			this->compute();
		}
		else {
			source_pid=status.MPI_SOURCE;
			Message message = Messenger::receive_message(source_pid);			
			if(this->process(message)) break;
		}
	} while(1);
}

int Worker::process(Message message) {
	int ret_val = 0;

	// cout << "Worker " << this->get_pid() << ": received " << message.get(COMMAND) << " command\n";

	if(message.get(COMMAND) == START_WORKER_COMMAND) {
		// while(this->get_total_actors_num() < this->init_actors_num);
		this->start_simulation = 1;
		cout << "Started: ";
		this->print();
	}
	else if(message.get(COMMAND) == KILL_WORKER_COMMAND) {
    	Messenger::send_message(this->master_pid, message);
    	ret_val = 1;
	}
	else if(message.get(COMMAND) == SPAWN_ACTOR_COMMAND) {
		// cout << "Worker SPAWN_ACTOR_COMMAND " << message.message_data.actor_type << "\n";
		Actor *actor = Worker::spawn_actor(message);
		this->add_actor(actor);
	}
	else if(message.get(COMMAND) == KILL_ACTOR_COMMAND) {
		// cout << "KILL_ACTOR_COMMAND\n";
		this->remove_actor(message.get(ACTOR_ID));
	}
	else {
		Actor *actor = this->find_actor(message.get(ACTOR_ID_DEST));
		if(actor)
			actor->process(message);
	}
	return ret_val;
}

void Worker::compute() {
	if(this->start_simulation == 0)
		return;

	for (auto actor : this->actors) {
		actor->compute();
	}
}

void Worker::finalize() {
	cout << "Worker " << this->pid << " finalized: actors_spawned = " << this->actors_spawned << ", actors_died = " << this->actors_died << endl;
}

void Worker::add_actor(Actor *actor) {
	this->actors_spawned++;
	this->actors.push_back(actor);
}

void Worker::remove_actor(int actor_id) {
	this->actors_died++;
	for (int i = 0; i < this->actors.size(); ++i) {
		if(this->actors[i]->get_id() == actor_id) {
			delete this->actors[i];
			this->actors[i] = nullptr;
			this->actors.erase(this->actors.begin() + i);
			return;
		}
	}
	assert(0);
}

Actor* Worker::find_actor(int actor_id) {
    for (auto actor : this->actors)
    	if(actor->get_id() == actor_id)
    		return actor;
    return nullptr;
}

void Worker::kill_all_actors() {
	for (auto actor : this->actors) {
		Message message;
		message.set(COMMAND, KILL_ACTOR_COMMAND);
		message.set(ACTOR_ID, actor->get_id());
		Messenger::send_message(master_pid, message);
	}
}

Worker::~Worker() {

}
