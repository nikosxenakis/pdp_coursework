#include "cell.h"

#define SIMULATE 0

#define CLOCK_ID 16

static void parse_message_simulate(Actor *actor, Message message) {
	Cell *cell = dynamic_cast<Cell*>(actor);

	if(message.message_data.command == VISIT_ACTOR_COMMAND) {
		Message message_new;
		int squirrel_id = message.message_data.actor_id;

		message_new.message_data.actor_id_dest = squirrel_id;
		message_new.message_data.command = VISIT_ACTOR_COMMAND;
		message_new.message_data.population_influx = cell->population_influx;
		message_new.message_data.infection_level = cell->infection_level;
		
		if (find(cell->alive_squirrels.begin(), cell->alive_squirrels.end(), squirrel_id) == cell->alive_squirrels.end())
			cell->alive_squirrels.push_back(squirrel_id);

		if (message.message_data.healthy == 0 && find(cell->infected_squirrels.begin(), cell->infected_squirrels.end(), squirrel_id) == cell->infected_squirrels.end())
			cell->infected_squirrels.push_back(squirrel_id);

		cell->send_msg(message.message_data.actor_id, message_new);
		cell->visited(message);
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


		// Message message_ping;
		// message_ping.message_data.command = PING_ACTOR;

		// for(auto actor_id : cell->alive_squirrels) {

		// }
		// message_end.message_data.alive_squirrels = cell->alive_squirrels.size();
		// message_end.message_data.infected_squirrels = cell->infected_squirrels.size();

		cell->alive_squirrels.clear();
		cell->infected_squirrels.clear();

		Message message_end;
		message_end.message_data.command = TIMESTEP_END;
		message_end.message_data.population_influx = cell->population_influx;
		message_end.message_data.infection_level = cell->infection_level;

		cell->send_msg(CLOCK_ID, message_end);
	}

}

Cell::Cell(int id, int master_pid, int worker_pid, int workers_num, int cell_number, int max_months): Actor(id, master_pid, worker_pid, workers_num) {
	this->type = ACTOR_TYPE_CELL;
	this->max_months = max_months;
	this->timestep = 1;
	this->population_influx = 0;
	this->infection_level = 0;
	this->population_in_steps = vector<int>();
	this->inflection_in_steps = vector<int>();
	this->alive_squirrels = vector<int>();
	this->infected_squirrels = vector<int>();

	for (int i = 0; i <= this->max_months; ++i) {
		this->population_in_steps.push_back(0);
		this->inflection_in_steps.push_back(0);
	}

	this->set_state(SIMULATE);

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
