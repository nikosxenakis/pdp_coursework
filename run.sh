module load mpt
module load intel-compilers-17

make clean;

make actor_framework;
make simulation;

SQUIRRELS=34
INFECTION_LEVEL=4
MAX_MONTHS=24
MAX_SQUIRRELS_NUM=200

PROC_NUM=5

LD_LIBRARY_PATH=./actor_framework:$LD_LIBRARY_PATH

mpirun -n $PROC_NUM ./simulation/simulation $SQUIRRELS $INFECTION_LEVEL $MAX_MONTHS $MAX_SQUIRRELS_NUM
