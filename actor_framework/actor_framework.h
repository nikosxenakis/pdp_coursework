/*
 * Example code to run and test the process pool. To compile use something like mpicc -o test test.c pool.c
 */
#ifndef ACTOR_FRAMEWORK_H
#define ACTOR_FRAMEWORK_H

#include <mpi.h>

#include "pool.h"
#include "master.h"
#include "messenger.h"

#define UPPER_BOUND_BUFFER_SIZE 10000

class Actor_framework {
public:

	static long seed;

	static void (*init_actors)(void *input_data);
	static void (*initialiseRNG)(long *seed);

	static long get_seed();

	static void worker_code(int pid, int init_actors_num);
	static void master_code(int pid, void *input_data, int max_actors_num);
	static void spawn_actor(Message message);

	static void register_initialiseRNG(void (*initialiseRNG)(long *seed));
	static void register_init_actors(void (*)(void *input_data));
	static void register_create_actor(Actor* (create_actor)(int actor_id, int actor_type, int master_pid, int worker_pid, int workers_num, void *data), void *data);

	static void actor_framework(void *input_data, int max_actors_num, int init_actors_num);
};

#endif
