/*
 * Example code to run and test the process pool. To compile use something like mpicc -o test test.c pool.c
 */

#include <stdio.h>
#include <mpi.h>
#include "pool.h"

#define MASTER_ID 0
#define MAX_SQUIRRELS 200

#define WORKERS_CONTROL_CHANNEL 111

enum Worker_type{SQUIRREL=0, CELL=1, CLOCK=2, NONE=3};

// typedef struct Worker_data {
// 	int pid;
// 	int worker_type;
// } Worker_data;

int hashWorkerId(int i) {
	return i-1;
}

static void masterStartWorker(int *actors_array, int activeWorkers) {
	int workerPid = startWorkerProcess();			
	actors_array[activeWorkers] = workerPid;
}

static void squirrel_worker_code(int myRank, int parentId, int clockPid, int max_months) {
	// spawn 1 squirrel if first child
	// if(parentId == MASTER_ID) {
	// 	int childPid = startWorkerProcess();
	// 	printf("Worker on process %d, started child processes %d\n", myRank, childPid);
	// 	MPI_Request childRequests;
	// 	MPI_Irecv(NULL, 0, MPI_INT, childPid, 0, MPI_COMM_WORLD, &childRequests);

	// 	int worker_type = SQUIRREL;
	// 	MPI_Send(&worker_type, 1, MPI_INT, childPid, 0, MPI_COMM_WORLD);

	// 	MPI_Waitall(1, &childRequests, MPI_STATUS_IGNORE);
	// }
	MPI_Status status;
	int timestep = 0;
	int goingToDie = 0, goingToGiveBirth = 0;
	int once=2;
	// if(myRank == 11) {
	// 	MPI_Recv(&d, 1, MPI_INT, clockPid, 0, MPI_COMM_WORLD, &status);
	// 	printf("Worker received %d from %d\n", d, parentId);
	// }

	// ready to work
	MPI_Send(&myRank, 1, MPI_INT, clockPid, 0, MPI_COMM_WORLD);


	// simulation
	while(timestep < max_months-1) {
		MPI_Recv(&timestep, 1, MPI_INT, clockPid, 0, MPI_COMM_WORLD, &status);
		// printf("squirell %d: timestep %d started\n", myRank, timestep);

		// calculate
		if(myRank == 2) {
			printf("I WANT TO DIE!!!\n");
			goingToDie=1;
		}
		// if(myRank == 14) {
		// 	printf("I WANT TO GIVE BIRTH!!!\n");
		// 	goingToGiveBirth=1;
		// }

		if(once>0) {
			MPI_Send(NULL, 0, MPI_INT, parentId, 1, MPI_COMM_WORLD);
			once--;
		}

		if(goingToDie) {
			timestep=-1;
			MPI_Send(&timestep, 1, MPI_INT, clockPid, 0, MPI_COMM_WORLD);
			break;
		}
		// if(goingToGiveBirth) {

		// }

		MPI_Send(&timestep, 1, MPI_INT, clockPid, 0, MPI_COMM_WORLD);

	}

	// MPI_Send(NULL, 1, MPI_INT, clockPid, 0, MPI_COMM_WORLD);


	// // loop
	// 	// start timestep

	// 	// calculate
	// 	// end timestep
	// 	MPI_Send(NULL, 1, MPI_INT, clockPid, 0, MPI_COMM_WORLD);


}

static void cell_worker_code(int parentId) {
	MPI_Status status;
	int d;
	MPI_Recv(&d, 1, MPI_INT, parentId, 1, MPI_COMM_WORLD, &status);
	printf("Worker received %d from %d\n", d, parentId);
}

static void print_actors(int *actors_array, int actors_array_size) {
	int i;
	for (i = 0; i < actors_array_size; ++i)
	{
		printf("%d ", actors_array[i]);
	}
	printf("\n");
}

