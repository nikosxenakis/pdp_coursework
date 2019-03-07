typedef struct Actor_data {
	int type;
} Actor_data;

static void workerCode(int argc, char* argv[]) {
	int worker_status = 1, actor_type, outstanding=0, num_of_actors=0;
	Actor_data actors_data[100];

	Input_data* input_data = init_input_data(argc, argv);

	while (worker_status) {
		int my_id, master_id;
		MPI_Status status;

		MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
		master_id = getCommandData();

		MPI_Recv(NULL, 0, MPI_INT, master_id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// do {
		// 	MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &outstanding, &status);
		// 	if(outstanding) {
		// 		MPI_Recv(&actor_type, 1, MPI_INT, master_id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// 		actors_data[num_of_actors].type = actor_type;
		// 		num_of_actors++;
		// 	}
		// 	else {
		// 		num_of_actors--;
		// 		if(num_of_actors==0)
		// 			MPI_Send(NULL, 0, MPI_INT, master_id, 0, MPI_COMM_WORLD);
		// 	}

		// } while(1);

		// squirrel_simulation(my_id, master_id);
		// clock_simulation(actor_data_array, active_actors, max_actors, max_months);

		// MPI_Recv(&worker_type, 1, MPI_INT, master_id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// printf("Actor %d started, type = %d\n", my_id, worker_type);

		// printf("Worker started, pid = %d\n", my_id);
	MPI_Send(NULL, 0, MPI_INT, master_id, 0, MPI_COMM_WORLD);

		worker_status=workerSleep();
	}
}

typedef struct Worker_data {
	int pid;
	MPI_Request worker_request;
} Worker_data;


static void masterCode(int argc, char* argv[]) {
	int i, returnCode, max_workers, active_workers;
	int cells_deployed=0, squirells_deployed=0, clocks_deployed=0;
	Worker_data *workers_data;

	Input_data* input_data = init_input_data(argc, argv);
	print_input_data(input_data);

	max_workers = input_data->cells + MAX_SQUIRRELS + 1;
	active_workers = input_data->cells + input_data->squirells + 1;
	// active_workers = cells + 210 + 1;

	workers_data = (Worker_data*)calloc(max_workers, sizeof(Worker_data)); 

	for (i=0; i<active_workers; i++) {
		int actor_type;
		Worker_data *worker_data;
			
		worker_data = &workers_data[i];
		worker_data->pid = startWorkerProcess();
		MPI_Irecv(NULL, 0, MPI_INT, worker_data->pid, 0, MPI_COMM_WORLD, &worker_data->worker_request);

		printf("Master: spawned worker at process with pid = %d\n", worker_data->pid);

		//init actor
		// send init data
		if(squirells_deployed < input_data->squirells) {
			actor_type = SQUIRREL;
			squirells_deployed++;
		}
		else if(cells_deployed < input_data->cells) {
			actor_type = CELL;
			cells_deployed++;
		}
		else if(clocks_deployed < 1) {
			actor_type = CLOCK;
			clocks_deployed++;
		}

	}

	for (i=0; i<active_workers; i++) {
		MPI_Send(NULL, 0, MPI_INT, workers_data[i].pid, 0, MPI_COMM_WORLD);
	}


	int masterStatus = masterPoll();
	while (masterStatus) {
		masterStatus=masterPoll();
		for (i=0; i<max_workers; i++) {
			if (workers_data[i].worker_request != MPI_REQUEST_NULL) {
				MPI_Test(&workers_data[i].worker_request, &returnCode, MPI_STATUS_IGNORE);
				if (returnCode) active_workers--;
			}
		}
		if (active_workers==0) break;
	}

}