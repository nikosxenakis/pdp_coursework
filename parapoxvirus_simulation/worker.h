#ifndef WORKER_H
#define WORKER_H

#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>
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
	vector<Actor*> known_actors;
	Worker(int pid, int master_pid);
	int get_load();
	int get_pid();
	void print();
	void run();
	int parse_message(Message message);
	void finalize();
	void add_actor(Actor *actor);
	void remove_actor(Actor *actor);
	Actor* find_actor(int actor_id);

	~Worker();
	void check_messages();
	void process_message();
	void compute();
};

#endif
