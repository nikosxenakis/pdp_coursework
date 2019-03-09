#include "messenger.h"

MPI_Datatype Messenger::Message_type;

void Messenger::init_types() {

    MPI_Datatype type[5] = { MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT };
    int blocklen[5] = { 1, 1, 1, 1, 1 };
    MPI_Aint disp[5];

    disp[0] = 0;
    disp[1] = sizeof(int);
    disp[2] = 2*sizeof(int);
    disp[3] = 3*sizeof(int);
    disp[4] = 4*sizeof(int);

    MPI_Type_create_struct(5, blocklen, disp, type, &Messenger::Message_type);
    MPI_Type_commit(&Messenger::Message_type);
}

void Messenger::send_message(int worker_pid, Message message) {
	MPI_Bsend(&message, 1, Messenger::Message_type, worker_pid, 0, MPI_COMM_WORLD);
}

void Messenger::send_blocking_message(int worker_pid, Message message) {
	MPI_Send(&message, 1, Messenger::Message_type, worker_pid, 0, MPI_COMM_WORLD);
}

Message Messenger::receive_message(int source_pid) {
	Message message = Message();
	MPI_Recv(&message, 1, Messenger::Message_type, source_pid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	return message;
}
