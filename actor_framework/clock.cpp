#include "clock.h"

#define INIT 0
#define SIMULATE 1
#define WAIT 2
#define FINISH 3

static void compute_init(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);

	clock->squirrels = clock->get_actors_by_type(ACTOR_TYPE_SQUIRREL);
	cout << "Clock found squirrels: ";
    for(auto squirrel : clock->squirrels) {
    	cout << squirrel->get_id() << " ";
    }
    cout << endl;
    clock->set_state(SIMULATE);
}

static void compute_simulate(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	// TIMESTEP_END
    for(auto squirrel : clock->squirrels) {
    	// cout << "SEND to " << squirrel->get_id() << "\n";
		clock->send_msg(squirrel->get_id(), Message(TIMESTEP_START));	
    }

    clock->set_state(WAIT);
}

static void compute_wait(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);

    // clock->set_state(SIMULATE);
}

static void compute_finish(Actor *actor) {
}

static void parse_message_init(Actor *actor, Message message) {
}

static void parse_message_simulate(Actor *actor, Message message) {
}

static void parse_message_wait(Actor *actor, Message message) {
}

static void parse_message_finish(Actor *actor, Message message) {
}

Clock::Clock(int id, int master_pid, int worker_pid): Actor(id, master_pid, worker_pid) {
	this->type = ACTOR_TYPE_CLOCK;

	this->register_state(COMPUTE, INIT, compute_init);
	this->register_state(COMPUTE, SIMULATE, compute_simulate);
	this->register_state(COMPUTE, WAIT, compute_wait);
	this->register_state(COMPUTE, FINISH, compute_finish);

	this->register_state(PARSE_MESSAGE, INIT, parse_message_init);
	this->register_state(PARSE_MESSAGE, SIMULATE, parse_message_simulate);
	this->register_state(PARSE_MESSAGE, WAIT, parse_message_wait);
	this->register_state(PARSE_MESSAGE, FINISH, parse_message_finish);

	this->set_state(INIT);
}
