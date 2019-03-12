#include "master.h"

int Master::pid = 0;
int Master::workers_num = 0;
int Master::max_actors_num = 0;
int Master::active_actors = 0;
int Master::next_actor_id = 0;
int Master::dead_workers = 0;

vector<Worker*> Master::workers;
Actor* (*Master::create_actor)(int actor_id, int actor_type, int master_pid, int worker_pid, int workers_num, void *data);

void *Master::input_data = nullptr;

void Master::register_create_actor(Actor* (create_actor)(int actor_id, int actor_type, int master_pid, int worker_pid, int workers_num, void* data), void *data) {
	Master::create_actor = create_actor;
	Master::input_data = data;
}

void Master::init_workers() {
    for (int i = 0; i < Master::workers_num; ++i) {
    	Worker *worker = new Worker(startWorkerProcess(), Master::pid, 0, Master::workers_num);
		Master::workers.push_back(worker);
		cout << "Master started worker on MPI process " << worker->get_pid() << endl;
    }
}

void Master::initialize_master(int pid, int workers_num, int max_actors_num) {
	Master::pid = pid;
	Master::workers_num = workers_num;
	Master::max_actors_num = max_actors_num;
	Master::init_workers();
}

int Master::get_next_worker() {
	return Master::next_actor_id%workers_num + 1;
}

void Master::spawn_actor(Message message) {

	// Message message;
	// message.message_data.command = SPAWN_ACTOR_COMMAND;

	// Messenger::send_blocking_message(this->master_pid, message);
	// message = Messenger::receive_message(this->master_pid);

	// return message.message_data.actor_id%this->workers_num + 1;


	// Worker *worker = Master::find_available_worker();

	message.message_data.command = SPAWN_ACTOR_COMMAND;
	message.message_data.actor_id = Master::next_actor_id;		
	message.message_data.worker_pid = Master::get_next_worker();

	Input_data* tmp = (Input_data*)Master::input_data;
	tmp->x = message.message_data.x;		
	tmp->y = message.message_data.y;		

	// cout << "MASTER spawn actor: id = " << Master::next_actor_id << " worker = " << message.message_data.worker_pid << " type = " << message.message_data.actor_type << " x = " << tmp->x << " y = " << tmp->y << endl;
	

	// Actor *actor = Master::create_actor(message.message_data.actor_id, message.message_data.actor_type, Master::pid, message.message_data.worker_pid, worker->workers_num, tmp);
	// worker->add_actor(actor);
	

	Master::active_actors++;
	Master::next_actor_id++;


	Messenger::send_message(message.message_data.worker_pid, message);

	// message.message_data.command = DISCOVER_ACTOR_COMMAND;

  //   for (auto worker : Master::workers) {
		// Messenger::send_message(worker->get_pid(), message);
  //   }
}

void Master::kill_actor(int actor_id) {
	Actor *actor = nullptr;
    for (auto worker : Master::workers) {
    	actor = worker->find_actor(actor_id);
		if(actor != nullptr) {
			worker->remove_actor(actor->get_id());
	    	Message message;
    		message.message_data.command = KILL_ACTOR_COMMAND;
    		message.message_data.actor_id = actor_id;			
			Messenger::send_message(worker->get_pid(), message);
			Master::active_actors--;
			break;
		}
	}
  //   for (auto worker : Master::workers) {
  //   	Message forget_message;
  //   	forget_message.message_data.command = FORGET_ACTOR_COMMAND;
  //   	forget_message.message_data.actor_id = actor_id;
		// Messenger::send_message(worker->get_pid(), forget_message);
  //   }
}

// Worker * Master::find_available_worker() {
// 	Worker *worker_min = nullptr;
// 	int worker_load;

//     for (auto worker : Master::workers) {
//     	if(worker_min != nullptr) {
// 	    	if(worker->get_load() < worker_load) {
// 	    		worker_min = worker;
// 	    		worker_load = worker->get_load();
//     		}
//     	}
//     	else {
//     		worker_min = worker;
//     		worker_load = worker->get_load();
//     	}
// 	}

//     return worker_min;
// }

Actor* Master::find_actor(int id) {
    for (auto worker : Master::workers) {
	    for (auto actor : worker->actors) {
	    	if(actor->get_id() == id)
	    		return actor;
	    }
    }
}

void Master::start_simulation() {
	Message message;
	message.message_data.command = START_WORKER_COMMAND;
    for (auto worker : Master::workers) {
    	Messenger::send_blocking_message(worker->get_pid(), message);
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
		if(Master::compute()) break;
	} while(1);
}

int Master::compute() {
	// cout << Master::active_actors << endl;

	if(Master::active_actors <= 0 || Master::active_actors > Master::max_actors_num) {
		cout << "Master terminates simulation, there are " << Master::active_actors << " active_actors\n";
		return 1;
	}
	return 0;
}

int Master::parse_message(int source_pid, Message message) {
	// cout << "Master received " << message.get_string_command() << " command\n";

	if(message.message_data.command == KILL_ACTOR_COMMAND) {
		// cout << "KILL_ACTOR_COMMAND " << message.actor_id << "\n";
		if(message.message_data.actor_id == 16) {
		    for (int worker_pid = 1; worker_pid <= Master::workers_num; ++worker_pid) {
		    	Message message;
		    	message.message_data.command = KILL_WORKER_COMMAND;
		    	Messenger::send_message(worker_pid, message);
			}
		}
		Master::kill_actor(message.message_data.actor_id);
	}
	else if(message.message_data.command == SPAWN_ACTOR_COMMAND) {
		// cout << "Master: SPAWN_ACTOR_COMMAND\n";
		Master::spawn_actor(message);
	}
	else if(message.message_data.command == KILL_WORKER_COMMAND) {
		cout << "Master: KILL_WORKER_COMMAND\n";
		Master::dead_workers++;
		if(Master::dead_workers == Master::workers_num)
			return 1;
	}

	return 0;
}

void Master::finalize() {
    	
 //    for (int worker_pid = 1; worker_pid <= Master::workers_num; ++worker_pid) {
 //    	Message message;
 //    	message.message_data.command = KILL_WORKER_COMMAND;
 //    	Messenger::send_message(worker_pid, message);
	// }
 //    for (int worker_pid = 1; worker_pid <= Master::workers_num; ++worker_pid) {
 //    	Message message;
 //    	do {
 //    		//discard messages
 //    		message = Messenger::receive_message(worker_pid);
 //    	} while (message.message_data.command != KILL_WORKER_COMMAND);
	// }

	cout << "Master finalize" << endl;
	// free all memory
}

void Master::print() {}
