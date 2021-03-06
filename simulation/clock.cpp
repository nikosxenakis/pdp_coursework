#include "clock.h"

stringstream Clock::population_influx_stream;
stringstream Clock::infection_level_stream;
stringstream Clock::alive_squirrels_stream;
stringstream Clock::infected_squirrels_stream;

/**
 * @brief in month computation when clock is waiting for month to complete
 * @param actor Pointer to this Actor
 */
static void compute_in_month(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	milliseconds curr_time = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

	if((curr_time - clock->begin_time).count() > TIMESTEP_DURATION) {
		clock->begin_time = curr_time;
		clock->set_state(END_OF_MONTH);
		Message message;
		message.set(COMMAND, TIMESTEP_END);
		for (int cell_id = 0; cell_id < CELL_NUM; ++cell_id) {
			clock->send_msg(cell_id, message);
		}
	}
}

/**
 * @brief in the end of month computation where clock waits cells to respond
 * @param actor Pointer to this Actor
 */
static void compute_end_of_month(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);

	if(clock->cells_ready == CELL_NUM) {
		clock->write_output_stream();
		cout << "Month " << clock->month << ": alive_squirrels = " << clock->alive_squirrels << ", infected_squirrels = " << clock->infected_squirrels << endl;
		if(clock->month >= clock->max_months || clock->alive_squirrels == 0 || clock->alive_squirrels > 200) {
			if(clock->alive_squirrels == 0)
				cout << "Simulation termination: all squirrels died\n";
			if(clock->alive_squirrels > 200)
				cout << "Simulation termination: squirrels exceeded 200\n";
			clock->set_state(FINISH);
			clock->write_output_files();
			clock->kill_all();
		}
		else {
			clock->cells_ready = 0;
			clock->month++;
			clock->begin_time = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
			clock->set_state(IN_MONTH);
		}
	}
}

/**
 * @brief parses messages for squirrels updates
 * @param actor Pointer to this Actor
 * @param message Input message
 */
static void parse_message_in_month(Actor *actor, Message message) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	if(message.get(COMMAND) == SPAWN_SQUIRREL_COMMAND) {
		clock->alive_squirrels++;
	}
	else if(message.get(COMMAND) == KILL_SQUIRREL_COMMAND) {
		clock->alive_squirrels--;
		clock->infected_squirrels--;
	}
	else if(message.get(COMMAND) == INFECTED_SQUIRREL_COMMAND) {
		clock->infected_squirrels++;
	}
	else
		assert(0);
}

/**
 * @brief parses messages for squirrels and cell updates
 * @param actor Pointer to this Actor
 * @param message Input message
 */
static void parse_message_end_of_month(Actor *actor, Message message) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	if(message.get(COMMAND) == TIMESTEP_END) {
		clock->population_influx[clock->cells_ready] = message.get(POPULATION_INFLUX);
		clock->infection_level[clock->cells_ready] = message.get(INFECTION_LEVEL);
		clock->cells_ready++;
	}
	else if(message.get(COMMAND) == SPAWN_SQUIRREL_COMMAND) {
		clock->alive_squirrels++;
	}
	else if(message.get(COMMAND) == KILL_SQUIRREL_COMMAND) {
		clock->alive_squirrels--;
		clock->infected_squirrels--;
	}
	else if(message.get(COMMAND) == INFECTED_SQUIRREL_COMMAND) {
		clock->infected_squirrels++;
	}	
	else
		assert(0);
}

void Clock::write_output_stream() {

	if(this->month == 1) {
  		Clock::population_influx_stream << "month";
  		Clock::infection_level_stream << "month";
  		Clock::alive_squirrels_stream << "month";
  		Clock::infected_squirrels_stream << "month";

  		for (int i = 0; i < CELL_NUM; ++i) {
  			Clock::population_influx_stream << "\tCell " << i;
  			Clock::infection_level_stream << "\tCell " << i;
  		}
  		Clock::population_influx_stream << "\n";
  		Clock::infection_level_stream << "\n";
  		Clock::alive_squirrels_stream << "\tSquirrels Number\n";
  		Clock::infected_squirrels_stream << "\tSquirrels Number\n";
	}

	Clock::population_influx_stream << this->month;
	Clock::infection_level_stream << this->month;

	for(auto population_influx: this->population_influx) {
		this->population_influx_stream << "\t" << population_influx;
	}
	for(auto infection_level: this->infection_level) {
		Clock::infection_level_stream << "\t" << infection_level;
	}

	Clock::population_influx_stream << endl;
	Clock::infection_level_stream << endl;

	Clock::alive_squirrels_stream << this->month << "\t" << this->alive_squirrels << endl;
	Clock::infected_squirrels_stream << this->month << "\t" << this->infected_squirrels << endl;
}

void Clock::write_output_files() {
	ofstream population_influx_file, infection_level_file, alive_squirrels_file, infected_squirrels_file;
	population_influx_file.open ("./data/population_influx.tsv");
	infection_level_file.open ("./data/infection_level.tsv");
	alive_squirrels_file.open ("./data/alive_squirrels.tsv");
	infected_squirrels_file.open ("./data/infected_squirrels.tsv");

	population_influx_file << Clock::population_influx_stream.rdbuf();
	infection_level_file << Clock::infection_level_stream.rdbuf();
	alive_squirrels_file << Clock::alive_squirrels_stream.rdbuf();
	infected_squirrels_file << Clock::infected_squirrels_stream.rdbuf();

	population_influx_file.close();
	infection_level_file.close();
	alive_squirrels_file.close();
	infected_squirrels_file.close();
}

Clock::Clock(int id, int workers_num, int max_months, int init_squirrels_no, int init_inf_squirrels_no): Actor(id, ACTOR_TYPE_CLOCK, workers_num) {
	this->max_months = max_months;
	this->month = 1;
	this->cells_ready = 0;
	this->alive_squirrels = init_squirrels_no;
	this->infected_squirrels = init_inf_squirrels_no;
	this->population_influx = vector<int>(CELL_NUM);
	this->infection_level = vector<int>(CELL_NUM);
	this->begin_time = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

	for (int i = 0; i < CELL_NUM; ++i) {
		this->population_influx[i] = 0;
		this->infection_level[i] = 0;
	}

	this->set_state(IN_MONTH);

	this->register_state(IN_MONTH, compute_in_month);
	this->register_state(END_OF_MONTH, compute_end_of_month);
	this->register_state(IN_MONTH, parse_message_in_month);
	this->register_state(END_OF_MONTH, parse_message_end_of_month);
}

Clock::~Clock() {}

