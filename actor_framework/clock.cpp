#include "clock.h"

#define INIT 0
#define SIMULATE 1
#define WAIT 2
#define FINISH 3

static void compute_init(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);
    clock->set_state(SIMULATE);
	clock->squirrels = clock->get_actors_by_type(ACTOR_TYPE_SQUIRREL);
	// cout << "Clock found squirrels: ";
 //    for(auto squirrel : clock->squirrels) {
 //    	cout << squirrel->get_id() << " ";
 //    }
 //    cout << endl;
}

static void compute_simulate(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	clock->set_state(WAIT);

	cout << "Clock starts simulation for timestep " << clock->timestep << "\n";

    for(auto squirrel : clock->squirrels) {
    	// cout << "SEND to " << squirrel->get_id() << "\n";
		clock->send_msg(squirrel->get_id(), Message(TIMESTEP_START));	
    }
}

static void compute_wait(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	clock->squirrels = clock->get_actors_by_type(ACTOR_TYPE_SQUIRREL);
	// cout << "clock waits " << clock->births << " " << clock->squirrels_finished_timestep << " " << clock->squirrels.size() << "\n";
	if(clock->squirrels_finished_timestep + clock->births == clock->squirrels.size()) {
		clock->births = 0;
		clock->squirrels_finished_timestep = 0;
		clock->timestep++;
		if(clock->timestep <= clock->max_months)
    		clock->set_state(SIMULATE);
    	else
    		clock->set_state(FINISH);
	}
}

static void compute_finish(Actor *actor) {
	// end simulation
	Clock *clock = dynamic_cast<Clock*>(actor);
	static int exec_finish = 1;
	if(exec_finish) {
		clock->kill_all_actors();
	  //   for(auto actor : clock->known_actors) {
			// clock->kill_actor(actor);
	  //   }
		exec_finish = 0;
	}
}

static void parse_message_init(Actor *actor, Message message) {
}

static void parse_message_simulate(Actor *actor, Message message) {
}

static void parse_message_wait(Actor *actor, Message message) {
	// wait to receive from all squirrels that it is done
	Clock *clock = dynamic_cast<Clock*>(actor);
	clock->squirrels = clock->get_actors_by_type(ACTOR_TYPE_SQUIRREL);

	if(message.command == TIMESTEP_END) {
		clock->squirrels_finished_timestep++;
	}
	else if(message.command == TIMESTEP_END_GAVE_BIRTH) {
		clock->squirrels_finished_timestep++;
		clock->births++;
	}
}

static void parse_message_finish(Actor *actor, Message message) {
}

Clock::Clock(int id, int master_pid, int worker_pid): Actor(id, master_pid, worker_pid) {
	this->type = ACTOR_TYPE_CLOCK;
	this->max_months = 5;
	this->timestep = 1;
	this->squirrels_finished_timestep = 0;
	this->births = 0;

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
