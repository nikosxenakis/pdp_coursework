# Parallel Design Patterns Coursework
Red squirrels are in decline in the UK and as such there is considerable effort to save them, however a disease called squirrel parapoxvirus is especially perilous to red squirrels and can often kill them, wiping out entire habitats. This application simulates the lifes of squirrels. The program is separated in two solutions. One is the actor framework and the other the simulation.

# Exam number: B136013

## Prerequisites

* Compiler: [mpicxx](https://www.mpich.org/static/docs/v3.1/www1/mpicxx.html)
* Build tool: [GNU Make](https://www.gnu.org/software/make/)

## Building & Running

### Frontend

To build the both projects and run the executable in the frontend execute the following script:

```
./run.sh
```

### Backend

To build the both projects and submit the executable in the backend execute the following script:

```
./run_backend.sh
```

## Testing

To test the simulation's behaviour run:

```
./test.sh
```

This script will run a number of behavioural scenarios, these are:

1. Scenario 1: If there are not infected squirrels no one dies and the population is constantly increasing
2. Scenario 2: If there are not infected squirrels and the the number of actors is close to 200 then the simulation will probably terminate before 24 months
3. Scenario 3: If all squirrels are infected the simulation will terminate in early stage
4. Scenario 4: running on 1 process will terminate
5. Scenario 5: running for 48 months

## Output

The output of the simulation is displayed in 5 files:

1. Standard Output: it contains information about the initialisation and finalisation of the simulation and during each months prints the number of alive and infected squirrels
2. data/population_influx.tsv: contains for each simulated month of the simulation the population influx for each cell
3. data/infection_level.tsv: contains for each simulated month of the simulation the infection level for each cell
4. data/alive_squirrels.tsv: contains for each simulated month of the number of alive squirrels
5. data/infected_squirrels.tsv: contains for each simulated month of the number of infected squirrels

To visualise the data run:

```
python plot.py
```

This command will create four eps files in data/ folder. These figures represent:

1. Population influx for each cell across all months
2. Infection level for each cell across all months
3. Number of alive squirrels for each month
4. Number of infected squirrels for each month

## Documentation

C++ header files contain in line documentation
We used [DoxyGen](https://github.com/doxygen/doxygen)
To auto-generate a website-documentation run

```
./documentation.sh
```
You can check by opening with your browser the documentation/index.html file the project directory

## Version Control

We use [GitHub](http://github.com/) for Version Control. For the versions available, see the [tags on this repository](https://github.com/nikosxenakis/pdp_coursework).
Please send request to the owner for further investigation.

## Acknowledgements

* Thanks to anyone whose code was used, and to EPCC
