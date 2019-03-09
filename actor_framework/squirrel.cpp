#include "squirrel.h"

#define INIT 0
#define SIMULATE 1
#define WAIT 2
#define FINISH 3

	// squirrel->create_actor(ACTOR_TYPE_SQUIRREL);
	// squirrel->set_state(SQUIRREL_STATE_COMPUTE);

	// squirrel->visit(1);
	// squirrel->create_actor(ACTOR_TYPE_SQUIRREL);
	// ping
	// int actor_id = 1;
	// Message ping_message = Message(PING_ACTOR_COMMAND, this->get_id(), actor_id);
	// this->send_msg(actor_id, ping_message);

	// squirrel->die();
	// squirrel->visit(2);

static void squirrel_compute_func_init(Actor *actor) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
}

static void squirrel_compute_func_compute(Actor *actor) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
}

static void squirrel_compute_func_die(Actor *actor) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);

}

static void squirrel_compute_func_finish(Actor *actor) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
}

static void squirrel_parse_message_func_init(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	if(message.command == VISIT_ACTOR_COMMAND) {
		// cout << "VISIT_ACTOR_COMMAND\n";
		squirrel->visited(message.actor_id);
	}
	else if(message.command == PING_ACTOR_COMMAND) {
		// cout << "PING_ACTOR_COMMAND\n";
		// squirrel->visited(message.actor_id);
	}
	else if(message.command == TIMESTEP_START) {
		squirrel->set_state(SIMULATE);
	}
}
static void squirrel_parse_message_func_compute(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);

	if(message.command == VISIT_ACTOR_COMMAND) {
		// cout << "VISIT_ACTOR_COMMAND\n";
		squirrel->visited(message.actor_id);
	}
	else if(message.command == PING_ACTOR_COMMAND) {
		// cout << "PING_ACTOR_COMMAND\n";
		// squirrel->visited(message.actor_id);
	}
	else if(message.command == TIMESTEP_END) {
		squirrel->set_state(WAIT);
	}
}
static void squirrel_parse_message_func_die(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);

	if(message.command == VISIT_ACTOR_COMMAND) {
		// cout << "VISIT_ACTOR_COMMAND\n";
		squirrel->visited(message.actor_id);
	}
	else if(message.command == PING_ACTOR_COMMAND) {
		// cout << "PING_ACTOR_COMMAND\n";
		// squirrel->visited(message.actor_id);
	}
	else if(message.command == TIMESTEP_START) {
		squirrel->set_state(SIMULATE);
	}
}
static void squirrel_parse_message_func_finish(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);

	if(message.command == VISIT_ACTOR_COMMAND) {
		// cout << "VISIT_ACTOR_COMMAND\n";
		squirrel->visited(message.actor_id);
	}
	else if(message.command == PING_ACTOR_COMMAND) {
		// cout << "PING_ACTOR_COMMAND\n";
		// squirrel->visited(message.actor_id);
	}
}

Squirrel::Squirrel(int id, int master_pid, int worker_pid): Actor(id, master_pid, worker_pid) {
	this->type = ACTOR_TYPE_SQUIRREL;

	this->register_state(COMPUTE, INIT, squirrel_compute_func_init);
	this->register_state(COMPUTE, SIMULATE, squirrel_compute_func_compute);
	this->register_state(COMPUTE, WAIT, squirrel_compute_func_die);
	this->register_state(COMPUTE, FINISH, squirrel_compute_func_finish);

	this->register_state(PARSE_MESSAGE, INIT, squirrel_parse_message_func_init);
	this->register_state(PARSE_MESSAGE, SIMULATE, squirrel_parse_message_func_compute);
	this->register_state(PARSE_MESSAGE, WAIT, squirrel_parse_message_func_die);
	this->register_state(PARSE_MESSAGE, FINISH, squirrel_parse_message_func_finish);

	this->set_state(INIT);
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
