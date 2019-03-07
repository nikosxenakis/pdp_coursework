
#define DEATH_CHANNEL 111
#define BIRTH_CHANNEL 112
#define SIMULATION_CHANNEL 113

#define INIT_TAG 1000
#define SIMULATION_TAG 1001
#define DEATH_TAG 1002
#define BIRTH_TAG 1003



typedef struct Actor_data {
	int pid;
	int type;
	int alive;
	MPI_Request workerRequest;
} Actor_data;

static int check_inbox() {
	MPI_Status status;
	int flag=0;
	MPI_Iprobe(MPI_ANY_SOURCE, SIMULATION_TAG, MPI_COMM_WORLD, &flag, &status);
	return flag;
}

static void print_actors(Actor_data *actor_data_array, int active_actors) {
	int i;
	for (i = 0; i < active_actors; ++i)
	{
		printf("Actor %d: type=%d\n", actor_data_array[i].pid, actor_data_array[i].type);
	}
	printf("\n");
}

typedef enum Squirrel_state{SQUIRREL_STATE_INIT=0, SQUIRREL_STATE_SIMULATION_WAIT, SQUIRREL_STATE_SIMULATION, SQUIRREL_STATE_SIMULATION_END, SQUIRREL_STATE_GIVE_BIRTH} Squirrel_state;

