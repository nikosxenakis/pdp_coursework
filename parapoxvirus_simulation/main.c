/*
 * Example code to run and test the process pool. To compile use something like mpicc -o test test.c pool.c
 */

#include <stdio.h>
#include <mpi.h>
#include "pool.h"

#define MASTER_ID 0
#define MAX_SQUIRRELS 200

enum Worker_type{SQUIRREL=0, CELL=1, CLOCK=2};

static void workerCode();

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
		int i, activeWorkers=0, returnCode;
		MPI_Request *initialWorkerRequests;
		int cells, squirells, infection_level, max_months;
		int max_proc_num;
		int cells_deployed=0, squirells_deployed=0, clock_deployed=0;

		cells = atoi(argv[1]);
		squirells = atoi(argv[2]);
		infection_level = atoi(argv[3]);
		max_months = atoi(argv[4]);

		printf("CELLS: %s\n", argv[1]);
		printf("SQUIRRELS: %s\n", argv[2]);
		printf("INFECTION_LEVEL: %s\n", argv[3]);
		printf("MAX_MONTHS: %s\n", argv[4]);

		max_proc_num = cells + squirells;
		initialWorkerRequests = malloc(max_proc_num*sizeof(MPI_Request));

		for (i=0;i<max_proc_num;i++) {
			int workerPid = startWorkerProcess();
			MPI_Irecv(NULL, 0, MPI_INT, workerPid, 0, MPI_COMM_WORLD, &initialWorkerRequests[i]);

			int worker_type = SQUIRREL;

			// send what type of worker to create
			if(cells_deployed < cells) {
				worker_type = CELL;
				cells_deployed++;
			}
			else if(squirells_deployed < squirells) {
				worker_type = SQUIRREL;
				squirells_deployed++;
			}
			else if(clock_deployed < 1) {
				worker_type = CLOCK;
				clock_deployed++;
			}

			MPI_Send(&worker_type, 1, MPI_INT, workerPid, 0, MPI_COMM_WORLD);

			activeWorkers++;
			printf("Master started worker number %d, type = %d, on MPI process %d\n", i , worker_type, workerPid);
		}
		int masterStatus = masterPoll();
		while (masterStatus) {
			masterStatus=masterPoll();
			for (i=0;i<max_proc_num;i++) {
				// Checks all outstanding workers that master spawned to see if they have completed
				if (initialWorkerRequests[i] != MPI_REQUEST_NULL) {
					MPI_Test(&initialWorkerRequests[i], &returnCode, MPI_STATUS_IGNORE);
					if (returnCode) activeWorkers--;
				}
			}
			// If we have no more active workers then quit poll loop which will effectively shut the pool down when  processPoolFinalise is called
			if (activeWorkers==0) break;
		}
	}
	// Finalizes the process pool, call this before closing down MPI
	processPoolFinalise();
	// Finalize MPI, ensure you have closed the process pool first
	MPI_Finalize();
	return 0;
}

static void squirrel_worker_code(int myRank, int parentId) {
	// spawn 1 squirrel if first child
	if(parentId == MASTER_ID) {
		int childPid = startWorkerProcess();
		printf("Worker on process %d, started child processes %d\n", myRank, childPid);
		MPI_Request childRequests;
		MPI_Irecv(NULL, 0, MPI_INT, childPid, 0, MPI_COMM_WORLD, &childRequests);

		int worker_type = SQUIRREL;
		MPI_Send(&worker_type, 1, MPI_INT, childPid, 0, MPI_COMM_WORLD);

		MPI_Waitall(1, &childRequests, MPI_STATUS_IGNORE);
	}

}

static void cell_worker_code() {
}

static void clock_worker_code() {
	// initialize clock
}

static int kill(int parentId) {
	MPI_Send(NULL, 0, MPI_INT, parentId, 0, MPI_COMM_WORLD);
	return workerSleep();
}

static void workerCode() {
	int workerStatus = 1;
	while (workerStatus) {
		int myRank, parentId, worker_type=-1;
		MPI_Status status;

		MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
		parentId = getCommandData();	// We encode the parent ID in the wake up command data

		MPI_Recv(&worker_type, 1, MPI_INT, parentId, 0, MPI_COMM_WORLD, &status);

		printf("Child worker of type %d on process %d started with parent %d\n", worker_type, myRank, parentId);


		if(worker_type == SQUIRREL)
		{
			squirrel_worker_code(myRank, parentId);
		}
		else if(worker_type == CELL)
		{
			cell_worker_code();
		}
		else if(worker_type == CLOCK)
		{
			clock_worker_code();
		}

		workerStatus = kill(parentId);

	}
}
