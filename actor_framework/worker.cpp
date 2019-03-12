#include "worker.h"

Actor* (*Worker::create_actor_f)(int actor_id, int actor_type, int master_pid, int worker_pid, int workers_num, void *data);

void *Worker::input_data = nullptr;

void Worker::register_create_actor(Actor* (create_actor_f)(int actor_id, int actor_type, int master_pid, int worker_pid, int workers_num, void* data), void *data) {
	Worker::create_actor_f = create_actor_f;
	Worker::input_data = data;
}

Worker::Worker(int pid, int master_pid, int init_actors_num, int workers_num) {
	this->pid = pid;
	this->master_pid = master_pid;
	this->init_actors_num = init_actors_num;
	this->workers_num = workers_num;
	this->actors = vector<Actor*>();
	this->start_simulation = 0;
}

int Worker::get_load() {
	return this->actors.size();
}

int Worker::get_pid() {
	return this->pid;
}

void Worker::print() {
	cout << "Worker " << this->pid << " with load = " << this->actors.size() << " its actors are: ";

	for (auto actor : this->actors) {
		cout << actor->get_id() << " ";
	}
	cout << endl;
	
}

void Worker::run() {
	int outstanding, source_pid;
	MPI_Status status;

	do
	{
		MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &outstanding, &status);
		if(outstanding) {
			source_pid=status.MPI_SOURCE;
			Message message = Messenger::receive_message(source_pid);
			if(this->parse_message(message)) break;
		}
		this->compute();
	} while(1);
}

int Worker::parse_message(Message message) {
	int ret_val = 0;

	// cout << "Worker " << this->get_pid() << ": received " << message.get_string_command() << " command\n";

	if(message.message_data.command == START_WORKER_COMMAND) {
		// while(this->get_total_actors_num() < this->init_actors_num);
		this->start_simulation = 1;
		// cout << "Worker " << this->get_pid() << " start_simulation\n";
		// cout << "Worker " << this->get_pid() << " knows Actors ";
		cout << "Worker " << this->get_pid() << " start_simulation with actors: ";
    	for( const auto& actor : this->actors ) {
			cout << actor->get_id() << " ";
    	}
    	cout << endl;
	}
	else if(message.message_data.command == KILL_WORKER_COMMAND) {
		cout << "FINITO\n";
		Message message;
		message.message_data.command = KILL_WORKER_COMMAND;
    	Messenger::send_blocking_message(this->master_pid, message);
    	ret_val = 1;
	}
	else if(message.message_data.command == SPAWN_ACTOR_COMMAND) {
		// cout << "Worker SPAWN_ACTOR_COMMAND " << message.message_data.actor_type << "\n";
		// Actor *actor = Actor_factory::create(message.actor_id, message.actor_type, this->master_pid, this->get_pid());

		Input_data* tmp = (Input_data*)Worker::input_data;
		tmp->x = message.message_data.x;
		tmp->y = message.message_data.y;
		tmp->healthy = message.message_data.healthy;

		// cout << "WORKER: " << message.message_data.actor_id << " " << tmp->x << " " << tmp->y << endl;

		Actor *actor = Worker::create_actor_f(message.message_data.actor_id, message.message_data.actor_type, this->master_pid, this->get_pid(), this->workers_num, tmp);

		this->add_actor(actor);
	}
	else if(message.message_data.command == KILL_ACTOR_COMMAND) {
		// cout << "KILL_ACTOR_COMMAND\n";
		this->remove_actor(message.message_data.actor_id);
	}
	else {
		Actor *actor = this->find_actor(message.message_data.actor_id_dest);
		if(actor){
			actor->parse_message(message);
		}
		else {
			// cout << "Actor " << message.message_data.actor_id_dest << " is dead\n";
			// message.print();
		}
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
	cout << "Worker finalize" << endl;
}

void Worker::add_actor(Actor *actor) {
	this->actors.push_back(actor);
}

void Worker::remove_actor(int actor_id) {
	for (int i = 0; i < this->actors.size(); ++i)
	{
		if(this->actors[i]->get_id() == actor_id)
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

void Worker::kill_all_actors() {
	for (auto actor : this->actors) {
		Message message;
		message.message_data.command = KILL_ACTOR_COMMAND;
		message.message_data.actor_id = actor->get_id();
		message.message_data.actor_type = actor->get_type();
		Messenger::send_message(master_pid, message);
	}
}

Worker::~Worker() {

}
