#include "master.h"

int Master::pid = 0;
int Master::workers_num = 0;
int Master::active_actors = 0;
vector<Worker*> Master::workers;

void Master::init_workers() {
    for (int i = 0; i < Master::workers_num; ++i) {
    	Worker *worker = new Worker(startWorkerProcess(), Master::pid);
		Master::workers.push_back(worker);
		// MPI_Irecv(NULL, 0, MPI_INT, worker->pid, 0, MPI_COMM_WORLD, &worker->workerRequest);
		cout << "Master started worker on MPI process " << worker->get_pid() << endl;
    }
}

void Master::initialize_master(int pid, int workers_num) {
	Master::pid = pid;
	Master::workers_num = workers_num;
	Master::active_actors = 0;
	Master::init_workers();
}

void Master::spawn_actor(Actor *actor) {
	actor->print();

	Worker *worker = Master::find_available_worker();
	worker->add_actor(actor);

	int command = SPAWN_ACTOR_COMMAND;
	Message message = Message(command, actor);
	Messenger::send_message(worker->get_pid(), message);
}

Worker * Master::find_available_worker() {
	Worker *worker = Master::workers[0];
	int worker_load = worker->get_load();

    for (int i = 1; i < Master::workers_num; ++i) {
    	if(worker->get_load() < worker_load) {
	    	worker = Master::workers[i];
	    	worker_load = worker->get_load();
    	}
    }
    return worker;
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
	// check messages
	// while(1);
}

void Master::kill_actor(int id) {}
void Master::finalize() {}
void Master::print() {}
