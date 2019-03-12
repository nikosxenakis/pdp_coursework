module load mpt
module load intel-compilers-17

# make clean;
# make actor_framework;
make parapoxvirus_simulation

SQUIRRELS=34
CELLS=16
CLOCKS=1
INFECTION_LEVEL=4
MAX_MONTHS=24
MAX_ACTORS_NUM=200

PROC_NUM=5

mpirun -n $PROC_NUM ./actor_framework/parapoxvirus_simulation $CLOCKS $CELLS $SQUIRRELS $INFECTION_LEVEL $MAX_MONTHS $MAX_ACTORS_NUM $PROC_NUM


# mpicxx -o ./parapoxvirus_simulation/parapoxvirus_simulation ./parapoxvirus_simulation/worker.o ./parapoxvirus_simulation/master.o ./parapoxvirus_simulation/actor.o ./parapoxvirus_simulation/input_data.o