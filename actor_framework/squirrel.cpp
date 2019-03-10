#include "squirrel.h"

#define INIT 0
#define SIMULATE 1
#define WAIT 2
#define FINISH 3

	// squirrel->visit(1);
	// squirrel->create_actor(ACTOR_TYPE_SQUIRREL);
	// ping
	// int actor_id = 1;
	// Message ping_message = Message(PING_ACTOR_COMMAND, this->get_id(), actor_id);
	// this->send_msg(actor_id, ping_message);

	// squirrel->die();
	// squirrel->visit(2);

static void compute_init(Actor *actor) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	squirrel->clock = squirrel->get_actors_by_type(ACTOR_TYPE_CLOCK)[0];
	squirrel->set_state(WAIT);
}

static void compute_simulate(Actor *actor) {
	int give_birth = 0;

	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	cout << "Squirrel " << squirrel->get_id() << " computes timestep " << squirrel->timestep << endl;

	// if(squirrel->timestep == 2 && squirrel->get_id() == 1) {
	// 	give_birth = 1;
	// }
	// if(squirrel->timestep == 1 && squirrel->get_id() == 2) {
	// 	squirrel->set_state(FINISH);
	// 	squirrel->die();
	// 	return;
	// }


	squirrel->set_state(WAIT);
	Message message;

	if(give_birth == 1) {
		cout << "Squirrel " << squirrel->get_id() << " gave birth in timestep "<< squirrel->timestep <<"\n";
		squirrel->create_actor(ACTOR_TYPE_SQUIRREL);
		message = Message(TIMESTEP_END_GAVE_BIRTH);
	}
	else {
		message = Message(TIMESTEP_END);
	}

	squirrel->timestep++;
	squirrel->send_msg(squirrel->clock->get_id(), message);
}

static void compute_wait(Actor *actor) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
}

static void compute_finish(Actor *actor) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
}

static void parse_message_init(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	// if(message.command == VISIT_ACTOR_COMMAND) {
	// 	// cout << "VISIT_ACTOR_COMMAND\n";
	// 	squirrel->visited(message.actor_id);
	// }
	// else if(message.command == PING_ACTOR_COMMAND) {
	// 	// cout << "PING_ACTOR_COMMAND\n";
	// 	// squirrel->visited(message.actor_id);
	// }
	
	if(message.command == TIMESTEP_START) {
		squirrel->set_state(SIMULATE);
	}
}
static void parse_message_simulate(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);

	// if(message.command == VISIT_ACTOR_COMMAND) {
	// 	// cout << "VISIT_ACTOR_COMMAND\n";
	// 	squirrel->visited(message.actor_id);
	// }
	// else if(message.command == PING_ACTOR_COMMAND) {
	// 	// cout << "PING_ACTOR_COMMAND\n";
	// 	// squirrel->visited(message.actor_id);
	// }
	
	// if(message.command == TIMESTEP_END) {
	// 	squirrel->set_state(WAIT);
	// }
}
static void parse_message_wait(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);

	// if(message.command == VISIT_ACTOR_COMMAND) {
	// 	// cout << "VISIT_ACTOR_COMMAND\n";
	// 	squirrel->visited(message.actor_id);
	// }
	// else if(message.command == PING_ACTOR_COMMAND) {
	// 	// cout << "PING_ACTOR_COMMAND\n";
	// 	// squirrel->visited(message.actor_id);
	// }
	if(message.command == TIMESTEP_START) {
		squirrel->set_state(SIMULATE);
	}
}
static void parse_message_finish(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	// if(message.command == VISIT_ACTOR_COMMAND) {
	// 	// cout << "VISIT_ACTOR_COMMAND\n";
	// 	squirrel->visited(message.actor_id);
	// }
	// else if(message.command == PING_ACTOR_COMMAND) {
	// 	// cout << "PING_ACTOR_COMMAND\n";
	// 	// squirrel->visited(message.actor_id);
	// }
}

Squirrel::Squirrel(int id, int master_pid, int worker_pid): Actor(id, master_pid, worker_pid) {
	this->type = ACTOR_TYPE_SQUIRREL;
	this->clock = nullptr;
	this->timestep = 1;
	this->x = 0;
	this->y = 0;
	this->healthy = 1;
	this->steps = 0;

	this->set_state(INIT);

	this->register_state(COMPUTE, INIT, compute_init);
	this->register_state(COMPUTE, SIMULATE, compute_simulate);
	this->register_state(COMPUTE, WAIT, compute_wait);
	this->register_state(COMPUTE, FINISH, compute_finish);

	this->register_state(PARSE_MESSAGE, INIT, parse_message_init);
	this->register_state(PARSE_MESSAGE, SIMULATE, parse_message_simulate);
	this->register_state(PARSE_MESSAGE, WAIT, parse_message_wait);
	this->register_state(PARSE_MESSAGE, FINISH, parse_message_finish);
}

void Squirrel::visit(int actor_id) {
	// cout << "Actor " << this->get_id() << ": wants to visit Actor " << actor_id << "\n";

	Actor *dst_actor = this->get_actor(actor_id);
	if(dst_actor) {
		// cout << "Actor " << this->get_id() << ": found Actor " << actor_id << "\n";
		Message visit_message = Message(VISIT_ACTOR_COMMAND, this->get_id(), dst_actor->get_id());
		this->send_msg(dst_actor->get_id(), visit_message);	
	}
	else {
		// cout << "Actor " << this->get_id() << ": Actor " << actor_id << " is not cached" << "\n";
	}
}

void Squirrel::visited(int actor_id) {
	// cout << "Actor " << this->get_id() << ": visited by " << actor_id << endl;
}
