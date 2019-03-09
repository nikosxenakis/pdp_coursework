#include "squirrel.h"

#define SQUIRREL_STATE_INIT 0
#define SQUIRREL_STATE_COMPUTE 1
#define SQUIRREL_STATE_DIE 2
#define SQUIRREL_STATE_FINISH 3

		// visit
		// int actor_id = 1;
		// Actor *actor = this->get_actor(actor_id);
		// actor->print();
		// this->create_actor(ACTOR_TYPE_SQUIRREL);
		// this->create_actor(ACTOR_TYPE_SQUIRREL);

		// Message visit_message = Message(VISIT_ACTOR_COMMAND, this->get_id(), actor_id);
		// this->send_msg(actor_id, visit_message);

		// state = SQUIRREL_STATE_COMPUTE;


		// ping
		// int actor_id = 1;
		// Message ping_message = Message(PING_ACTOR_COMMAND, this->get_id(), actor_id);
		// this->send_msg(actor_id, ping_message);


static void squirrel_compute_func_init(Actor *squirrel) {
	squirrel->create_actor(ACTOR_TYPE_SQUIRREL);
	squirrel->create_actor(ACTOR_TYPE_SQUIRREL);
	squirrel->set_state(SQUIRREL_STATE_COMPUTE);
}

static void squirrel_compute_func_compute(Actor *squirrel) {
	squirrel->set_state(SQUIRREL_STATE_DIE);
}

static void squirrel_compute_func_die(Actor *squirrel) {
	squirrel->die();
	squirrel->set_state(SQUIRREL_STATE_FINISH);
}

static void squirrel_compute_func_finish(Actor *squirrel) {
}

static void squirrel_parse_message_func_init(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);

	if(message.command == VISIT_ACTOR_COMMAND) {
		cout << "VISIT_ACTOR_COMMAND\n";
		squirrel->visited(message.actor_id);
	}
	else if(message.command == PING_ACTOR_COMMAND) {
		cout << "PING_ACTOR_COMMAND\n";
		// squirrel->visited(message.actor_id);
	}
}
static void squirrel_parse_message_func_compute(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);

	if(message.command == VISIT_ACTOR_COMMAND) {
		cout << "VISIT_ACTOR_COMMAND\n";
		squirrel->visited(message.actor_id);
	}
	else if(message.command == PING_ACTOR_COMMAND) {
		cout << "PING_ACTOR_COMMAND\n";
		// squirrel->visited(message.actor_id);
	}
}
static void squirrel_parse_message_func_die(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);

	if(message.command == VISIT_ACTOR_COMMAND) {
		cout << "VISIT_ACTOR_COMMAND\n";
		squirrel->visited(message.actor_id);
	}
	else if(message.command == PING_ACTOR_COMMAND) {
		cout << "PING_ACTOR_COMMAND\n";
		// squirrel->visited(message.actor_id);
	}
}
static void squirrel_parse_message_func_finish(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);

	if(message.command == VISIT_ACTOR_COMMAND) {
		cout << "VISIT_ACTOR_COMMAND\n";
		squirrel->visited(message.actor_id);
	}
	else if(message.command == PING_ACTOR_COMMAND) {
		cout << "PING_ACTOR_COMMAND\n";
		// squirrel->visited(message.actor_id);
	}
}

Squirrel::Squirrel(int id, int master_pid, int worker_pid): Actor(id, master_pid, worker_pid) {
	this->type = ACTOR_TYPE_SQUIRREL;
	this->set_state(SQUIRREL_STATE_INIT);

	this->compute_map[SQUIRREL_STATE_INIT] = squirrel_compute_func_init;
	this->compute_map[SQUIRREL_STATE_COMPUTE] = squirrel_compute_func_compute;
	this->compute_map[SQUIRREL_STATE_DIE] = squirrel_compute_func_die;
	this->compute_map[SQUIRREL_STATE_FINISH] = squirrel_compute_func_finish;

	this->parse_message_map[SQUIRREL_STATE_INIT] = squirrel_parse_message_func_init;
	this->parse_message_map[SQUIRREL_STATE_COMPUTE] = squirrel_parse_message_func_compute;
	this->parse_message_map[SQUIRREL_STATE_DIE] = squirrel_parse_message_func_die;
	this->parse_message_map[SQUIRREL_STATE_FINISH] = squirrel_parse_message_func_finish;
}

void Squirrel::visited(int actor_id) {
	cout << "Actor " << this->get_id() << " visited by " << actor_id << endl;
}
