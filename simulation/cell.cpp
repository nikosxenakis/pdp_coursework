#include "cell.h"

#define SIMULATE 0

#define CLOCK_ID 16

static void parse_message_simulate(Actor *actor, Message message) {
	Cell *cell = dynamic_cast<Cell*>(actor);
	int actor_id;

	if(message.get(COMMAND) == VISIT_CELL_COMMAND) {
		actor_id = message.get(ACTOR_ID);
		cell->visited(message);
	}
	else if(message.get(COMMAND) == TIMESTEP_END) {
		// cout << "Cell: " << cell->timestep << " population_influx " << cell->population_influx << " infection_level " << cell->infection_level << endl;
		actor_id = CLOCK_ID;

		if(cell->timestep >= 3)
			cell->population_influx -= cell->population_in_steps[cell->timestep-3];
		if(cell->timestep >= 2)
			cell->infection_level -= cell->inflection_in_steps[cell->timestep-2];

		cell->timestep++;
	}
	else
		assert(0);

	message.set(POPULATION_INFLUX, cell->population_influx);
	message.set(INFECTION_LEVEL, cell->infection_level);
	cell->send_msg(actor_id, message);
}

Cell::Cell(int id, int worker_pid, int workers_num, int max_months): Actor(id, ACTOR_TYPE_CELL, worker_pid, workers_num) {
	this->max_months = max_months;
	this->timestep = 1;
	this->population_influx = 0;
	this->infection_level = 0;
	this->population_in_steps = vector<int>();
	this->inflection_in_steps = vector<int>();

	for (int i = 0; i <= this->max_months; ++i) {
		this->population_in_steps.push_back(0);
		this->inflection_in_steps.push_back(0);
	}

	this->set_state(SIMULATE);

	this->register_state(SIMULATE, parse_message_simulate);
}

Cell::~Cell() {}

void Cell::visited(Message message) {
 	this->population_in_steps[this->timestep] = this->population_in_steps[this->timestep] + 1;
 	this->population_influx++;
 	if(message.get(HEALTHY) == 0) {
 		this->inflection_in_steps[this->timestep] = this->inflection_in_steps[this->timestep] + 1;
 		this->infection_level++;
 	}
	// cout << "Visit from Actor " << message.message_data.actor_id << " with health status " << message.message_data.healthy << " to Cell " << message.message_data.actor_id_dest << endl;
}
