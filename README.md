# Parallel Design Patterns Coursework
description

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

## Output

The output of the simulation is displayed in 3 files:

1. Standard Output: it contains information about the initialisation and finalisation of the simulation and during each months prints the number of alive and infected squirrels
2. data/population_influx.tsv: contains for each simulated month of the simulation the population influx for each cell
3. data/infection_level.tsv: contains for each simulated month of the simulation the infection level for each cell

To visualise the data of population_influx and infection_level run:

```
python plot.py
```

This command will create two eps files in data/ folder

## Documentation

C++ header files contain in line documentation

## Version Control

We use [GitHub](http://github.com/) for Version Control. For the versions available, see the [tags on this repository](https://github.com/nikosxenakis/pdp_coursework).

## Acknowledgements

* Thanks to anyone whose code was used, and to EPCC