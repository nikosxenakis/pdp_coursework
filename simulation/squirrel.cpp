#include "squirrel.h"

#define LIVE 0
#define INTERACT 1
#define DIED 2

#define DELAY 1000

static void compute_live(Actor *actor) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
		
	if(squirrel->counter % DELAY == 0) {
		squirrel->set_state(INTERACT);
		squirrel->birth();
		squirrel->catch_disease();
		squirrel->die();
		squirrel->move();
	}

	squirrel->counter++;
}

static void compute_died(Actor *actor) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
}

static void parse_message_interact(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	if(message.get(COMMAND) == VISIT_ACTOR_COMMAND) {
		// cout << "Squirrel parse_message_interact: " << squirrel->get_id() << "\n";
		squirrel->set_state(LIVE);
		squirrel->population_influx[squirrel->step_no%50] = message.get(POPULATION_INFLUX);
		squirrel->infection_level[squirrel->step_no%50] = message.get(INFECTION_LEVEL);
		squirrel->step_no++;
		
		if(squirrel->healthy == 0)
			squirrel->infected_steps++;
	}

}

void Squirrel::print() {
	cout << "Squirrel " << this->get_id() << " in cell number " << getCellFromPosition(this->x, this->y) << endl;
}

void Squirrel::move() {

	squirrelStep(this->x, this->y, &this->x, &this->y, &this->seed);
	int cell_num = getCellFromPosition(this->x, this->y);
	assert(cell_num >=0 && cell_num <16);
	// cout << "Squirrel " << this->get_id() << ": wants to visit Cell " << this->get_id() << " in worker " << this->get_worker(this->get_id()) << "\n";
	Message message;
	message.set(COMMAND, VISIT_ACTOR_COMMAND);
	message.set(HEALTHY, this->healthy);
	this->send_msg(cell_num, message);
}

void Squirrel::birth() {
	if(this->step_no%50 == 0) {
		float avg_pop = 0;

		for (int i = 0; i < 50; ++i)
			avg_pop += this->population_influx[i];
		avg_pop /= 50;

		if(willGiveBirth(avg_pop, &this->seed)) {
			// cout << "Squirrel " << this->get_id() << " will give birth\n";
			Message message;
			message.set(COMMAND, SPAWN_SQUIRREL_COMMAND);
			message.set(ACTOR_ID_DEST, CLOCK_ID);
			this->send_msg(CLOCK_ID, message);
			message.set(HEALTHY, 1);
			message.set(X, this->x);
			message.set(Y, this->y);
			message.set(COMMAND, SPAWN_ACTOR_COMMAND);
			this->create_actor(message);
		}
	}

}

void Squirrel::catch_disease() {
	float avg_inf_level = 0;

	for (int i = 0; i < 50; ++i)
		avg_inf_level += this->infection_level[i];
	avg_inf_level /= 50;
	// avg_inf_level = 0;
	// cout << "inf = " << avg_inf_level << endl;
	if(this->healthy == 1){		
		this->healthy = willCatchDisease(avg_inf_level, &this->seed) == 1 ? 0 : 1;
		if(this->healthy == 0) {
			Message message;
			message.set(COMMAND, INFECTED_SQUIRREL_COMMAND);
			this->send_msg(CLOCK_ID, message);
		}
	}
	// cout << "res = " << this->healthy;
}

void Squirrel::die() {
	if(this->infected_steps >= 50 && willDie(&this->seed)) {
		// cout << "Squirrel " << this->get_id() << " will die\n";
		this->set_state(DIED);
		Message message;
		message.set(COMMAND, KILL_SQUIRREL_COMMAND);
		message.set(ACTOR_ID_DEST, CLOCK_ID);
		this->send_msg(CLOCK_ID, message);
		message.set(COMMAND, KILL_ACTOR_COMMAND);
		this->kill_actor();
	}
}

void Squirrel::init(float x, float y, int healthy){
	assert(healthy == 0 || healthy == 1);
	this->set_type(ACTOR_TYPE_SQUIRREL);
	this->x = x;
	this->y = y;
	this->healthy = healthy;
	this->step_no = 0;
	this->infected_steps = 0;
	this->counter = 0;
	this->population_influx = vector<int>();
	this->infection_level = vector<int>();
	this->seed = -this->get_id() - 1;
	initialiseRNG(&this->seed);
	// cout << "Seed: " << this->seed << endl;

	for (int i = 0; i < 50; ++i) {
		this->population_influx.push_back(0);
		this->infection_level.push_back(0);
	}

	// this->print();

	this->set_state(LIVE);

	this->register_state(LIVE, compute_live);
	this->register_state(DIED, compute_died);
	this->register_state(INTERACT, parse_message_interact);
}

Squirrel::Squirrel(int id, int master_pid, int worker_pid, int workers_num, float x, float y, int healthy): Actor(id, master_pid, worker_pid, workers_num) {
	if(healthy != 0)	healthy = 1;
	this->init(x, y, healthy);
}

Squirrel::Squirrel(int id, int master_pid, int worker_pid, int workers_num, float x, float y): Actor(id, master_pid, worker_pid, workers_num) {
	this->init(x, y, 1);
}

Squirrel::~Squirrel() {}

