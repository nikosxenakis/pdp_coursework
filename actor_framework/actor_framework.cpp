#include "actor_framework.h"

void (*Actor_framework::init_actors)(Message message);

void Actor_framework::worker_code(int pid, Message message) {
	int world_size, workers_num;

	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	workers_num = world_size - 1;
	Worker *worker = new Worker(pid, MASTER_PID, message.get(INIT_ACTORS_NUM), workers_num);
	worker->run();
	worker->finalize();
	delete worker;
}

void Actor_framework::master_code(int pid, Message message) {
	int world_size, workers_num;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	workers_num = world_size - 1;
	Master::initialize_master(pid, workers_num);

	vector<int> workers_pid;

    for (int i = 1; i < world_size; ++i) {
		workers_pid.push_back(i);
		cout << "Worker started on MPI process " << i << endl;
	}

	Master::init_workers(workers_pid);

	message.set(WORKERS_NUM, workers_num);
	Actor_framework::init_actors(message);

	Master::run();

	Master::finalize();
}

void Actor_framework::spawn_actor(Message message) {
	Master::spawn_actor(message);
}

void Actor_framework::register_init_actors(void (init_actors)(Message message)) {
	Actor_framework::init_actors = init_actors;
}

void Actor_framework::register_spawn_actor(Actor* (spawn_actor)(Message message), Message message) {
	Worker::register_spawn_actor(spawn_actor, message);
}

void Actor_framework::actor_framework(Message message) {
	int pid;
	int bsize;
	char *buffer, *bbuffer;

	MPI_Init(NULL, NULL);

	Messenger::init_types();

	MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    MPI_Pack_size( UPPER_BOUND_BUFFER_SIZE, MPI_BYTE, MPI_COMM_WORLD, &bsize );
	buffer = (char *)malloc( bsize );
	MPI_Buffer_attach( buffer, bsize );

	if (pid == MASTER_PID) {
		Actor_framework::master_code(pid, message);
	}
	else {
		Actor_framework::worker_code(pid, message);
	}

	MPI_Buffer_detach( &bbuffer, &bsize );

	MPI_Finalize();
}
