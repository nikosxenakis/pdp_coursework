#include "worker.h"

Worker::Worker(int pid, int master_pid) {
	this->pid = pid;
	this->master_pid = master_pid;
	this->actors = vector<Actor*>();
	this->known_actors = vector<Actor*>();
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

void Worker::run() {

	int outstanding, source_pid, exit_command = 0;
	MPI_Status status;

	do
	{
		MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &outstanding, &status);
		if(outstanding) {
			source_pid=status.MPI_SOURCE;
			cout << "HERE";
			Message message = Messenger::receive_message(source_pid);
			exit_command = this->parse_message(message);
		}
		else {
			// this->print();

			// for (auto actor : this->actors) {
			// 	Message message = Message(KILL_ACTOR_COMMAND, actor);
			// 	message.print();

			// 	Messenger::send_message(master_pid, message);
			// 	this->remove_actor(actor);
			// }
		}
	} while(!exit_command);


}

int Worker::parse_message(Message message) {
	int command = 0;
	int ret_val = 0;
	message.print();
	if(message.command == KILL_WORKER_COMMAND) {
		int blocking = 1;
    	Messenger::send_message(master_pid, Message(KILL_WORKER_COMMAND), blocking);
    	ret_val = 1;
	}
	else if(message.command == SPAWN_ACTOR_COMMAND) {
		Actor *actor = Actor_factory::create(message.actor_id, message.actor_type);
		actor->print();
		this->add_actor(actor);
	}

	return ret_val;
}

void Worker::finalize() {
	cout << "Worker finalize" << endl;
}

void Worker::add_actor(Actor *actor) {
	this->actors.push_back(actor);
}

void Worker::remove_actor(Actor *actor) {
	for (int i = 0; i < this->actors.size(); ++i)
	{
		if(this->actors[i] == actor)
			this->actors.erase(this->actors.begin() + i);
	}
}

Actor* Worker::find_actor(int actor_id) {
    for (auto actor : this->actors) {
    	if(actor->get_id() == actor_id)
    		return actor;
    }
    return nullptr;
}

Worker::~Worker() {

}

void Worker::check_messages() {

}

void Worker::process_message() {

}

void Worker::compute() {

}
