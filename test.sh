# Exam number: B136013

module load mpt
module load intel-compilers-17

make clean;

make actor_framework;

make simulation;

LD_LIBRARY_PATH=./actor_framework:$LD_LIBRARY_PATH

SQUIRRELS=34
INFECTION_LEVEL=4
MAX_MONTHS=24
MAX_SQUIRRELS_NUM=200

PROC_NUM=5

#Every test: the sum of the number of spawned and died squirrels in each worker equals to the number of spawned and died squirrels the master has notified

#Scenario 1: If there are not infected squirrels no one dies and the population is constantly increasing
echo "Start Scenario 1"
	INFECTION_LEVEL=0
	mpirun -n $PROC_NUM ./simulation/simulation $SQUIRRELS $INFECTION_LEVEL $MAX_MONTHS $MAX_SQUIRRELS_NUM

echo "Start Scenario 2"
#Scenario 2: If there are not infected squirrels and the the number of actors is close to 200 then the simulation will probably terminate before 24 months
	INFECTION_LEVEL=0
	SQUIRRELS=170
	mpirun -n $PROC_NUM ./simulation/simulation $SQUIRRELS $INFECTION_LEVEL $MAX_MONTHS $MAX_SQUIRRELS_NUM

echo "Start Scenario 3"
#Scenario 3: If all squirrels are infected the simulation will probably terminate in early stage
	INFECTION_LEVEL=5
	SQUIRRELS=5
	mpirun -n $PROC_NUM ./simulation/simulation $SQUIRRELS $INFECTION_LEVEL $MAX_MONTHS $MAX_SQUIRRELS_NUM

echo "Start Scenario 4"
#Scenario 4: running on 1 process will terminate
	PROC_NUM=1
	mpirun -n $PROC_NUM ./simulation/simulation $SQUIRRELS $INFECTION_LEVEL $MAX_MONTHS $MAX_SQUIRRELS_NUM

echo "Start Scenario 5"
#Scenario 5: running for 48 months
	PROC_NUM=5
	SQUIRRELS=34
	INFECTION_LEVEL=4
	MAX_MONTHS=48
	mpirun -n $PROC_NUM ./simulation/simulation $SQUIRRELS $INFECTION_LEVEL $MAX_MONTHS $MAX_SQUIRRELS_NUM

echo "Start Scenario 6"
#Scenario 6: running of 36 processes
	PROC_NUM=36
	MAX_MONTHS=12
	SQUIRRELS=7
	INFECTION_LEVEL=4

	mpirun -n $PROC_NUM ./simulation/simulation $SQUIRRELS $INFECTION_LEVEL $MAX_MONTHS $MAX_SQUIRRELS_NUM
