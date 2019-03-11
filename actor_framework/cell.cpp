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
	cell->send_msg(cell->clock->get_id(), Message(TIMESTEP_START, cell->get_id(), cell->get_type()));
}

static void compute_simulate(Actor *actor) {
	Cell *cell = dynamic_cast<Cell*>(actor);
	cell->set_state(WAIT);
	// calculate

	cell->send_msg(cell->clock->get_id(), Message(TIMESTEP_END, cell->get_id(), cell->get_type()));

}

static void parse_message_simulate(Actor *actor, Message message) {
	Cell *cell = dynamic_cast<Cell*>(actor);
	if(message.command == VISIT_ACTOR_COMMAND) {
		cell->visited(message);
	}

}

static void parse_message_wait(Actor *actor, Message message) {
	Cell *cell = dynamic_cast<Cell*>(actor);
	// receive message to increase timestep
	if(message.command == TIMESTEP_START) {
		cell->set_state(SIMULATE);
		// cell->timestep++;
	}
	else if(message.command == VISIT_ACTOR_COMMAND) {
		// cout << "VISIT_ACTOR_COMMAND\n";
		cell->visited(message);
	}
	else {
		cout << "Error in parse_message_wait\n";
	}
	// receive message to finish
}

Cell::Cell(int id, int master_pid, int worker_pid, int cell_number): Actor(id, master_pid, worker_pid) {
	this->type = ACTOR_TYPE_CELL;
	this->cell_number = id;
	this->timestep = 1;
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
	this->register_state(COMPUTE, SIMULATE, compute_simulate);
	this->register_state(COMPUTE, WAIT, compute_dummy);
	this->register_state(COMPUTE, FINISH, compute_dummy);

	this->register_state(PARSE_MESSAGE, INIT, parse_message_dummy);
	this->register_state(PARSE_MESSAGE, SIMULATE, parse_message_simulate);
	this->register_state(PARSE_MESSAGE, WAIT, parse_message_wait);
	this->register_state(PARSE_MESSAGE, FINISH, parse_message_dummy);
}

void Cell::visited(Message message) {
	cout << "Visit from Actor " << message.actor_id << " with health status " << message.healthy << " to Cell " << message.actor_id_dest << endl;
}
