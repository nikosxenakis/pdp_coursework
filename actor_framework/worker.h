#ifndef WORKER_H
#define WORKER_H

#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "actor.h"
#include "message.h"
#include "messenger.h"

using namespace std;

class Worker
{

private:
	int pid;
	int master_pid;
	int init_actors_num;

public:
	vector<Actor*> actors;
	map<int, vector<Actor*>> known_actors;
	int start_simulation;

	static void* input_data;
	static Actor* (*create_actor_f)(int actor_id, int actor_type, int master_pid, int worker_pid, void* data);

	Worker(int pid, int master_pid, int init_actors_num);
	static void register_create_actor(Actor* (*)(int actor_id, int actor_type, int master_pid, int worker_pid, void* data), void *data);
	int get_load();
	int get_pid();
	void print();
	void run();
	int parse_message(Message message);
	void finalize();
	void add_actor(Actor *actor);
	void remove_actor(int actor_id);
	void discover_actor(int worker_pid, Actor *actor);
	void forget_actor(int actor_id);
	void update_actor(Actor *actor);
	Actor* find_actor(int actor_id);
	int get_total_actors_num();
	void compute();
	void kill_all_actors();
	void create_actor(int actor_type);
	~Worker();
};

#endif
