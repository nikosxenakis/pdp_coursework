module load mpt;
module load intel-compilers-17;

make clean;
# make actor_framework;
make parapoxvirus_simulation;
# ./parapoxvirus_simulation/parapoxvirus_simulation

SQUIRRELS=34
CELLS=16
MAX_SQUIRRELS=200
INFECTION_LEVEL=4
MAX_MONTHS=24

MAX_PROC_NUM=$(($MAX_SQUIRRELS + $CELLS + 1))

mpirun -n $MAX_PROC_NUM ./parapoxvirus_simulation/parapoxvirus_simulation $CELLS $SQUIRRELS $INFECTION_LEVEL $MAX_MONTHS