#include "master.h"

int Master::workers_num = 0;
int Master::next_actor_id = 0;
int Master::actors_died = 0;

int Master::get_next_worker() {
	return Master::next_actor_id%workers_num + 1;
}

void Master::start_simulation() {
	Message message;
	message.set(COMMAND, START_WORKER_COMMAND);
    for (int worker_pid = 1; worker_pid <= Master::workers_num; ++worker_pid)
    	Messenger::send_message(worker_pid, message);
	cout << "Master start_simulation\n";
}

int Master::parse_message(int source_pid, Message message) {
	switch((int)message.get(COMMAND)) {
		case KILL_ALL_ACTORS_COMMAND: 
			return 1;
		case SPAWN_ACTOR_COMMAND: 
			Master::spawn_actor(message);
			break;
		case KILL_ACTOR_COMMAND: 
			Master::actors_died++;
			break;
		default:
			assert(0);
	}
	return 0;
}

void Master::set_workers_num(int workers_num) {
	Master::workers_num = workers_num;
}

void Master::spawn_actor(Message message) {
	int worker_pid = Master::get_next_worker();
	message.set(COMMAND, SPAWN_ACTOR_COMMAND);
	message.set(ACTOR_ID, Master::next_actor_id);
	message.set(WORKER_PID, worker_pid);
	message.set(WORKERS_NUM, Master::workers_num);
	Messenger::send_message(worker_pid, message);
	Master::next_actor_id++;
}

void Master::run() {
	int outstanding, source_pid;
	MPI_Status status;

	Master::start_simulation();

	do {
		MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &outstanding, &status);
		if(outstanding) {
			source_pid=status.MPI_SOURCE;
			Message message = Messenger::receive_message(source_pid);
			if(Master::parse_message(source_pid, message)) break;
		}
	} while(1);
}

void Master::finalize() {
    	
    for (int worker_pid = 1; worker_pid <= Master::workers_num; ++worker_pid) {
		Message message;
		message.set(COMMAND, KILL_WORKER_COMMAND);
		Messenger::send_message(worker_pid, message);
	}	
    for (int worker_pid = 1; worker_pid <= Master::workers_num; ++worker_pid) {
     	Message message;
     	do {
     		//discard messages and breaks when a KILL_WORKER_COMMAND is received
     		message = Messenger::receive_message(worker_pid);
     	} while (message.get(COMMAND) != KILL_WORKER_COMMAND);
	}
	cout << "Master finalized: actors_spawned = " << Master::next_actor_id << ", actors_died = " << Master::actors_died << endl;
}
