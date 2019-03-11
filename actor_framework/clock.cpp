#include "clock.h"

#define INIT 0
#define SIMULATE_CELLS 1
#define WAIT_CELLS 2
#define SIMULATE_SQUIRRELS 3
#define WAIT_SQUIRRELS 4
#define FINISH 5

static void compute_dummy(Actor *actor) {}
static void parse_message_dummy(Actor *actor, Message message) {}

static void compute_init(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	clock->cells = clock->get_actors_by_type(ACTOR_TYPE_CELL);
	clock->squirrels = clock->get_actors_by_type(ACTOR_TYPE_SQUIRREL);

	if(clock->cells_ready == clock->cells.size() && clock->squirrels_ready == clock->squirrels.size()) {
		clock->cells_ready = 0;
		clock->squirrels_ready = 0;
	    clock->set_state(SIMULATE_CELLS);
	}
}

static void compute_simulate_cells(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	clock->set_state(WAIT_CELLS);
	cout << "Clock starts simulation for timestep " << clock->timestep << "\n";
	cout << "\tSquirrels: " << clock->squirrels.size() << " Cells: " << clock->cells.size() << "\n";
	clock->cells = clock->get_actors_by_type(ACTOR_TYPE_CELL);

	Message message;
	message.message_data.command = TIMESTEP_START;
	
    for(auto cell : clock->cells) {
		clock->send_msg(cell->get_id(), message);	
    }
}

static void compute_simulate_wait_cells(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	clock->cells = clock->get_actors_by_type(ACTOR_TYPE_CELL);

	if(clock->cells.size() == clock->cells_ready) {
		clock->cells_ready = 0;
	    clock->set_state(SIMULATE_SQUIRRELS);
	}

}

static void compute_simulate_squirrels(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	clock->set_state(WAIT_SQUIRRELS);
	// cout << "Clock starts simulation for timestep " << clock->timestep << "\n";
	clock->squirrels = clock->get_actors_by_type(ACTOR_TYPE_SQUIRREL);

	Message message;
	message.message_data.command = TIMESTEP_START;
    for(auto squirrel : clock->squirrels) {
		clock->send_msg(squirrel->get_id(), message);	
    }
}

static void compute_simulate_wait_squirrels(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	clock->squirrels = clock->get_actors_by_type(ACTOR_TYPE_SQUIRREL);
	// cout << "clock waits " << clock->births << " " << clock->squirrels_finished_timestep << " " << clock->squirrels.size() << "\n";

	if(clock->squirrels.size() == clock->squirrels_ready + clock->births && clock->births == clock->births_came) {
		clock->births = 0;
		clock->births_came = 0;
		clock->squirrels_ready = 0;
		clock->timestep++;

		if(clock->timestep <= clock->max_months)
			clock->set_state(SIMULATE_CELLS);
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
	Clock *clock = dynamic_cast<Clock*>(actor);
	if(message.message_data.command == TIMESTEP_START && message.message_data.actor_type == ACTOR_TYPE_CELL) {
		clock->cells_ready++;
	}
	else if(message.message_data.command == TIMESTEP_START && message.message_data.actor_type == ACTOR_TYPE_SQUIRREL) {
		clock->squirrels_ready++;
	}
	else {
		cout << "Wrong message in parse_message_init\n";
	}
}

static void parse_message_wait_cells(Actor *actor, Message message) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	if(message.message_data.command == TIMESTEP_END && message.message_data.actor_type == ACTOR_TYPE_CELL) {
		clock->cells_ready++;
	}
	else {
			message.print();
		cout << "Wrong message in parse_message_wait_cells\n";
	}
}

static void parse_message_wait_squirrels(Actor *actor, Message message) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	if(message.message_data.actor_type == ACTOR_TYPE_SQUIRREL) {
		if(message.message_data.command == TIMESTEP_END) {
			clock->squirrels_ready++;
		}	
		else if(message.message_data.command == TIMESTEP_END_GAVE_BIRTH) {
			clock->squirrels_ready++;
			clock->births++;
		}
		else if(message.message_data.command == TIMESTEP_START) {
			clock->births_came++;
		}
		else {
			cout << "Wrong message in parse_message_wait_squirrels\n";
		}
	}
	else {
		cout << "Wrong message in parse_message_wait_squirrels-\n";
	}
}

Clock::Clock(int id, int master_pid, int worker_pid, int max_months): Actor(id, master_pid, worker_pid) {
	this->type = ACTOR_TYPE_CLOCK;
	this->max_months = max_months;
	this->timestep = 1;
	this->squirrels_ready = 0;
	this->births = 0;
	this->births_came = 0;
	this->cells_ready = 0;

	this->set_state(INIT);

	this->register_state(COMPUTE, INIT, compute_init);
	this->register_state(COMPUTE, SIMULATE_CELLS, compute_simulate_cells);
	this->register_state(COMPUTE, WAIT_CELLS, compute_simulate_wait_cells);
	this->register_state(COMPUTE, SIMULATE_SQUIRRELS, compute_simulate_squirrels);
	this->register_state(COMPUTE, WAIT_SQUIRRELS, compute_simulate_wait_squirrels);
	this->register_state(COMPUTE, FINISH, compute_finish);

	this->register_state(PARSE_MESSAGE, INIT, parse_message_init);
	this->register_state(PARSE_MESSAGE, SIMULATE_CELLS, parse_message_dummy);
	this->register_state(PARSE_MESSAGE, WAIT_CELLS, parse_message_wait_cells);
	this->register_state(PARSE_MESSAGE, SIMULATE_SQUIRRELS, parse_message_dummy);
	this->register_state(PARSE_MESSAGE, WAIT_SQUIRRELS, parse_message_wait_squirrels);
	this->register_state(PARSE_MESSAGE, FINISH, parse_message_dummy);
}
