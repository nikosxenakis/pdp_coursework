#include "squirrel.h"

#define LIVE 0
#define INTERACT 1

const static long delay = 5000;

static void compute_dummy(Actor *actor) {
}
static void parse_message_dummy(Actor *actor, Message message) {}

static void compute_live(Actor *actor) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
		
	if(squirrel->counter % delay == 0) {
		squirrel->set_state(INTERACT);
		squirrel->birth();
		squirrel->catch_disease();
		squirrel->die();
		squirrel->move();
	}

	squirrel->counter++;
}

static void parse_message_interact(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	if(message.message_data.command == VISIT_ACTOR_COMMAND) {
		// cout << "Squirrel parse_message_interact: " << squirrel->get_id() << "\n";
		squirrel->set_state(LIVE);
		squirrel->population_influx[squirrel->step_no%50] = message.message_data.population_influx;
		squirrel->infection_level[squirrel->step_no%50] = message.message_data.infection_level;
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
	// cout << "Squirrel " << this->get_id() << ": wants to visit Cell " << this->get_id() << " in worker " << this->get_worker(this->get_id()) << "\n";
	Message message;
	message.message_data.command = VISIT_ACTOR_COMMAND;
	message.message_data.healthy = this->healthy;
	this->send_msg(cell_num, message);
}

void Squirrel::birth() {
	float avg_pop = 0;

	for (int i = 0; i < 50; ++i)
		avg_pop += this->population_influx[i];
	avg_pop /= 50;

	if(this->step_no%50 == 0 && willGiveBirth(avg_pop, &this->seed)) {
		// cout << "Squirrel " << this->get_id() << " will give birth\n";
		Message message;
		message.message_data.healthy = 1;
		message.message_data.x = this->x;
		message.message_data.y = this->y;
		this->create_actor(message);
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
	}
	// cout << "res = " << this->healthy;
}

void Squirrel::die() {
	if(this->infected_steps >= 50 && willDie(&this->seed))
		this->kill_actor();
}

void Squirrel::init(float x, float y, int healthy){
	assert(healthy == 0 || healthy == 1);
	this->type = ACTOR_TYPE_SQUIRREL;
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

	this->register_state(COMPUTE, LIVE, compute_live);
	this->register_state(COMPUTE, INTERACT, compute_dummy);

	this->register_state(PARSE_MESSAGE, LIVE, parse_message_dummy);
	this->register_state(PARSE_MESSAGE, INTERACT, parse_message_interact);
}

Squirrel::Squirrel(int id, int master_pid, int worker_pid, int workers_num, float x, float y, int healthy): Actor(id, master_pid, worker_pid, workers_num) {
	// if(healthy == -1)	healthy = 1;
	this->init(x, y, healthy);
}

Squirrel::Squirrel(int id, int master_pid, int worker_pid, int workers_num, float x, float y): Actor(id, master_pid, worker_pid, workers_num) {
	this->init(x, y, 1);
}
