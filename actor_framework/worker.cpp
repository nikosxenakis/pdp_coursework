#include "worker.h"

Actor* (*Worker::spawn_actor)(Message message);

void Worker::add_actor(Actor *actor) {
	assert(actor != nullptr);
	this->actors_spawned++;
	this->actors.push_back(actor);
}

void Worker::remove_actor(int actor_id) {
	this->actors_died++;
	for (unsigned int i = 0; i < this->actors.size(); ++i) {
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
		Messenger::send_message(MASTER_PID, message);
	}
}

void Worker::compute() {
	if(this->start_simulation == false)
		return;

	for (auto actor : this->actors) {
		actor->compute();
	}
}


int Worker::process(Message message) {
	switch((int) message.get(COMMAND)) {
		case START_WORKER_COMMAND: 
			this->start_simulation = true;
			cout << "Worker " << this->pid << " started: actors_num = " << this->actors.size() << endl;
			break;
		case KILL_WORKER_COMMAND: 
    		Messenger::send_message(MASTER_PID, message);
    		return 1;
		case SPAWN_ACTOR_COMMAND: 
			this->add_actor(Worker::spawn_actor(message));
			break;
		case KILL_ACTOR_COMMAND: 
			this->remove_actor(message.get(ACTOR_ID));
			break;
		default:
			Actor *actor = this->find_actor(message.get(ACTOR_ID_DEST));
			if(actor)
				actor->process(message);
	}
	return 0;
}

Worker::Worker(int pid) {
	this->pid = pid;
	this->actors = vector<Actor*>();
	this->start_simulation = false;
	this->actors_spawned = 0;
	this->actors_died = 0;
}

Worker::~Worker() {
	for(auto actor : this->actors)
		delete actor;
}

void Worker::register_spawn_actor(Actor* (spawn_actor)(Message message)) {
	Worker::spawn_actor = spawn_actor;
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

void Worker::finalize() {
	cout << "Worker " << this->pid << " finalized: actors_spawned = " << this->actors_spawned << ", actors_died = " << this->actors_died << endl;
}
