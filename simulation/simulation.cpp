#include "simulation.h"

Actor* spawn_actor(Message message) {

	int id = message.get(ACTOR_ID);
	int workers_num = message.get(WORKERS_NUM);

	if(message.get(ACTOR_TYPE) == ACTOR_TYPE_CELL) {
		return new Cell(id, workers_num);
	}
	else if(message.get(ACTOR_TYPE) == ACTOR_TYPE_CLOCK) {
		int init_squirrels_no = message.get(SQUIRRELS);
		int init_inf_squirrels_no = message.get(INFECTION_LEVEL);
		int max_months = message.get(MAX_MONTHS);
		return new Clock(id, workers_num, max_months, init_squirrels_no, init_inf_squirrels_no);
	}
	else if(message.get(ACTOR_TYPE) == ACTOR_TYPE_SQUIRREL) {
		int x = message.get(X);
		int y = message.get(Y);
		int healthy = message.get(HEALTHY);
		return new Squirrel(id, workers_num, x, y, healthy);
	}
	else
		assert(0);

	return nullptr;
}

void init_actors(Message message) {
	for (int i = 0; i < message.get(CELLS); ++i) {
		message.set(ACTOR_TYPE, ACTOR_TYPE_CELL);
		Actor_framework::spawn_actor(message);
	}
	for (int i = 0; i < message.get(CLOCKS); ++i) {
		message.set(ACTOR_TYPE, ACTOR_TYPE_CLOCK);
		Actor_framework::spawn_actor(message);
	}
	for (int i = 0; i < message.get(SQUIRRELS); ++i) {
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

Message parse_args(int argc, char* argv[]) {
	int squirrels = atoi(argv[1]);
	int max_squirrels_num = atoi(argv[4]);

	Message message;
	message.set(CLOCKS, CLOCKS_NO);
	message.set(CELLS, CELLS_NO);
	message.set(SQUIRRELS, squirrels);
	message.set(INFECTION_LEVEL, atoi(argv[2]));
	message.set(INIT_ACTORS_NUM, CLOCKS_NO + CELLS_NO + squirrels);
	message.set(MAX_MONTHS, atoi(argv[3]));
	message.set(MAX_ACTORS_NUM, max_squirrels_num + CLOCKS_NO + CELLS_NO);
	message.set(HEALTHY, 1);
	message.set(X, 0);
	message.set(Y, 0);
	return message;
}

int main(int argc, char* argv[]) {

	Message message = parse_args(argc, argv);

	Actor_framework::register_init_actors(init_actors);

	Actor_framework::register_spawn_actor(spawn_actor);

	Actor_framework::actor_framework(message);

	return 0;
}
