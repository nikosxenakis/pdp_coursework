module load mpt;
module load intel-compilers-17;

# make clean;

# make actor_framework;
make parapoxvirus_simulation;
# ./parapoxvirus_simulation/parapoxvirus_simulation

SQUIRRELS=34
CELLS=16
CLOCKS=1
INFECTION_LEVEL=4
MAX_MONTHS=24
MAX_ACTORS_NUM=200




# TEST
SQUIRRELS=30
MAX_MONTHS=5
CELLS=0
CLOCKS=0
MAX_ACTORS_NUM=1000


PROC_NUM=4

mpirun -n $PROC_NUM ./parapoxvirus_simulation/parapoxvirus_simulation $CLOCKS $CELLS $SQUIRRELS $INFECTION_LEVEL $MAX_MONTHS $MAX_ACTORS_NUM


# mpicxx -o ./parapoxvirus_simulation/parapoxvirus_simulation ./parapoxvirus_simulation/worker.o ./parapoxvirus_simulation/master.o ./parapoxvirus_simulation/actor.o ./parapoxvirus_simulation/input_data.o