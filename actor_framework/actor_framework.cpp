#include "actor_framework.h"

void (*Actor_framework::init_actors)(Message message);

void Actor_framework::worker_code(int pid) {
	Worker *worker = new Worker(pid);
	worker->run();
	worker->finalize();
	delete worker;
}

void Actor_framework::master_code(Message message) {
	int world_size, workers_num;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	workers_num = world_size - 1;
	Master::set_workers_num(workers_num);
	vector<int> workers_pid;

    for (int i = 1; i < world_size; ++i) {
		workers_pid.push_back(i);
		cout << "Worker started on MPI process " << i << endl;
	}

	message.set(WORKERS_NUM, workers_num);
	Actor_framework::init_actors(message);
	Master::run();
	Master::finalize();
}

void Actor_framework::register_init_actors(void (init_actors)(Message message)) {
	Actor_framework::init_actors = init_actors;
}

void Actor_framework::register_spawn_actor(Actor* (spawn_actor)(Message message)) {
	Worker::register_spawn_actor(spawn_actor);
}

void Actor_framework::spawn_actor(Message message) {
	Master::spawn_actor(message);
}

void Actor_framework::actor_framework(Message message) {
	int pid, world_size;
	int bsize;
	char *buffer, *bbuffer;

	MPI_Init(NULL, NULL);
	Messenger::init_types();
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	if(world_size < 2) {
		cerr << "The simulation must run on more that 1 process\n";
	}
	else {
	    MPI_Pack_size( UPPER_BOUND_BUFFER_SIZE, MPI_BYTE, MPI_COMM_WORLD, &bsize );
		buffer = (char *) malloc( bsize );
		MPI_Buffer_attach( buffer, bsize );

		if (pid == MASTER_PID)
			Actor_framework::master_code(message);
		else
			Actor_framework::worker_code(pid);

		MPI_Buffer_detach( &bbuffer, &bsize );
	}

	MPI_Finalize();
}
