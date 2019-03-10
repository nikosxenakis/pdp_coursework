/*
 * Example code to run and test the process pool. To compile use something like mpicc -o test test.c pool.c
 */
#ifndef ACTOR_FRAMEWORK_H
#define ACTOR_FRAMEWORK_H

#include <mpi.h>

#include "input_data.h"
#include "pool.h"
#include "master.h"
#include "messenger.h"

#define UPPER_BOUND_BUFFER_SIZE 10000

class Actor_framework {
public:
	static void (*init_actors)(Input_Data *input_data);

	static void worker_code(int pid, Input_Data *input_data);
	static void master_code(int pid, Input_Data *input_data);
	static void spawn_actor(int actor_type);
	static void register_init_actors(void (*)(Input_Data *input_data));
	static void actor_framework(int argc, char* argv[]);
};

#endif
