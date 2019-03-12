#include "clock.h"

#define IN_MONTH 0
#define END_OF_MONTH 1
#define FINISH 3

static void compute_dummy(Actor *actor) {}
static void parse_message_dummy(Actor *actor, Message message) {}

static void compute_in_month(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	milliseconds curr_time = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
	static long duration = 500;

	if((curr_time - clock->begin_time).count() > duration) {
		clock->begin_time = curr_time;
		cout << "Clock starts simulation for timestep " << clock->timestep << "\n";
		clock->timestep++;

		clock->set_state(END_OF_MONTH);
		Message message;
		message.message_data.command = TIMESTEP_END;
		for (int cell_id = 0; cell_id < 16; ++cell_id) {
			clock->send_msg(cell_id, message);
		}
	}
}

static void compute_end_of_month(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	if(clock->cells_ready == 16) {
    	cout << "simulation output:\n\tpopulation_influx ";
    	for(auto population_influx: clock->population_influx) {
    		cout << population_influx << " ";
    	}
    	cout << endl;
    	cout << "\tinfection_level ";
    	for(auto infection_level: clock->infection_level) {
    		cout << infection_level << " ";
    	}
    	cout << endl;

		clock->cells_ready = 0;
		clock->begin_time = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

		if(clock->timestep > clock->max_months) {
			clock->set_state(FINISH);
		}
		else {
			clock->set_state(IN_MONTH);
		}
	}
}

static void compute_finish(Actor *actor) {
	// end simulation
	Clock *clock = dynamic_cast<Clock*>(actor);
	// clock->die();
}


static void parse_message_end_of_month(Actor *actor, Message message) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	if(message.message_data.command == TIMESTEP_END) {
		clock->population_influx[clock->cells_ready] = message.message_data.population_influx;
		clock->infection_level[clock->cells_ready] = message.message_data.infection_level;
		// cout << "clock: " << clock->timestep << " population_influx " << clock->population_influx[clock->cells_ready] << " infection_level " << clock->infection_level[clock->cells_ready] << endl;
		clock->cells_ready++;
	}
}

	// clock->cells = clock->get_actors_by_type(ACTOR_TYPE_CELL);
	// clock->squirrels = clock->get_actors_by_type(ACTOR_TYPE_SQUIRREL);

	// if(clock->cells_ready == clock->cells.size() && clock->squirrels_ready == clock->squirrels.size()) {
	// 	clock->cells_ready = 0;
	// 	clock->squirrels_ready = 0;
	//     clock->set_state(SIMULATE_CELLS);
	// }
	// cout << "\tSquirrels: " << clock->squirrels.size() << " Cells: " << clock->cells.size() << "\n";
	// clock->cells = clock->get_actors_by_type(ACTOR_TYPE_CELL);

	// Message message;
	// message.message_data.command = TIMESTEP_START;
	
 //    for(auto cell : clock->cells) {
	// 	clock->send_msg(cell->get_id(), message);
 //    }





// static void compute_simulate_cells(Actor *actor) {
// 	Clock *clock = dynamic_cast<Clock*>(actor);
// 	clock->set_state(WAIT_CELLS);
// 	cout << "Clock starts simulation for timestep " << clock->timestep << "\n";
// 	cout << "\tSquirrels: " << clock->squirrels.size() << " Cells: " << clock->cells.size() << "\n";
// 	clock->cells = clock->get_actors_by_type(ACTOR_TYPE_CELL);

// 	Message message;
// 	message.message_data.command = TIMESTEP_START;
	
//     for(auto cell : clock->cells) {
// 		clock->send_msg(cell->get_id(), message);
//     }
// }

// static void compute_simulate_wait_cells(Actor *actor) {
// 	Clock *clock = dynamic_cast<Clock*>(actor);
// 	clock->cells = clock->get_actors_by_type(ACTOR_TYPE_CELL);

// 	if(clock->cells.size() == clock->cells_ready) {
// 		clock->cells_ready = 0;
// 	    clock->set_state(SIMULATE_SQUIRRELS);
// 	}
// }

