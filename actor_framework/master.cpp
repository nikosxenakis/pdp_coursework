#include "master.h"

int Master::pid = 0;
int Master::workers_num = 0;
int Master::next_actor_id = 0;
int Master::dead_workers = 0;

vector<int> Master::workers_pid;
Actor* (*Master::create_actor)(int actor_id, int actor_type, int master_pid, int worker_pid, int workers_num, void *data);

void *Master::input_data = nullptr;

void Master::register_create_actor(Actor* (create_actor)(int actor_id, int actor_type, int master_pid, int worker_pid, int workers_num, void* data), void *data) {
	Master::create_actor = create_actor;
	Master::input_data = data;
}

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
	message.message_data.command = SPAWN_ACTOR_COMMAND;
	message.message_data.actor_id = Master::next_actor_id;		
	message.message_data.worker_pid = Master::get_next_worker();
	Messenger::send_message(message.message_data.worker_pid, message);

	// cout << "MASTER spawn actor: id = " << Master::next_actor_id << " worker = " << message.message_data.worker_pid << " type = " << \
	message.message_data.actor_type << " x = " << message.message_data.x << " y = " << message.message_data.y << " healthy = " << message.message_data.healthy << endl;
	
	Master::next_actor_id++;
}

void Master::start_simulation() {
	Message message;
	message.message_data.command = START_WORKER_COMMAND;
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

	if(message.message_data.command == KILL_ALL_ACTORS_COMMAND) {
		// cout << "KILL_ALL_ACTORS_COMMAND " << "\n";
		return 1;
	}
	else if(message.message_data.command == SPAWN_ACTOR_COMMAND) {
		// cout << "Master: SPAWN_ACTOR_COMMAND\n";
		Master::spawn_actor(message);
	}

	return 0;
}

void Master::finalize() {
    	
    for (auto worker_pid : Master::workers_pid) {
		Message message;
		message.message_data.command = KILL_WORKER_COMMAND;
		Messenger::send_message(worker_pid, message);
	}	
    for (auto worker_pid : Master::workers_pid) {
     	Message message;
     	do {
     		//discard messages
     		message = Messenger::receive_message(worker_pid);
     	} while (message.message_data.command != KILL_WORKER_COMMAND);
	}	
	cout << "Master finalize" << endl;
	// free all memory
}

void Master::print() {}
