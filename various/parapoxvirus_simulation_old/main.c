/*
 * Example code to run and test the process pool. To compile use something like mpicc -o test test.c pool.c
 */

#include <stdio.h>
#include <mpi.h>
#include "pool.h"
#include "input_data.h"
#include "master.h"

#define UPPER_BOUND_BUFFER_SIZE 10000

static void workerCode(Input_data* input_data) {
	int myRank, master_pid;
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	master_pid = getCommandData();

	MPI_Send(NULL, 0, MPI_INT, master_pid, 0, MPI_COMM_WORLD);
}

static void masterCode(Input_data* input_data) {
	int i, returnCode;

	int active_actors = 0;
	int activeWorkers = input_data->workers_num;

	Worker* workers = init_workers(input_data->workers_num);

	print_input_data(input_data);

	int masterStatus = masterPoll();
	while (masterStatus) {
		masterStatus=masterPoll();
		for (i=0;i<input_data->workers_num;i++) {
			// Checks all outstanding workers that master spawned to see if they have completed
			if (workers[i].workerRequest != MPI_REQUEST_NULL) {
				MPI_Test(&workers[i].workerRequest, &returnCode, MPI_STATUS_IGNORE);
				if (returnCode) activeWorkers--;
			}
		}
		// If we have no more active workers then quit poll loop which will effectively shut the pool down when  processPoolFinalise is called
		if (activeWorkers==0) break;
	}

	// int masterStatus = masterPoll();
	// while (masterStatus) {
	// 	masterStatus=masterPoll();
	// 	if (active_actors==0) break;
	// }

}



int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);

	int statusCode = processPoolInit();
	int bsize;
	char *buffer, *bbuffer;

	// parse input data
	Input_data* input_data = init_input_data(argc, argv);
	// print_input_data(input_data);

	// set the buffer
    MPI_Pack_size( UPPER_BOUND_BUFFER_SIZE, MPI_BYTE, MPI_COMM_WORLD, &bsize );
	buffer = (char *)malloc( bsize );
	MPI_Buffer_attach( buffer, bsize );

	if (statusCode == 1) {
		workerCode(input_data);
	}
	else if (statusCode == 2) {
		masterCode(input_data);
	}

	MPI_Buffer_detach( &bbuffer, &bsize );

	processPoolFinalise();
	MPI_Finalize();
	return 0;
}
