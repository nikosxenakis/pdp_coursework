#include "master.h"

int Master::pid = 0;
int Master::workers_num = 0;
int Master::active_actors = 0;
vector<Worker*> Master::workers;

void Master::init_workers() {
    for (int i = 0; i < Master::workers_num; ++i) {
    	Worker *worker = new Worker(startWorkerProcess(), Master::pid);
		Master::workers.push_back(worker);
		cout << "Master started worker on MPI process " << worker->get_pid() << endl;
    }
}

void Master::initialize_master(int pid, int workers_num) {
	Master::pid = pid;
	Master::workers_num = workers_num;
	Master::active_actors = 0;
	Master::init_workers();
}

void Master::spawn_actor(int actor_type) {
	Worker *worker = Master::find_available_worker();

	Actor *actor = Actor_factory::create(Master::active_actors, actor_type, worker->get_pid());

	worker->add_actor(actor);

	int command = SPAWN_ACTOR_COMMAND;
	Message message = Message(command, actor->get_id(), actor->get_type());
	Messenger::send_message(worker->get_pid(), message);
	Master::active_actors++;
}

Worker * Master::find_available_worker() {
	Worker *worker_min = nullptr;
	int worker_load;

    for (auto worker : Master::workers) {
    	if(worker_min != nullptr) {
	    	if(worker->get_load() < worker_load) {
	    		worker_min = worker;
	    		worker_load = worker->get_load();
    		}
    	}
    	else {
    		worker_min = worker;
    		worker_load = worker->get_load();
    	}
	}

    return worker_min;
}

Actor* find_actor(int id) {
    for (auto worker : Master::workers) {
	    for (auto actor : worker->actors) {
	    	if(actor->get_id() == id)
	    		return actor;
	    }
    }
}

void Master::run() {
	int outstanding, source_pid, exit_command = 0;
	MPI_Status status;

	do
	{
		MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &outstanding, &status);
		if(outstanding) {
			source_pid=status.MPI_SOURCE;
			Message message = Messenger::receive_message(source_pid);
			Master::parse_message(message);
		}
		else {
			if(Master::active_actors == 0)
				exit_command = 1;
		}
	} while(!exit_command);
}

void Master::parse_message(Message message) {
	if(message.command == KILL_ACTOR_COMMAND) {
		Master::kill_actor(message.actor_id);
	}
	else if(message.command == SPAWN_ACTOR_COMMAND) {
		Master::spawn_actor(message.actor_type);
	}
}

void Master::finalize() {
    for (auto worker : Master::workers) {
    	Messenger::send_message(worker->get_pid(), Message(KILL_WORKER_COMMAND));
	}
    for (auto worker : Master::workers) {
		Message message = Messenger::receive_message(worker->get_pid());
		if(message.command != KILL_WORKER_COMMAND)
			cout << "ERROR" << endl;
	}

	cout << "Master finalize" << endl;
	// free all memory
}

void Master::kill_actor(int actor_id) {
	Actor *actor = nullptr;
    for (auto worker : Master::workers) {
    	actor = worker->find_actor(actor_id);
		if(actor != nullptr) {
			worker->remove_actor(actor);
			break;
		}
	}
	Master::active_actors--;
}

void Master::print() {}
