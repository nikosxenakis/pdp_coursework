#include "master.h"

int Master::pid = 0;
int Master::workers_num = 0;
int Master::next_actor_id = 0;
int Master::dead_workers = 0;

vector<int> Master::workers_pid;

void *Master::input_data = nullptr;

void Master::init_workers(vector<int> workers_pid) {
    Master::workers_pid = workers_pid;
}

void Master::initialize_master(int pid, int workers_num) {
	Master::pid = pid;
	Master::workers_num = workers_num;
}

int Master::get_next_worker() {
	return Master::next_actor_id%workers_num + 1;
}

void Master::spawn_actor(Message message) {
	message.set(COMMAND, SPAWN_ACTOR_COMMAND);
	message.set(ACTOR_ID, Master::next_actor_id);
	Messenger::send_message(Master::get_next_worker(), message);
	Master::next_actor_id++;
}

void Master::start_simulation() {
	Message message;
	message.set(COMMAND, START_WORKER_COMMAND);
    for (auto worker_pid : Master::workers_pid) {
    	Messenger::send_message(worker_pid, message);
	}
	cout << "Master start_simulation\n";
}

void Master::run() {

	Master::start_simulation();

	int outstanding, source_pid;
	MPI_Status status;

	do
	{
		MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &outstanding, &status);
		if(outstanding) {
			source_pid=status.MPI_SOURCE;
			Message message = Messenger::receive_message(source_pid);
			if(Master::parse_message(source_pid, message)) break;
		}
	} while(1);
}

int Master::parse_message(int source_pid, Message message) {
	// cout << "Master received " << message.get_string_command() << " command\n";

	if(message.get(COMMAND) == KILL_ALL_ACTORS_COMMAND) {
		// cout << "KILL_ALL_ACTORS_COMMAND " << "\n";
		return 1;
	}
	else if(message.get(COMMAND) == SPAWN_ACTOR_COMMAND) {
		// cout << "Master: SPAWN_ACTOR_COMMAND\n";
		Master::spawn_actor(message);
	}

	return 0;
}

void Master::finalize() {
    	
    for (auto worker_pid : Master::workers_pid) {
		Message message;
		message.set(COMMAND, KILL_WORKER_COMMAND);
		Messenger::send_message(worker_pid, message);
	}	
    for (auto worker_pid : Master::workers_pid) {
     	Message message;
     	do {
     		//discard messages
     		message = Messenger::receive_message(worker_pid);
     	} while (message.get(COMMAND) != KILL_WORKER_COMMAND);
	}	
	cout << "Master finalize" << endl;
	// free all memory
}

void Master::print() {}
