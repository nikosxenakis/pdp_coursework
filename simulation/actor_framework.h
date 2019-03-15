#ifndef ACTOR_FRAMEWORK_H
#define ACTOR_FRAMEWORK_H

#include <mpi.h>

#include "pool.h"
#include "master.h"
#include "worker.h"
#include "messenger.h"

#define UPPER_BOUND_BUFFER_SIZE 10000

class Actor_framework {

private:
	static void (*init_actors)(Message message);

	static long get_seed();

	static void worker_code(int pid, Message message);
	static void master_code(int pid, Message message);



public:

	static void register_init_actors(void (init_actors)(Message message));
	static void register_spawn_actor(Actor* (spawn_actor)(Message message), Message message);

	static void spawn_actor(Message message);

	static void actor_framework(Message message);

};

#endif
