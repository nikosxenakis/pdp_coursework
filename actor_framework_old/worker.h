#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <vector>
#include "actor.h"
#include "message.h"
#include "messenger.h"
#include "input_data.h"

using namespace std;

class Worker
{

private:
	int pid;
	int master_pid;
	int init_actors_num;

public:
	int workers_num;

	vector<Actor*> actors;
	int start_simulation;

	static void* input_data;
	static Actor* (*create_actor_f)(int actor_id, int actor_type, int master_pid, int worker_pid, int workers_num, void* data);

	Worker(int pid, int master_pid, int init_actors_num, int workers_num);
	static void register_create_actor(Actor* (*)(int actor_id, int actor_type, int master_pid, int worker_pid, int workers_num, void* data), void *data);
	int get_load();
	int get_pid();
	void print();
	void run();
	int parse_message(Message message);
	void finalize();
	void add_actor(Actor *actor);
	void remove_actor(int actor_id);
	Actor* find_actor(int actor_id);
	void compute();
	void kill_all_actors();
	void create_actor(int actor_type);
	~Worker();
};

#endif
