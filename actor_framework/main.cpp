/*
 * Example code to run and test the process pool. To compile use something like mpicc -o test test.c pool.c
 */

#include <mpi.h>

#include "input_data.h"
#include "pool.h"
#include "master.h"
#include "messenger.h"

#define UPPER_BOUND_BUFFER_SIZE 10000

static void worker_code(int pid, Input_Data *input_data) {
	int master_pid = getCommandData();
	Worker *worker = new Worker(pid, master_pid, input_data->init_actors_num);
	worker->run();
	worker->finalize();
}

static void master_code(int pid, Input_Data *input_data) {
	int world_size, workers_num;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	workers_num = world_size - 1;
	Master::initialize_master(pid, workers_num, input_data->max_actors_num);

	for (int i = 0; i < input_data->clocks; ++i)
		Master::spawn_actor(ACTOR_TYPE_CLOCK);
	for (int i = 0; i < input_data->squirells; ++i)
		Master::spawn_actor(ACTOR_TYPE_SQUIRREL);
	
	Master::run();

	Master::finalize();
}

int main(int argc, char* argv[]) {

	int pid;
	int bsize;
	char *buffer, *bbuffer;

	MPI_Init(&argc, &argv);

	Input_Data *input_data = new Input_Data(argc, argv);

	Messenger::init_types();

	MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    MPI_Pack_size( UPPER_BOUND_BUFFER_SIZE, MPI_BYTE, MPI_COMM_WORLD, &bsize );
	buffer = (char *)malloc( bsize );
	MPI_Buffer_attach( buffer, bsize );

	int statusCode = processPoolInit();
	if (statusCode == 1) {
		worker_code(pid, input_data);
	} else if (statusCode == 2) {
		master_code(pid, input_data);
	}

	MPI_Buffer_detach( &bbuffer, &bsize );
	processPoolFinalise();
	MPI_Finalize();
	return 0;
}
