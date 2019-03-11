#include "cell.h"

#define INIT 0

static void compute_init(Actor *actor) {
	Cell *cell = dynamic_cast<Cell*>(actor);
}

static void parse_message_init(Actor *actor, Message message) {
	Cell *cell = dynamic_cast<Cell*>(actor);

	if(message.command == VISIT_ACTOR_COMMAND) {
		cell->visited(message);
	}
}

Cell::Cell(int id, int master_pid, int worker_pid, int cell_number): Actor(id, master_pid, worker_pid) {
	this->type = ACTOR_TYPE_CELL;
	this->cell_number = id;
	this->healthy = 1;
	this->virus_age = 0;
	this->population_influx = 0;
	this->infection_level = 0;

	if(this->cell_number != this->get_id()) {
		cout << "Wrong cell number " << cell_number << " and id " << id << "\n";
		exit(1);
	}

	this->set_state(INIT);

	this->register_state(COMPUTE, INIT, compute_init);

	this->register_state(PARSE_MESSAGE, INIT, parse_message_init);
}

void Cell::visited(Message message) {
	cout << "Visit from Actor " << message.actor_id << " with health status " << message.healthy << " to Cell " << message.actor_id_dest << endl;
}
