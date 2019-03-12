#include "squirrel.h"

#define LIVE 0
#define INTERACT 1

const static long delay = 20000;

static void compute_dummy(Actor *actor) {}
static void parse_message_dummy(Actor *actor, Message message) {}

static void compute_live(Actor *actor) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	int will_die = 0;
	
	if(squirrel->counter % delay == 0) {
		squirrel->set_state(INTERACT);
		squirrel->move();
		squirrel->birth();
		squirrel->will_die();
	}

	squirrel->counter++;
}

static void compute_interact(Actor *actor) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
}

static void parse_message_interact(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	if(message.message_data.command == VISIT_ACTOR_COMMAND) {

		// cout << "Squirrel parse_message_interact: " << squirrel->get_id() << "\n";

		squirrel->set_state(LIVE);
		squirrel->population_influx[squirrel->step_no] = message.message_data.population_influx;
		squirrel->infection_level[squirrel->step_no] = message.message_data.infection_level;
		squirrel->step_no = (squirrel->step_no+1)%50;
		
		if(squirrel->healthy == 0)
			squirrel->infected_steps++;
	}

}

void Squirrel::print() {
	cout << "Squirrel " << this->get_id() << " in cell number " << getCellFromPosition(this->x, this->y) << endl;
}

void Squirrel::visit(int actor_id) {
	// cout << "Squirrel " << this->get_id() << ": wants to visit Cell " << actor_id << " in worker " << this->get_worker(actor_id) << "\n";
	Message message;
	message.message_data.command = VISIT_ACTOR_COMMAND;
	message.message_data.healthy = this->healthy;
	this->send_msg(actor_id, message);
}

void Squirrel::move() {
	squirrelStep(this->x, this->y, &this->x, &this->y, &this->seed);
	int cell_num = getCellFromPosition(this->x, this->y);
	this->steps[this->step_no] = cell_num;

	this->visit(cell_num);
}

void Squirrel::birth() {
	float avg_pop = 0;

	for (int i = 0; i < 50; ++i)
		avg_pop += this->population_influx[i];
	avg_pop /= 50;

	if(this->step_no%50 == 0 && willGiveBirth(avg_pop, &this->seed)) {
		// cout << "Squirrel " << this->get_id() << " will give birth\n";
		Message message;
		message.message_data.x = this->x;
		message.message_data.y = this->y;
		this->create_actor(message);
	}

}

void Squirrel::will_die() {
	float avg_inf_level = 0;

	for (int i = 0; i < 50; ++i)
		avg_inf_level += this->infection_level[i];
	avg_inf_level /= 50;

	if(this->healthy == 1){
		cout << "catch\n";
		this->healthy = willCatchDisease(avg_inf_level, &this->seed);
	}

	if(this->infected_steps >= 50 && willDie(&this->seed)) {
		this->die();
	}
}

void Squirrel::init(float x, float y, int healthy){
	this->type = ACTOR_TYPE_SQUIRREL;
	this->x = x;
	this->y = y;
	this->healthy = healthy;
	this->step_no = 0;
	this->infected_steps = 0;
	this->steps = vector<int>();
	this->counter = 0;
	this->population_influx = vector<int>();
	this->infection_level = vector<int>();

	for (int i = 0; i < 50; ++i) {
		this->steps.push_back(0);
		this->population_influx.push_back(0);
		this->infection_level.push_back(0);
	}

	// this->print();

	this->set_state(LIVE);

	this->register_state(COMPUTE, LIVE, compute_live);
	this->register_state(COMPUTE, INTERACT, compute_interact);

	this->register_state(PARSE_MESSAGE, LIVE, parse_message_dummy);
	this->register_state(PARSE_MESSAGE, INTERACT, parse_message_interact);
}

Squirrel::Squirrel(int id, int master_pid, int worker_pid, int workers_num, float x, float y, int healthy): Actor(id, master_pid, worker_pid, workers_num) {
	this->init(x, y, healthy);
}

Squirrel::Squirrel(int id, int master_pid, int worker_pid, int workers_num, float x, float y): Actor(id, master_pid, worker_pid, workers_num) {
	this->init(x, y, 1);
}
