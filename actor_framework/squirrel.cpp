#include "squirrel.h"

#define INIT 0
#define SIMULATE 1
#define WAIT 2
#define FINISH 3

static void compute_dummy(Actor *actor) {}
static void parse_message_dummy(Actor *actor, Message message) {}

static void compute_init(Actor *actor) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	squirrel->set_state(WAIT);
	squirrel->clock = squirrel->get_actors_by_type(ACTOR_TYPE_CLOCK)[0];

	Message message;
	message.message_data.command = TIMESTEP_START;
	message.message_data.actor_id = squirrel->get_id();
	message.message_data.actor_type = squirrel->get_type();

	squirrel->send_msg(squirrel->clock->get_id(), message);
}

static void compute_simulate(Actor *actor) {
	int give_birth = 0, will_die = 0;

	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	squirrel->set_state(WAIT);

	// cout << "Squirrel " << squirrel->get_id() << " computes timestep " << squirrel->timestep << endl;
	// squirrel->print();
	int times = 0;

	while(times < 100) {
		will_die = squirrel->move();
		if(will_die) {
			return;
		}
	}
	times = 0;

	Message message;
	message.message_data.actor_type = squirrel->get_type();

	if(give_birth == 1) {
		cout << "Squirrel " << squirrel->get_id() << " gave birth in timestep "<< squirrel->timestep <<"\n";
		squirrel->create_actor(ACTOR_TYPE_SQUIRREL);
		message.message_data.command = TIMESTEP_END_GAVE_BIRTH;
	}
	else {
		message.message_data.command = TIMESTEP_END;
		message.message_data.actor_id = squirrel->get_id();
	}

	squirrel->timestep++;
	squirrel->send_msg(squirrel->clock->get_id(), message);
}

static void parse_message_wait(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	if(message.message_data.command == TIMESTEP_START) {
		squirrel->set_state(SIMULATE);
	}
}

Squirrel::Squirrel(int id, int master_pid, int worker_pid, float x, float y): Actor(id, master_pid, worker_pid) {
	this->type = ACTOR_TYPE_SQUIRREL;
	this->clock = nullptr;
	this->timestep = 1;
	this->x = x;
	this->y = y;
	this->healthy = 1;
	this->step_no = 0;
	this->infected_steps = 0;
	this->seed = Actor_framework::get_seed();
	this->steps = vector<int>(50);

	for (int i = 0; i < 50; ++i)
		this->steps[i] = 0;

	this->set_state(INIT);

	this->register_state(COMPUTE, INIT, compute_init);
	this->register_state(COMPUTE, SIMULATE, compute_simulate);
	this->register_state(COMPUTE, WAIT, compute_dummy);
	this->register_state(COMPUTE, FINISH, compute_dummy);

	this->register_state(PARSE_MESSAGE, INIT, parse_message_dummy);
	this->register_state(PARSE_MESSAGE, SIMULATE, parse_message_dummy);
	this->register_state(PARSE_MESSAGE, WAIT, parse_message_wait);
	this->register_state(PARSE_MESSAGE, FINISH, parse_message_dummy);
}

void Squirrel::print() {
	cout << "Squirrel " << this->get_id() << " in cell number " << getCellFromPosition(this->x, this->y) << endl;
}

void Squirrel::visit(int actor_id) {
	// cout << "Actor " << this->get_id() << ": wants to visit Cell " << actor_id << "\n";

	Actor *dst_actor = this->get_actor(actor_id);
	if(dst_actor) {
		// cout << "Actor " << this->get_id() << ": found Actor " << actor_id << "\n";

		Message_data message_data = {VISIT_ACTOR_COMMAND, this->get_id(), dst_actor->get_id(), -1, -1, this->healthy};
		Message visit_message = Message(message_data);
		this->send_msg(dst_actor->get_id(), visit_message);	
	}
	else {
		cout << "Actor " << this->get_id() << ": Actor " << actor_id << " is not cached" << "\n";
	}
}

int Squirrel::move() {
	float x_new, y_new;

	squirrelStep(this->x, this->y, &x_new, &y_new, &this->seed);
	this->x = x_new;
	this->y = y_new;
	int cell_num = getCellFromPosition(this->x, this->y);

	this->steps[this->step_no%50] = cell_num;

	this->step_no++;
	if(this->healthy == 0)
		this->infected_steps++;

	// cout << "new cell num is " << cell_num << endl;
	this->visit(cell_num);


	if(this->step_no%50 == 0)
		give_birth = this->birth();
	
	return this->will_die();

}

int Squirrel::birth() {
	int give_birth;
	float avg_pop = 3;

//	if(this->timestep == 1 && this->get_id() == 17) {
//		give_birth = 1;
//	}

	// for (int i = 0; i < 50; ++i) {

	// 	this->steps[i];

	// }

	// this->steps

	// if(this->step_no%50)

	// int populationInflux = 10;
	// int infectionLevel = 10;



	give_birth = willGiveBirth(avg_pop, &this->seed);

	return give_birth;
}

int Squirrel::will_die() {
	int will_die = 0;

	if(this->infected_steps >= 50) {

		will_die = willDie(&this->seed);
		if(will_die) {
			this->set_state(FINISH);
			this->die();
		}

	}

	return will_die;
}




