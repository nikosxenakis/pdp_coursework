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

static void workerCode();

static void check_workers(int max_proc_num, MPI_Request *initialWorkerRequests, MPI_Request *initialWorkerBirthRequests, int activeWorkers) {
	int returnCode, i;
	int masterStatus = masterPoll();
	int worker_pid;

	while (masterStatus) {
		masterStatus=masterPoll();
		for (i=0;i<max_proc_num;i++) {
			worker_pid=hashWorkerId(i);

			// Checks all outstanding workers that master spawned to see if they have completed
			if ( initialWorkerRequests[worker_pid] != MPI_REQUEST_NULL) {

				MPI_Test(&initialWorkerRequests[worker_pid], &returnCode, MPI_STATUS_IGNORE);
				if (returnCode)
				{
					activeWorkers--;
					initialWorkerRequests[worker_pid] = MPI_REQUEST_NULL;
					initialWorkerBirthRequests[worker_pid] = MPI_REQUEST_NULL;
				}
			}

			printf("%d\n", activeWorkers);
			if(activeWorkers > 0)
			{
				if (initialWorkerBirthRequests[worker_pid] != MPI_REQUEST_NULL) {

					MPI_Test(&initialWorkerBirthRequests[worker_pid], &returnCode, MPI_STATUS_IGNORE);
					if (returnCode)
					{
						initialWorkerBirthRequests[worker_pid] = MPI_REQUEST_NULL;
						MPI_Irecv(NULL, 0, MPI_INT, worker_pid, 1, MPI_COMM_WORLD, &initialWorkerBirthRequests[worker_pid]);
						// activeWorkers++;
					}
				}
			}
			
		}
		// If we have no more active workers then quit poll loop which will effectively shut the pool down when  processPoolFinalise is called
		if (activeWorkers==0) break;
	}
	printf("OUTT\n");
}

static void masterStartWorker(int *actors_array, int activeWorkers) {
	int workerPid = startWorkerProcess();			
	actors_array[activeWorkers] = workerPid;
}

int main(int argc, char* argv[]) {
	// Call MPI initialize first
	MPI_Init(&argc, &argv);

	/*
	 * Initialise the process pool.
     * The return code is = 1 for worker to do some work, 0 for do nothing and stop and 2 for this is the master so call master poll
     * For workers this subroutine will block until the master has woken it up to do some work
	 */
	int statusCode = processPoolInit();
	if (statusCode == 1) {
		// A worker so do the worker tasks
		workerCode();
	} else if (statusCode == 2) {
		/*
		 * This is the master, each call to master poll will block until a message is received and then will handle it and return
         * 1 to continue polling and running the pool and 0 to quit.
         * Basically it just starts 10 workers and then registers when each one has completed. When they have all completed it
         * shuts the entire pool down
		 */
		int i, activeWorkers=0;
		MPI_Request *initialWorkerRequests;
		MPI_Request *initialWorkerBirthRequests;
		int worker_type;

		int cells, squirells, infection_level, max_months;
		int max_proc_num;
		int cells_deployed=0, squirells_deployed=0, clock_deployed=0;
		int clockPid = -1;

		cells = atoi(argv[1]);
		squirells = atoi(argv[2]);
		infection_level = atoi(argv[3]);
		max_months = atoi(argv[4]);

		printf("CELLS: %s\n", argv[1]);
		printf("SQUIRRELS: %s\n", argv[2]);
		printf("INFECTION_LEVEL: %s\n", argv[3]);
		printf("MAX_MONTHS: %s\n", argv[4]);

		max_proc_num = cells + squirells + 1;
		initialWorkerRequests = malloc(max_proc_num*sizeof(MPI_Request));
		initialWorkerBirthRequests = malloc(max_proc_num*sizeof(MPI_Request));


		for (i=0;i<max_proc_num;i++) {
			int workerPid = startWorkerProcess();			

			// death message
			MPI_Irecv(NULL, 0, MPI_INT, workerPid, 0, MPI_COMM_WORLD, &initialWorkerRequests[hashWorkerId(i)]);

			worker_type = NONE;

			// send what type of worker to create
			if(clock_deployed < 1) {
				worker_type = CLOCK;
				clockPid = workerPid;
				clock_deployed++;
			}
			else if(squirells_deployed < squirells) {
				worker_type = SQUIRREL;
				squirells_deployed++;
				// birth message
				MPI_Irecv(NULL, 0, MPI_INT, workerPid, 1, MPI_COMM_WORLD, &initialWorkerBirthRequests[hashWorkerId(i)]);
			}

			// Send type
			MPI_Send(&worker_type, 1, MPI_INT, workerPid, 0, MPI_COMM_WORLD);

			// Send clock
			MPI_Send(&clockPid, 1, MPI_INT, workerPid, 0, MPI_COMM_WORLD);

			// Send max_months
			MPI_Send(&max_months, 1, MPI_INT, workerPid, 0, MPI_COMM_WORLD);

			activeWorkers++;
			printf("Master: spawned worker. pid = %d, type = %d\n", workerPid, worker_type);
		}
		activeWorkers--;

		// Send workers number to clock
		MPI_Send(&activeWorkers, 1, MPI_INT, clockPid, 0, MPI_COMM_WORLD);

		check_workers(max_proc_num, initialWorkerRequests, initialWorkerBirthRequests, activeWorkers);

	}
	// Finalizes the process pool, call this before closing down MPI
	processPoolFinalise();
	// Finalize MPI, ensure you have closed the process pool first
	MPI_Finalize();
	return 0;
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

static int die(int myRank, int parentId, int clockPid) {
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
		int clockPid;
		int max_months=0;

		MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
		parentId = getCommandData();	// We encode the parent ID in the wake up command data

		// get worker type
		MPI_Recv(&worker_type, 1, MPI_INT, parentId, 0, MPI_COMM_WORLD, &status);

		// get clockPid
		MPI_Recv(&clockPid, 1, MPI_INT, parentId, 0, MPI_COMM_WORLD, &status);

		// get max_months
		MPI_Recv(&max_months, 1, MPI_INT, parentId, 0, MPI_COMM_WORLD, &status);

		printf("Worker: started. pid = %d, type %d, parent = %d, clock = %d\n", myRank, worker_type, parentId, clockPid);


		if(worker_type == CLOCK)
		{
			clock_worker_code(parentId, max_months);
		}
		else if(worker_type == SQUIRREL)
		{
			squirrel_worker_code(myRank, parentId, clockPid, max_months);
		}

		workerStatus = die(myRank, parentId, clockPid);

	}
}
