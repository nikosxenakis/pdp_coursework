#include "cell.h"

#define SIMULATE 0

static void compute_simulate(Actor *actor) {
	Cell *cell = dynamic_cast<Cell*>(actor);
}

static void parse_message_simulate(Actor *actor, Message message) {
	Cell *cell = dynamic_cast<Cell*>(actor);
	if(message.message_data.command == VISIT_ACTOR_COMMAND) {
		cell->visited(message);
		Message message_new;
		message_new.message_data.actor_id_dest = message.message_data.actor_id;
		message_new.message_data.command = VISIT_ACTOR_COMMAND;
		message_new.message_data.actor_type == ACTOR_TYPE_CELL;
		message_new.message_data.population_influx = cell->population_influx;
		message_new.message_data.infection_level = cell->infection_level;
		
		cell->send_msg(message.message_data.actor_id, message_new);
	}
	else if(message.message_data.command == TIMESTEP_END) {

		// cout << "Cell: " << cell->timestep << " population_influx " << cell->population_influx << " infection_level " << cell->infection_level << endl;

		if(cell->timestep >= 3) {
			cell->population_influx -= cell->population_in_steps[cell->timestep-3];
		}
		if(cell->timestep >= 2) {
			cell->infection_level -= cell->inflection_in_steps[cell->timestep-2];
		}
		cell->timestep++;

		Message message_end;
		message_end.message_data.command = TIMESTEP_END;
		message_end.message_data.actor_type == ACTOR_TYPE_CELL;
		message_end.message_data.population_influx = cell->population_influx;
		message_end.message_data.infection_level = cell->infection_level;
		cell->send_msg(16, message_end);
	}

}

Cell::Cell(int id, int master_pid, int worker_pid, int workers_num, int cell_number, int max_months): Actor(id, master_pid, worker_pid, workers_num) {
	this->type = ACTOR_TYPE_CELL;
	this->max_months = max_months;
	this->timestep = 1;
	this->population_influx = 0;
	this->infection_level = 0;
	this->population_in_steps = vector<int>(this->max_months);
	this->inflection_in_steps = vector<int>(this->max_months);

	for (int i = 0; i <= this->max_months; ++i) {
		this->population_in_steps[i] = 0;
		this->inflection_in_steps[i] = 0;
	}

	this->set_state(SIMULATE);

	this->register_state(COMPUTE, SIMULATE, compute_simulate);
	this->register_state(PARSE_MESSAGE, SIMULATE, parse_message_simulate);
}

void Cell::visited(Message message) {
 	this->population_in_steps[this->timestep] = this->population_in_steps[this->timestep] + 1;
 	this->population_influx++;
 	if(message.message_data.healthy == 0) {
 		this->inflection_in_steps[this->timestep] = this->inflection_in_steps[this->timestep] + 1;
 		this->infection_level++;
 	}
	// cout << "Visit from Actor " << message.message_data.actor_id << " with health status " << message.message_data.healthy << " to Cell " << message.message_data.actor_id_dest << endl;
}
