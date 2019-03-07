#include "master.h"

Worker* init_workers(int workers_num) {
	Worker *workers = calloc(workers_num, sizeof(Worker));
	int i;

	for (i=0; i<workers_num; i++) {
		workers[i].pid = startWorkerProcess();
		MPI_Irecv(NULL, 0, MPI_INT, workers[i].pid, 0, MPI_COMM_WORLD, &workers[i].workerRequest);
		printf("Master started worker on MPI process %d\n", workers[i].pid);
	}

	return workers;
}


