#include "squirrel.h"

static void compute_live(Actor *actor) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	
	if(squirrel->counter % DELAY == 0) {
		squirrel->set_state(INTERACT);
		squirrel->birth();
		squirrel->die();
		squirrel->move();
		squirrel->catch_disease();
	}
	squirrel->counter++;
}

static void parse_message_interact(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	if(message.get(COMMAND) == VISIT_CELL_COMMAND) {
		// cout << "Squirrel parse_message_interact: " << squirrel->get_id() << "\n";
		squirrel->set_state(LIVE);
		squirrel->population_influx[squirrel->step_no%STEPS_MEMORY] = message.get(POPULATION_INFLUX);
		squirrel->infection_level[squirrel->step_no%STEPS_MEMORY] = message.get(INFECTION_LEVEL);
		squirrel->step_no++;
		
		if(squirrel->healthy == 0)
			squirrel->infected_steps++;
	}
	else
		assert(0);
}

Squirrel::Squirrel(int id, int workers_num, float x, float y, int healthy): Actor(id, ACTOR_TYPE_SQUIRREL, workers_num) {
	assert((healthy == 0 || healthy == 1) && x>=0.0 && x<0.999 && y>=0.0 && y<0.999);

	this->x = x;
	this->y = y;
	this->healthy = healthy;
	this->step_no = 0;
	this->infected_steps = 0;
	this->counter = 0;
	this->population_influx = vector<int>();
	this->infection_level = vector<int>();
	this->seed = -this->get_id() - 1;

	// called once for each actor
	initialiseRNG(&this->seed);

	for (int i = 0; i < STEPS_MEMORY; ++i) {
		this->population_influx.push_back(0);
		this->infection_level.push_back(0);
	}

	this->set_state(LIVE);

	this->register_state(LIVE, compute_live);
	this->register_state(INTERACT, parse_message_interact);
}

Squirrel::~Squirrel() {}

void Squirrel::move() {
	squirrelStep(this->x, this->y, &this->x, &this->y, &this->seed);
	int cell_num = getCellFromPosition(this->x, this->y);
	assert(cell_num >=0 && cell_num <16);
	Message message;
	message.set(COMMAND, VISIT_CELL_COMMAND);
	message.set(HEALTHY, this->healthy);
	this->send_msg(cell_num, message);
}

void Squirrel::birth() {
	if(this->step_no%STEPS_MEMORY == 0) {
		float avg_pop = 0;

		for (int i = 0; i < STEPS_MEMORY; ++i)
			avg_pop += this->population_influx[i];
		avg_pop /= STEPS_MEMORY;

		if(willGiveBirth(avg_pop, &this->seed)) {
			Message message;
			message.set(COMMAND, SPAWN_SQUIRREL_COMMAND);
			this->send_msg(CLOCK_ID, message);
			// new squirrels are born healthy
			message.set(HEALTHY, 1);
			// new squirrels are born in the same position as their parents
			message.set(X, this->x);
			message.set(Y, this->y);
			this->create_actor(message);
		}
	}

}


void Squirrel::catch_disease() {
	float avg_inf_level = 0;

	if(this->healthy == 0)
		return;

	for (int i = 0; i < STEPS_MEMORY; ++i)
		avg_inf_level += this->infection_level[i];
	avg_inf_level /= STEPS_MEMORY;

	this->healthy = !willCatchDisease(avg_inf_level, &this->seed);
	if(this->healthy == 0) {
		Message message;
		message.set(COMMAND, INFECTED_SQUIRREL_COMMAND);
		this->send_msg(CLOCK_ID, message);
	}
}

void Squirrel::die() {
	if(this->infected_steps >= STEPS_MEMORY && willDie(&this->seed)) {
		assert(this->healthy == 0);
		this->set_state(DIED);
		Message message;
		message.set(COMMAND, KILL_SQUIRREL_COMMAND);
		this->send_msg(CLOCK_ID, message);
		this->kill_actor();
	}
}


