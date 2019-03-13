#include "clock.h"

#define IN_MONTH 0
#define END_OF_MONTH 1
#define FINISH 3

#define CELL_NUM 16
#define TIMESTEP_DURATION 100

stringstream Clock::population_influx_stream;
stringstream Clock::infection_level_stream;

static void parse_message_dummy(Actor *actor, Message message) {}

static void compute_in_month(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	milliseconds curr_time = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

	if((curr_time - clock->begin_time).count() > TIMESTEP_DURATION) {
		clock->begin_time = curr_time;
		cout << "Clock starts simulation for timestep " << clock->timestep << "\n";

		clock->set_state(END_OF_MONTH);
		Message message;
		message.message_data.command = TIMESTEP_END;
		for (int cell_id = 0; cell_id < CELL_NUM; ++cell_id) {
			clock->send_msg(cell_id, message);
		}
	}
}

static void compute_end_of_month(Actor *actor) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	if(clock->cells_ready == CELL_NUM) {
		clock->write_output_stream();
		clock->cells_ready = 0;
		clock->timestep++;

		if(clock->timestep > clock->max_months) {
			clock->set_state(FINISH);
			clock->write_output_files();
			clock->kill_all();
		}
		else {
			clock->begin_time = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
			clock->set_state(IN_MONTH);
		}
	}
}

static void compute_finish(Actor *actor) {}

static void parse_message_end_of_month(Actor *actor, Message message) {
	Clock *clock = dynamic_cast<Clock*>(actor);
	if(message.message_data.command == TIMESTEP_END) {
		clock->population_influx[clock->cells_ready] = message.message_data.population_influx;
		clock->infection_level[clock->cells_ready] = message.message_data.infection_level;
		clock->alive_squirrels = message.message_data.alive_squirrels;
		clock->infected_squirrels = message.message_data.infected_squirrels;

		// cout << "clock: " << clock->timestep << " population_influx " << clock->population_influx[clock->cells_ready] << " infection_level " << clock->infection_level[clock->cells_ready] << endl;
		clock->cells_ready++;
	}
}

void Clock::write_output_stream() {

	if(this->timestep == 1) {
  		Clock::population_influx_stream << "timestep\t";
  		Clock::infection_level_stream << "timestep\t";
  		for (int i = 0; i < CELL_NUM; ++i) {
  			Clock::population_influx_stream << "\tCell " << i;
  			Clock::infection_level_stream << "\tCell " << i;
  		}
  		Clock::population_influx_stream << "\n";
  		Clock::infection_level_stream << "\n";
	}

	Clock::population_influx_stream << this->timestep << "\t";

	Clock::infection_level_stream << this->timestep << "\t";
	for(auto population_influx: this->population_influx) {
		this->population_influx_stream << "\t" << population_influx;
	}
	for(auto infection_level: this->infection_level) {
		Clock::infection_level_stream << "\t" << infection_level;
	}
	Clock::population_influx_stream << endl;
	Clock::infection_level_stream << endl;

	cout << "alive_squirrels = " << this->alive_squirrels << "\t" << "infected_squirrels = " << this->infected_squirrels << endl;
}

void Clock::write_output_files() {
	ofstream population_influx_file, infection_level_file;
	population_influx_file.open ("./data/population_influx.tsv");
	infection_level_file.open ("./data/infection_level.tsv");
	population_influx_file << Clock::population_influx_stream.rdbuf();
	infection_level_file << Clock::infection_level_stream.rdbuf();
	population_influx_file.close();
	infection_level_file.close();
}

Clock::Clock(int id, int master_pid, int worker_pid, int workers_num, int max_months): Actor(id, master_pid, worker_pid, workers_num) {
	this->type = ACTOR_TYPE_CLOCK;
	this->max_months = max_months;
	this->timestep = 1;
	this->cells_ready = 0;
	this->alive_squirrels = 0;
	this->infected_squirrels = 0;
	this->population_influx = vector<int>(CELL_NUM);
	this->infection_level = vector<int>(CELL_NUM);
	this->begin_time = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

	for (int i = 0; i < CELL_NUM; ++i) {
		this->population_influx[i] = 0;
		this->infection_level[i] = 0;
	}

	this->set_state(IN_MONTH);

	this->register_state(COMPUTE, IN_MONTH, compute_in_month);
	this->register_state(COMPUTE, END_OF_MONTH, compute_end_of_month);
	this->register_state(COMPUTE, FINISH, compute_finish);

	this->register_state(PARSE_MESSAGE, IN_MONTH, parse_message_dummy);
	this->register_state(PARSE_MESSAGE, END_OF_MONTH, parse_message_end_of_month);
	this->register_state(PARSE_MESSAGE, FINISH, parse_message_dummy);
}
