module load mpt
module load intel-compilers-17

make clean;

make actor_framework;
make simulation;

SQUIRRELS=34
CELLS=16
CLOCKS=1
INFECTION_LEVEL=4
MAX_MONTHS=24
MAX_ACTORS_NUM=200

PROC_NUM=5

LD_LIBRARY_PATH=./actor_framework:$LD_LIBRARY_PATH

mpirun -n $PROC_NUM ./simulation/simulation $CLOCKS $CELLS $SQUIRRELS $INFECTION_LEVEL $MAX_MONTHS $MAX_ACTORS_NUM $PROC_NUM

# python plot.py
