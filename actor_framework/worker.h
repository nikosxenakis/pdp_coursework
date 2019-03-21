#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <vector>
#include "actor.h"
#include "message.h"
#include "messenger.h"

using namespace std;

#define MASTER_PID 0

class Worker {

private:
	int pid;
	int init_actors_num;
	int workers_num;
	vector<Actor*> actors;
	int start_simulation;
	static Message input_data;
	static Actor* (*spawn_actor)(Message message);

	int actors_spawned;
	int actors_died;

	int get_pid();
	void add_actor(Actor *actor);
	void remove_actor(int actor_id);
	Actor* find_actor(int actor_id);
	void kill_all_actors();

	void compute();
	int process(Message message);

public:

	Worker(int pid, int init_actors_num, int workers_num);
	~Worker();
	static void register_spawn_actor(Actor* (spawn_actor)(Message message), Message message);
	void run();
	void finalize();
};

#endif