// static void compute_simulate_squirrels(Actor *actor) {
// 	Clock *clock = dynamic_cast<Clock*>(actor);
// 	clock->set_state(WAIT_SQUIRRELS);
// 	// cout << "Clock starts simulation for timestep " << clock->timestep << "\n";
// 	clock->squirrels = clock->get_actors_by_type(ACTOR_TYPE_SQUIRREL);

// 	Message message;
// 	message.message_data.command = TIMESTEP_START;
//     for(auto squirrel : clock->squirrels) {
// 		clock->send_msg(squirrel->get_id(), message);	
//     }
// }

// static void compute_simulate_wait_squirrels(Actor *actor) {
// 	Clock *clock = dynamic_cast<Clock*>(actor);
// 	clock->squirrels = clock->get_actors_by_type(ACTOR_TYPE_SQUIRREL);
// 	// cout << "clock waits " << clock->births << " " << clock->squirrels_finished_timestep << " " << clock->squirrels.size() << "\n";

// 	if(clock->squirrels.size() == clock->squirrels_ready + clock->births && clock->births == clock->births_came) {
// 		clock->births = 0;
// 		clock->births_came = 0;
// 		clock->squirrels_ready = 0;
// 		clock->timestep++;

// 		if(clock->timestep <= clock->max_months)
// 			clock->set_state(SIMULATE_CELLS);
// 		else
// 			clock->set_state(FINISH);
// 	}

// }



// static void parse_message_init(Actor *actor, Message message) {
	// Clock *clock = dynamic_cast<Clock*>(actor);
	// if(message.message_data.command == TIMESTEP_START && message.message_data.actor_type == ACTOR_TYPE_CELL) {
	// 	clock->cells_ready++;
	// }
	// else if(message.message_data.command == TIMESTEP_START && message.message_data.actor_type == ACTOR_TYPE_SQUIRREL) {
	// 	clock->squirrels_ready++;
	// }
	// else {
	// 	cout << "Wrong message in parse_message_init\n";
	// }
// }

// static void parse_message_wait_cells(Actor *actor, Message message) {
// 	Clock *clock = dynamic_cast<Clock*>(actor);
// 	if(message.message_data.command == TIMESTEP_END && message.message_data.actor_type == ACTOR_TYPE_CELL) {
// 		clock->cells_ready++;
// 	}
// 	else {
// 		message.print();
// 		cout << "Wrong message in parse_message_wait_cells\n";
// 	}
// }

// static void parse_message_wait_squirrels(Actor *actor, Message message) {
// 	Clock *clock = dynamic_cast<Clock*>(actor);
// 	if(message.message_data.actor_type == ACTOR_TYPE_SQUIRREL) {
// 		if(message.message_data.command == TIMESTEP_END) {
// 			clock->squirrels_ready++;
// 		}	
// 		else if(message.message_data.command == TIMESTEP_END_GAVE_BIRTH) {
// 			clock->squirrels_ready++;
// 			clock->births++;
// 		}
// 		else if(message.message_data.command == TIMESTEP_START) {
// 			clock->births_came++;
// 		}
// 		else {
// 			cout << "Wrong message in parse_message_wait_squirrels -> ";
// 			message.print();
// 		}
// 	}
// 	else {
// 		cout << "Wrong message in parse_message_wait_squirrels-\n";
// 	}
// }

Clock::Clock(int id, int master_pid, int worker_pid, int workers_num, int max_months): Actor(id, master_pid, worker_pid, workers_num) {
	this->type = ACTOR_TYPE_CLOCK;
	this->max_months = max_months;
	this->timestep = 1;
	// this->squirrels_ready = 0;
	this->births = 0;
	this->births_came = 0;
	this->cells_ready = 0;
	this->population_influx = vector<int>(16);
	this->infection_level = vector<int>(16);
	this->begin_time = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

	this->set_state(IN_MONTH);

	this->register_state(COMPUTE, IN_MONTH, compute_in_month);
	this->register_state(COMPUTE, END_OF_MONTH, compute_end_of_month);
	this->register_state(COMPUTE, FINISH, compute_finish);

	this->register_state(PARSE_MESSAGE, IN_MONTH, parse_message_dummy);
	this->register_state(PARSE_MESSAGE, END_OF_MONTH, parse_message_end_of_month);
	this->register_state(PARSE_MESSAGE, FINISH, parse_message_dummy);
}
