/*
 * Example code to run and test the process pool. To compile use something like mpicc -o test test.c pool.c
 */

#include "actor_framework.h"

void (*Actor_framework::init_actors)(void *input_data);

void Actor_framework::worker_code(int pid, int init_actors_num) {
	int master_pid = getCommandData();
	Worker *worker = new Worker(pid, master_pid, init_actors_num);
	worker->run();
	worker->finalize();
}

void Actor_framework::master_code(int pid, void *input_data, int max_actors_num) {
	int world_size, workers_num;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	workers_num = world_size - 1;
	Master::initialize_master(pid, workers_num, max_actors_num);

	Actor_framework::init_actors(input_data);

	Master::run();

	Master::finalize();
}

void Actor_framework::spawn_actor(int actor_type) {
	Master::spawn_actor(actor_type);
}

void Actor_framework::register_init_actors(void (init_actors)(void *input_data)) {
	Actor_framework::init_actors = init_actors;
}

void Actor_framework::register_create_actor(Actor* (create_actor)(int actor_id, int actor_type, int master_pid, int worker_pid, void *data), void *data) {
	Master::register_create_actor(create_actor, data);
	Worker::register_create_actor(create_actor, data);
}

void Actor_framework::actor_framework(void *input_data, int max_actors_num, int init_actors_num) {
	int pid;
	int bsize;
	char *buffer, *bbuffer;

	MPI_Init(NULL, NULL);


	Messenger::init_types();

	MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    MPI_Pack_size( UPPER_BOUND_BUFFER_SIZE, MPI_BYTE, MPI_COMM_WORLD, &bsize );
	buffer = (char *)malloc( bsize );
	MPI_Buffer_attach( buffer, bsize );

	int statusCode = processPoolInit();
	if (statusCode == 1) {
		Actor_framework::worker_code(pid, init_actors_num);
	} else if (statusCode == 2) {
		Actor_framework::master_code(pid, input_data, max_actors_num);
	}

	MPI_Buffer_detach( &bbuffer, &bsize );

	processPoolFinalise();
	MPI_Finalize();
}
