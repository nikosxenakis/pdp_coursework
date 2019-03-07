module load mpt;
module load intel-compilers-17;

make clean;
# make actor_framework;
make parapoxvirus_simulation;
# ./parapoxvirus_simulation/parapoxvirus_simulation

SQUIRRELS=34
CELLS=16
CLOCKS=1
MAX_SQUIRRELS=200
INFECTION_LEVEL=4
MAX_MONTHS=24





# TEST
SQUIRRELS=2
MAX_MONTHS=5
CELLS=0
MAX_SQUIRRELS=10




PROC_NUM=20
WORKERS_NUM=10

mpirun -n $PROC_NUM ./parapoxvirus_simulation/parapoxvirus_simulation $CELLS $SQUIRRELS $INFECTION_LEVEL $MAX_MONTHS $WORKERS_NUM


# mpicxx -o ./parapoxvirus_simulation/parapoxvirus_simulation ./parapoxvirus_simulation/worker.o ./parapoxvirus_simulation/master.o ./parapoxvirus_simulation/actor.o ./parapoxvirus_simulation/input_data.o