static void clock_worker_code(int parentId, int max_months) {
	int *actors_array;
	int actors_array_size;
	MPI_Status status;
	int i;
	int worker_id;
	int timestep, tmp_val;

	// initialize clock
	// receive actors list
	MPI_Recv(&actors_array_size, 1, MPI_INT, parentId, 0, MPI_COMM_WORLD, &status);
	actors_array = malloc(actors_array_size*sizeof(int));
	printf("actors_array_size = %d\n", actors_array_size);

	// wait for actors to be ready
	for (i = 0; i < actors_array_size; i++)
	{
		MPI_Recv(&(actors_array[i]), 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
	}


	print_actors(actors_array, actors_array_size);


	// simulation
	for (timestep = 0; timestep < max_months; ++timestep)
	{
		// start timestep
		printf("clock: start timestep %d\n", timestep);
		for (i = 0; i < actors_array_size; ++i)
		{
			MPI_Send(&timestep, 1, MPI_INT, actors_array[i], 0, MPI_COMM_WORLD);
		}
		//wait
		for (i = 0; i < actors_array_size; ++i)
		{
			MPI_Recv(&tmp_val, 1, MPI_INT, actors_array[i], 0, MPI_COMM_WORLD, &status);
			// remove from active actors
			if(tmp_val == -1) {				
				actors_array[i] = actors_array[actors_array_size-1];
				actors_array_size--;
			}

		}
	}

	print_actors(actors_array, actors_array_size);

	// end simulation


	// // loop
	// 	
	// 	// wait
	// 	// end timestep
	// 	for (i = 0; i < actors_array_size; ++i)
	// 	{
	// 		MPI_Recv(NULL, 1, MPI_INT, actors_array[i], 0, MPI_COMM_WORLD, &status);
	// 	}
	// 	printf("clock: end timestep\n");
	// 	// test if actor died

}





















static int die(int myRank, int parentId) {
	printf("worker %d died\n", myRank);
	MPI_Send(NULL, 0, MPI_INT, parentId, 0, MPI_COMM_WORLD);
	MPI_Send(NULL, 0, MPI_INT, parentId, 1, MPI_COMM_WORLD);
	return workerSleep();
}

static void workerCode() {
	int workerStatus = 1;
	while (workerStatus) {
		int myRank, parentId, worker_type=-1;
		MPI_Status status;
		int max_months=0;

		MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
		parentId = getCommandData();

		// get worker type
		MPI_Recv(&worker_type, 1, MPI_INT, parentId, 0, MPI_COMM_WORLD, &status);

		// get max_months
		MPI_Recv(&max_months, 1, MPI_INT, parentId, 0, MPI_COMM_WORLD, &status);

		printf("Worker: started. pid = %d, type %d, parent = %d\n", myRank, worker_type, parentId);


		// if(worker_type == CLOCK)
		// {
		// 	clock_worker_code(parentId, max_months);
		// }
		// else if(worker_type == SQUIRREL)
		// {
		// 	squirrel_worker_code(myRank, parentId, clockPid, max_months);
		// }

		workerStatus = die(myRank, parentId);

	}
}

static void check_workers(int max_actors, MPI_Request *deathRequests, MPI_Request *birthRequests) {
	int returnCode, i;
	int worker_pid;
	int masterStatus = masterPoll();

	while (masterStatus) {
		masterStatus=masterPoll();
		for (i=0; i<max_actors; i++) {

			worker_pid=hashWorkerId(i);

			// Checks all outstanding workers that master spawned to see if they have completed
			if ( deathRequests[worker_pid] != MPI_REQUEST_NULL) {

				MPI_Test(&deathRequests[worker_pid], &returnCode, MPI_STATUS_IGNORE);
				if (returnCode)
				{
					activeWorkers--;
					deathRequests[worker_pid] = MPI_REQUEST_NULL;
				}
			}

			if (birthRequests[worker_pid] != MPI_REQUEST_NULL) {

				MPI_Test(&birthRequests[worker_pid], &returnCode, MPI_STATUS_IGNORE);
				if (returnCode)
				{
					birthRequests[worker_pid] = MPI_REQUEST_NULL;
					MPI_Irecv(NULL, 0, MPI_INT, worker_pid, 1, MPI_COMM_WORLD, &birthRequests[worker_pid]);
				}
			}
			
		}

		if (activeWorkers==0) break;
	}
}

static void masterCode(int argc, char* argv[]) {
	int i, activeWorkers=0, max_actors=0;
	MPI_Request *initialWorkerRequests, *initialWorkerBirthRequests;
	int worker_type;
	int cells, squirells, infection_level, max_months;
	int cells_deployed=0, squirells_deployed=0;
	int *actor_pids;

	cells = atoi(argv[1]);
	squirells = atoi(argv[2]);
	infection_level = atoi(argv[3]);
	max_months = atoi(argv[4]);

	printf("CELLS: %s\n", argv[1]);
	printf("SQUIRRELS: %s\n", argv[2]);
	printf("INFECTION_LEVEL: %s\n", argv[3]);
	printf("MAX_MONTHS: %s\n", argv[4]);

	max_actors = cells + squirells;

	deathRequests = malloc(max_actors*sizeof(MPI_Request));
	birthRequests = malloc(max_actors*sizeof(MPI_Request));
	actor_pids = malloc(max_actors*sizeof(int));

	for (i=0; i<max_actors; i++) {

		int actor_pid = startWorkerProcess();			
		actor_pids[i]=actor_pid;
		// death message
		MPI_Irecv(NULL, 0, MPI_INT, actor_pid, 0, MPI_COMM_WORLD, &deathRequests[hashWorkerId(i)]);
		// birth message
		MPI_Irecv(NULL, 0, MPI_INT, actor_pid, 1, MPI_COMM_WORLD, &birthRequests[hashWorkerId(i)]);

		// send what type of worker to create
		if(squirells_deployed < squirells) {
			worker_type = SQUIRREL;
			squirells_deployed++;
		}
		else if(cells_deployed < cells) {
			worker_type = CELL;
			cells_deployed++;
		}

		// Send type
		MPI_Send(&worker_type, 1, MPI_INT, actor_pid, 0, MPI_COMM_WORLD);

		// Send max_months
		MPI_Send(&max_months, 1, MPI_INT, actor_pid, 0, MPI_COMM_WORLD);

		printf("Master: spawned worker. pid = %d, type = %d\n", actor_pid, worker_type);
	}

	check_workers(max_actors, deathRequests, birthRequests);
}

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);

	int statusCode = processPoolInit();

	if (statusCode == 1) {
		workerCode();
	}
	else if (statusCode == 2) {
		masterCode(argc, argv);
	}

	processPoolFinalise();
	MPI_Finalize();
	return 0;
}
