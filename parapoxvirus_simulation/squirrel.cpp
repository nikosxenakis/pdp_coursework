#include "squirrel.h"

Squirrel::Squirrel(int id, int master_pid, int worker_pid): Actor(id, master_pid, worker_pid) {
	this->type = ACTOR_TYPE_SQUIRREL;
}

void Squirrel::compute() {

	static int count = 0;

	if(count < 1 && this->get_id() == 0) {

		int actor_id = 1;
		Actor *actor = this->get_actor(actor_id);
		actor->print();

		Message message = Message(VISIT_ACTOR_COMMAND, this->get_id(), actor_id);
		this->send_msg(actor_id, message);
		
		// this->create_actor(ACTOR_TYPE_SQUIRREL);
		// this->create_actor(ACTOR_TYPE_SQUIRREL);
		// int actor_id = 10;
		// this->send_msg(actor_id);
		// this->die();
		count++;
	}
}

void Squirrel::parse_message(Message message) {	
	if(message.command == VISIT_ACTOR_COMMAND) {
		cout << "VISIT_ACTOR_COMMAND\n";
		this->visited(message.actor_id);
	}
}

void Squirrel::visited(int actor_id) {
	cout << "Actor " << this->get_id() << " visited by " << actor_id << endl;
}

// implement state machine
