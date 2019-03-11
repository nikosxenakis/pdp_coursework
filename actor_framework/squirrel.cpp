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
	squirrel->send_msg(squirrel->clock->get_id(), Message(TIMESTEP_START, squirrel->get_id(), squirrel->get_type()));
}

static void compute_simulate(Actor *actor) {
	int give_birth = 0;

	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	squirrel->set_state(WAIT);

	// cout << "Squirrel " << squirrel->get_id() << " computes timestep " << squirrel->timestep << endl;
	// squirrel->print();

	// move
	squirrel->move();

	if(squirrel->timestep == 2 && squirrel->get_id() == 17) {
		give_birth = 1;
	}
	if(squirrel->timestep == 1 && squirrel->get_id() == 19) {
		squirrel->set_state(FINISH);
		squirrel->die();
		return;
	}

	Message message;

	if(give_birth == 1) {
		cout << "Squirrel " << squirrel->get_id() << " gave birth in timestep "<< squirrel->timestep <<"\n";
		squirrel->create_actor(ACTOR_TYPE_SQUIRREL);
		message = Message(TIMESTEP_END_GAVE_BIRTH);
	}
	else {
		message = Message(TIMESTEP_END, squirrel->get_id(), squirrel->get_type());
	}

	squirrel->timestep++;
	squirrel->send_msg(squirrel->clock->get_id(), message);
}

static void parse_message_wait(Actor *actor, Message message) {
	Squirrel *squirrel = dynamic_cast<Squirrel*>(actor);
	if(message.command == TIMESTEP_START) {
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
	this->steps = 0;
	this->seed = Actor_framework::get_seed();

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

	


void Squirrel::move() {
	float x_new, y_new;

	squirrelStep(this->x, this->y, &x_new, &y_new, &this->seed);
	this->x = x_new;
	this->y = y_new;
	int cell_num = getCellFromPosition(this->x, this->y);
	// cout << "new cell num is " << cell_num << endl;
	this->visit(cell_num);
}

void Squirrel::visit(int actor_id) {
	cout << "Actor " << this->get_id() << ": wants to visit Cell " << actor_id << "\n";

	Actor *dst_actor = this->get_actor(actor_id);
	if(dst_actor) {
		// cout << "Actor " << this->get_id() << ": found Actor " << actor_id << "\n";
		Message visit_message = Message(VISIT_ACTOR_COMMAND, this->get_id(), dst_actor->get_id(), this->healthy);
		this->send_msg(dst_actor->get_id(), visit_message);	
	}
	else {
		cout << "Actor " << this->get_id() << ": Actor " << actor_id << " is not cached" << "\n";
	}
}
