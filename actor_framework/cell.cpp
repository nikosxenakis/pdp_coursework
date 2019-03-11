#include "cell.h"

#define INIT 0
#define SIMULATE 1
#define WAIT 2
#define FINISH 3

static void compute_dummy(Actor *actor) {}
static void parse_message_dummy(Actor *actor, Message message) {}

static void compute_init(Actor *actor) {
	Cell *cell = dynamic_cast<Cell*>(actor);
	cell->set_state(WAIT);
	cell->clock = cell->get_actors_by_type(ACTOR_TYPE_CLOCK)[0];

	Message message;
	message.message_data.command = TIMESTEP_START;
	message.message_data.actor_id = cell->get_id();
	message.message_data.actor_type = cell->get_type();

	cell->send_msg(cell->clock->get_id(), message);
}

static void compute_simulate(Actor *actor) {
	Cell *cell = dynamic_cast<Cell*>(actor);
	cell->set_state(WAIT);

	// calculate
	if(cell->timestep > 3) {
		cell->population_influx -= cell->population_in_steps[cell->timestep-3];
	}
	
	if(cell->timestep > 2) {
		cell->infection_level -= cell->inflection_in_steps[cell->timestep-2];
	}

	Message message;
	message.message_data.command = TIMESTEP_END;
	message.message_data.actor_id = cell->get_id();
	message.message_data.actor_type = cell->get_type();

	cell->send_msg(cell->clock->get_id(), message);

}

static void parse_message_simulate(Actor *actor, Message message) {
	Cell *cell = dynamic_cast<Cell*>(actor);
	if(message.message_data.command == VISIT_ACTOR_COMMAND) {
		cell->visited(message);
	}

}

static void parse_message_wait(Actor *actor, Message message) {
	Cell *cell = dynamic_cast<Cell*>(actor);
	// receive message to increase timestep
	if(message.message_data.command == TIMESTEP_START) {
		cell->set_state(SIMULATE);
		// cell->timestep++;
	}
	else if(message.message_data.command == VISIT_ACTOR_COMMAND) {
		// cout << "VISIT_ACTOR_COMMAND\n";
		cell->visited(message);
	}
	else {
		cout << "Error in parse_message_wait\n";
	}
	// receive message to finish
}

Cell::Cell(int id, int master_pid, int worker_pid, int cell_number, int max_months): Actor(id, master_pid, worker_pid) {
	this->type = ACTOR_TYPE_CELL;
	this->cell_number = id;
	this->max_months = max_months;
	this->timestep = 1;
	this->population_influx = 0;
	this->infection_level = 0;
	this->population_in_steps = vector<int>(this->max_months);
	this->inflection_in_steps = vector<int>(this->max_months);

	for (int i = 1; i <= this->max_months; ++i) {
		this->population_in_steps[i] = 0;
	}

	for (int i = 1; i <= this->max_months; ++i) {
		this->inflection_in_steps[i] = 0;
	}

	if(this->cell_number != this->get_id()) {
		cout << "Wrong cell number " << cell_number << " and id " << id << "\n";
		exit(1);
	}

	this->set_state(INIT);

	this->register_state(COMPUTE, INIT, compute_init);
	this->register_state(COMPUTE, SIMULATE, compute_simulate);
	this->register_state(COMPUTE, WAIT, compute_dummy);
	this->register_state(COMPUTE, FINISH, compute_dummy);

	this->register_state(PARSE_MESSAGE, INIT, parse_message_dummy);
	this->register_state(PARSE_MESSAGE, SIMULATE, parse_message_simulate);
	this->register_state(PARSE_MESSAGE, WAIT, parse_message_wait);
	this->register_state(PARSE_MESSAGE, FINISH, parse_message_dummy);
}

void


 Cell::visited(Message message) {
 	this->population_in_steps[timestep]++;
 	this->population_influx++;
 	if(message.message_data.healthy == 0) {
 		this->inflection_in_steps[timestep]++;
 		this->infection_level++;
 	}
	// cout << "Visit from Actor " << message.message_data.actor_id << " with health status " << message.message_data.healthy << " to Cell " << message.message_data.actor_id_dest << endl;
}
