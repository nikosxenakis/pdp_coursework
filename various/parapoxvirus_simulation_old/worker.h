#ifndef WORKER_H
#define WORKER_H

#include <stdio.h>
#include <mpi.h>

typedef struct Worker {
	int pid;
	MPI_Request workerRequest;
} Worker;

static void check_messages();
static void process_message();
static void compute();

#endif
