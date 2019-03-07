#ifndef MASTER_H
#define MASTER_H

#include <stdio.h>
#include <mpi.h>
#include "worker.h"
#include "pool.h"

Worker* init_workers(int workers_num);
void finalize();

void register_actor();
void spawn_actor();
void kill_actor();

#endif
