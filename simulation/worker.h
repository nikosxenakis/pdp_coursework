#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <vector>
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
	int workers_num;

	vector<Actor*> actors;
	int start_simulation;

	static Message input_data;

	static Actor* (*spawn_actor)(Message message);

	Worker(int pid, int master_pid, int init_actors_num, int workers_num);

	static void register_spawn_actor(Actor* (spawn_actor)(Message message), Message message);
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
