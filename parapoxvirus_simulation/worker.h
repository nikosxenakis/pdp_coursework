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
#include "actor_factory.h"

using namespace std;

class Worker
{

private:
	int pid;
	int master_pid;

public:
	vector<Actor*> actors;
	map<int, vector<Actor*>> known_actors;
	Worker(int pid, int master_pid);
	int get_load();
	int get_pid();
	void print();
	void run();
	int parse_message(Message message);
	void finalize();
	void add_actor(Actor *actor);
	void remove_actor(int actor_id);
	void discover_actor(int worker_pid, Actor *actor);
	Actor* find_actor(int actor_id);
	void compute();
	void kill_all_actors();
	void create_actor(int actor_type);
	~Worker();
};

#endif
