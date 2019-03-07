#ifndef WORKER_H
#define WORKER_H

#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>
#include "actor.h"

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
	void add_actor(Actor *actor);

	~Worker();
	void check_messages();
	void process_message();
	void compute();
};

#endif
