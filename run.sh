module load mpt
module load intel-compilers-17

rm ./simulation/actor_framework.so
rm ./simulation/actor.h
rm ./simulation/actor_framework.h
rm ./simulation/ran2.h
rm ./simulation/message.h

make clean;

make actor_framework;

cp ./actor_framework/actor_framework.so ./simulation/
cp ./actor_framework/*.h ./simulation/

make simulation;

SQUIRRELS=34
CELLS=16
CLOCKS=1
INFECTION_LEVEL=4
MAX_MONTHS=24
MAX_ACTORS_NUM=200

PROC_NUM=5

LD_LIBRARY_PATH=./simulation:$LD_LIBRARY_PATH

mpirun -n $PROC_NUM ./simulation/simulation $CLOCKS $CELLS $SQUIRRELS $INFECTION_LEVEL $MAX_MONTHS $MAX_ACTORS_NUM $PROC_NUM


# mpicxx -o ./parapoxvirus_simulation/parapoxvirus_simulation ./parapoxvirus_simulation/worker.o ./parapoxvirus_simulation/master.o ./parapoxvirus_simulation/actor.o ./parapoxvirus_simulation/input_data.o