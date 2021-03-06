#include "cell.h"

/**
 * @brief parses the types of messages for this cell and do the appropriate actions
 * @param actor Pointer to this Actor
 * @param message Input message
 */
static void parse_message_simulate(Actor *actor, Message message) {
	Cell *cell = dynamic_cast<Cell*>(actor);
	int actor_id;

	if(message.get(COMMAND) == VISIT_CELL_COMMAND) {
		actor_id = message.get(ACTOR_ID);
		cell->visited(message);
	}
	else if(message.get(COMMAND) == TIMESTEP_END) {
		actor_id = CLOCK_ID;
		cell->month++;
		cell->population_influx[cell->month%3] = 0;
		cell->infection_level[cell->month%2] = 0;
	}
	else
		assert(0);

	// sum the population_influx and infection level
	int curr_population_influx = accumulate(cell->population_influx.begin(), cell->population_influx.end(), 0);
	int curr_infection_level = accumulate(cell->infection_level.begin(), cell->infection_level.end(), 0);

	message.set(POPULATION_INFLUX, curr_population_influx);
	message.set(INFECTION_LEVEL, curr_infection_level);
	cell->send_msg(actor_id, message);
}

Cell::Cell(int id, int workers_num): Actor(id, ACTOR_TYPE_CELL, workers_num) {
	this->month = 1;
	this->population_influx = vector<int>(3);
	this->infection_level = vector<int>(2);

	for (int i = 0; i <3; ++i)
		this->population_influx[i] = 0;
	for (int i = 0; i <2; ++i)
		this->infection_level[i] = 0;

	this->set_state(SIMULATE);
	this->register_state(SIMULATE, parse_message_simulate);
}

Cell::~Cell() {}

void Cell::visited(Message message) {
 	this->population_influx[this->month%3]++;
 	if(message.get(HEALTHY) == 0)
 		this->infection_level[this->month%2]++;
}
