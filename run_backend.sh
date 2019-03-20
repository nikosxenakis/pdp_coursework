module load mpt
module load intel-compilers-17

make clean;

make actor_framework;
make simulation;

qsub simulation.pbs
