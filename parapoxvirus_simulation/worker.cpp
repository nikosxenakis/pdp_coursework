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
	// while(1);
	int command = 0, outstanding;

	// do
	// {
	// 	MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &outstanding, &status);

	// 	if(outstanding)
	// 	{
	// 		source=status.MPI_SOURCE;

	// 	}
	// 	else {

	// 	}
	// } while(1);

	// MPI_Recv(&command, 1, MPI_INT, this->master_pid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

}

void Worker::add_actor(Actor *actor) {
	this->actors.push_back(actor);
}

Worker::~Worker() {

}

void Worker::check_messages() {

}

void Worker::process_message() {

}

void Worker::compute() {

}