static void squirrel_simulation(int my_id, int master_id) {
	Squirrel_state state = SQUIRREL_STATE_INIT;
	int type, max_months;
	MPI_Status status;
	int source, month, wantToDie=0, wantToGiveBirth=0;
	int outstanding=0, tag;
	int workerStatus=1;

	do
	{

		MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &outstanding, &status);
		tag=status.MPI_TAG;

		if(outstanding && (tag == SIMULATION_TAG || tag == DEATH_TAG || tag == BIRTH_TAG || tag == INIT_TAG))
		{
			source=status.MPI_SOURCE;

			if(state == SQUIRREL_STATE_INIT) {
				MPI_Recv(&type, 1, MPI_INT, master_id, INIT_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Recv(&max_months, 1, MPI_INT, master_id, INIT_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				state = SQUIRREL_STATE_SIMULATION_WAIT;
			}
			else if(state == SQUIRREL_STATE_SIMULATION_WAIT) {
				if(tag == SIMULATION_TAG) {
					MPI_Recv(&month, 1, MPI_INT, master_id, SIMULATION_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					state=SQUIRREL_STATE_SIMULATION;			
				}
				else if(tag == DEATH_TAG) {
					MPI_Recv(NULL, 0, MPI_INT, master_id, DEATH_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					state=SQUIRREL_STATE_SIMULATION_END;			
				}
				else if(tag == BIRTH_TAG) {
					// MPI_Recv(NULL, 0, MPI_INT, master_id, BIRTH_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					// state=SQUIRREL_STATE_SIMULATION_WAIT;			
				}
			}
			// else if(state == SQUIRREL_STATE_GIVE_BIRTH) {
			// 	MPI_Recv(NULL, 0, MPI_INT, master_id, BIRTH_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// 	MPI_Bsend(NULL, 0, MPI_INT, master_id, SIMULATION_TAG, MPI_COMM_WORLD);
			// 	if(month==max_months) {
			// 		state=SQUIRREL_STATE_SIMULATION_END;
			// 	}
			// 	else {
			// 		state=SQUIRREL_STATE_SIMULATION_WAIT;
			// 	}
			// }

		}
		else
		{
			if(state == SQUIRREL_STATE_INIT) {
				// printf("SQUIRREL_STATE_INIT\n");
			}
			else if(state == SQUIRREL_STATE_SIMULATION_WAIT) {
				// printf("SQUIRREL_STATE_SIMULATION_WAIT %d\n", month);
			}
			else if(state == SQUIRREL_STATE_SIMULATION) {
				// printf("SQUIRREL_STATE_SIMULATION %d\n", month);
				// calculate
				if(my_id == 3 && month == 2) {
					wantToDie=1;
				}
				// if(my_id == 4 && month == 3) {
				// 	wantToDie=1;
				// }
				// if(my_id == 5 && month == 4) {
				// 	wantToDie=1;
				// }
				// if(my_id == 6 && month == 4) {
				// 	wantToDie=1;
				// }

				// if(my_id == 7 && month == 10) {
				// 	wantToDie=1;
				// }
				// 				if(my_id == 6 && month == 2) {
				// 	wantToGiveBirth=1;
				// }

				if(wantToDie) {
					MPI_Bsend(NULL, 0, MPI_INT, master_id, DEATH_TAG, MPI_COMM_WORLD);
					state=SQUIRREL_STATE_SIMULATION_WAIT;
				}
				// else if(wantToGiveBirth) {
				// 	MPI_Bsend(NULL, 0, MPI_INT, master_id, BIRTH_TAG, MPI_COMM_WORLD);
				// 	wantToGiveBirth=0;
				// 	state=SQUIRREL_STATE_SIMULATION_WAIT;
				// }
				else {
					MPI_Bsend(NULL, 0, MPI_INT, master_id, SIMULATION_TAG, MPI_COMM_WORLD);
					state=SQUIRREL_STATE_SIMULATION_WAIT;
				}
			}
			// else if(state == SQUIRREL_STATE_GIVE_BIRTH) {

			// }
			else if(state == SQUIRREL_STATE_SIMULATION_END) {
				printf("SQUIRREL_STATE_SIMULATION_END\n");
				break;
			}


		}
	} while(1);

}

int is_squirrel(pid) {
	return 1;
}

typedef enum Clock_state{CLOCK_STATE_INIT=0, CLOCK_STATE_SIMULATION, CLOCK_STATE_SIMULATION_WAIT, CLOCK_STATE_SIMULATION_END} Clock_state;

static void clock_simulation(Actor_data *actor_data_array, int active_actors, int max_actors, int max_months) {

	Clock_state state = CLOCK_STATE_INIT;
	int i,month=1,data;
	MPI_Status status;
	int source, tag;
	int outstanding=0;
	int finished_actors, pending_actors=0;

	do
	{
		MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &outstanding, &status);
		tag=status.MPI_TAG;
		if(outstanding && (tag == SIMULATION_TAG || tag == DEATH_TAG || tag == BIRTH_TAG || tag == INIT_TAG))
		{

			source=status.MPI_SOURCE;

			if(state == CLOCK_STATE_SIMULATION_WAIT) {

				if(tag == SIMULATION_TAG) {
					MPI_Recv(NULL, 0, MPI_INT, source, SIMULATION_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					printf("received from %d\n", source);
					finished_actors++;

					printf("%d %d\n", finished_actors,active_actors );

					if(finished_actors==active_actors) {
						if(month==max_months) {
							state=CLOCK_STATE_SIMULATION_END;
						}
						else {
							month++;
							state=CLOCK_STATE_SIMULATION;
						}
					}					
				}
				else if(tag == DEATH_TAG) {
					MPI_Recv(NULL, 0, MPI_INT, source, DEATH_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					for (i = 0; i < active_actors; ++i)
					{
						if(actor_data_array[i].pid == source) {
							break;
						}
					}

					active_actors--;
					actor_data_array[i].pid = actor_data_array[active_actors].pid;
					actor_data_array[i].type = actor_data_array[active_actors].type;
					actor_data_array[i].alive = actor_data_array[active_actors].alive;

					actor_data_array[active_actors].alive = 0;

					MPI_Bsend(NULL, 0, MPI_INT, source, DEATH_TAG, MPI_COMM_WORLD);
	
				}
				// else if(tag == BIRTH_TAG) {
				// 	MPI_Recv(NULL, 0, MPI_INT, source, BIRTH_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				// 	finished_actors++;
				// 	int new_pid = startWorkerProcess();
				// 	actor_data_array[active_actors].pid=new_pid;
				// 	actor_data_array[active_actors].alive=1;
				// 	MPI_Bsend( &actor_data_array[i].type, 1, MPI_INT, new_pid, INIT_TAG, MPI_COMM_WORLD );
				// 	MPI_Bsend( &max_months, 1, MPI_INT, new_pid, INIT_TAG, MPI_COMM_WORLD );
				// 	active_actors++;
				// 	finished_actors++;
				// }
			}

		}
		else
		{
			if(state == CLOCK_STATE_INIT) {
				printf("CLOCK_STATE_INIT\n");
				for (i=0; i<active_actors; i++) {
					// Send type
	    			MPI_Bsend( &actor_data_array[i].type, 1, MPI_INT, actor_data_array[i].pid, INIT_TAG, MPI_COMM_WORLD );
					// Send max_months
	    			MPI_Bsend( &max_months, 1, MPI_INT, actor_data_array[i].pid, INIT_TAG, MPI_COMM_WORLD );
				}
				state=CLOCK_STATE_SIMULATION;
			}
			else if(state == CLOCK_STATE_SIMULATION) {
				printf("CLOCK_STATE_SIMULATION %d\n", month);
				char c;
				print_actors(actor_data_array, active_actors);

				scanf("%c\n",&c);

				for (i=0; i<active_actors; i++) {
					MPI_Bsend(&month, 1, MPI_INT, actor_data_array[i].pid, SIMULATION_TAG, MPI_COMM_WORLD);
				}
				finished_actors=0;
				state=CLOCK_STATE_SIMULATION_WAIT;
			}
			else if(state == CLOCK_STATE_SIMULATION_WAIT) {
				// printf("CLOCK_STATE_SIMULATION_WAIT %d\n", month);
			}
			else if(state == CLOCK_STATE_SIMULATION_END) {
				for (i=0; i<active_actors; i++) {
					MPI_Bsend(NULL, 0, MPI_INT, actor_data_array[i].pid, DEATH_TAG, MPI_COMM_WORLD);
				}
				printf("CLOCK_STATE_SIMULATION_END\n");
				break;
			}
		}

	} while(1);


	//start simulation
	// for (timestep = 0; timestep < max_months; ++timestep) {

	// 	for (i = 0; i < max_actors; ++i)
	// 	{
	// 		if(actor_data_array[i].alive) {
	// 			MPI_Send(NULL, 0, MPI_INT, actor_data_array[i].pid, SIMULATION_CHANNEL, MPI_COMM_WORLD);
	// 		}
	// 	}

	// 	// WAIT
	// 	printf("Master: waits in timestep %d\n", timestep);

	// 	for (i = 0; i < max_actors; ++i)
	// 	{
	// 		if(actor_data_array[i].alive) {
	// 			MPI_Recv(NULL, 0, MPI_INT, actor_data_array[i].pid, SIMULATION_CHANNEL, MPI_COMM_WORLD, &simulationStatus[i]);
	// 		}
	// 	}

	// 	check_inbox()
	// 	check_messages(actor_data_array, &active_actors, max_actors);
	// }

	// end
	// for (int i = 0; i < max_actors; ++i)
	// {
	// 	if(actor_data_array[i].alive) {
	// 		MPI_Isend(NULL, 0, MPI_INT, actor_data_array[i].pid, SIMULATION_CHANNEL, MPI_COMM_WORLD, &simulationRequest[i]);
	// 	}
	// }

	// MPI_Waitall(max_actors, simulationRequest, simulationStatus);

	// masterStatus = masterPoll();

	// while (masterStatus) {
	// // 	// check messages
	// 	check_messages(actor_data_array, &active_actors, max_actors);

	// 	if (active_actors==0) break;
	// }

}
