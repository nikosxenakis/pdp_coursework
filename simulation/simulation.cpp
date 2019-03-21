#include "simulation.h"

Actor* spawn_actor(Message message) {

	int id = message.get(ACTOR_ID);
	int max_months = message.get(MAX_MONTHS);
	int x = message.get(X);
	int y = message.get(Y);
	int healthy = message.get(HEALTHY);
	int worker_pid = message.get(WORKER_PID);
	int workers_num = message.get(WORKERS_NUM);
	int init_squirrels_no = message.get(SQUIRRELS);
	int init_inf_squirrels_no = message.get(INFECTION_LEVEL);

	if(message.get(ACTOR_TYPE) == ACTOR_TYPE_CELL){
		return new Cell(id, worker_pid, workers_num, max_months);
	}
	else if(message.get(ACTOR_TYPE) == ACTOR_TYPE_CLOCK){
		return new Clock(id, worker_pid, workers_num, max_months, init_squirrels_no, init_inf_squirrels_no);
	}
	else if(message.get(ACTOR_TYPE) == ACTOR_TYPE_SQUIRREL){
		return new Squirrel(id, worker_pid, workers_num, x, y, healthy);
	}
	else
		assert(0);

	return nullptr;
}

void init_actors(Message message) {
	for (int i = 0; i < message.get(CELLS); ++i){
		message.set(ACTOR_TYPE, ACTOR_TYPE_CELL);
		Actor_framework::spawn_actor(message);
	}
	for (int i = 0; i < message.get(CLOCKS); ++i){
		message.set(ACTOR_TYPE, ACTOR_TYPE_CLOCK);
		Actor_framework::spawn_actor(message);
	}
	for (int i = 0; i < message.get(SQUIRRELS); ++i){
		message.set(ACTOR_TYPE, ACTOR_TYPE_SQUIRREL);
		message.set(X, 0.0);
		message.set(Y, 0.0);
		if(message.get(INFECTION_LEVEL) > 0 ) {
			message.set(HEALTHY, 0);
			message.set(INFECTION_LEVEL, message.get(INFECTION_LEVEL) - 1);
		}
		else {
			message.set(HEALTHY, 1);
		}
		Actor_framework::spawn_actor(message);
	}
}

Message parse_args(Message *message, int argc, char* argv[]) {
	int clocks = atoi(argv[1]);
	int cells = atoi(argv[2]);
	int squirrels = atoi(argv[3]);

	message->set(CLOCKS, clocks);
	message->set(CELLS, cells);
	message->set(SQUIRRELS, squirrels);
	message->set(INFECTION_LEVEL, atoi(argv[4]));
	message->set(INIT_ACTORS_NUM, clocks + cells + squirrels);
	message->set(MAX_MONTHS, atoi(argv[5]));
	message->set(MAX_ACTORS_NUM, atoi(argv[6]));
	message->set(HEALTHY, 1);
	message->set(X, 0);
	message->set(Y, 0);
}

int main(int argc, char* argv[]) {

	Message message;
	parse_args(&message, argc, argv);

	Actor_framework::register_init_actors(init_actors);

	Actor_framework::register_spawn_actor(spawn_actor, message);

	Actor_framework::actor_framework(message);

	return 0;
}
