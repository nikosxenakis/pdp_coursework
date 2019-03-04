#DIRECTORIES
BASE_DIR := .
ACTOR_DIR := $(BASE_DIR)/actor_framework
SIMULATION_DIR := $(BASE_DIR)/parapoxvirus_simulation

actor_framework:
	@echo " Building actor_framework..."
	make -C $(ACTOR_DIR) actor_framework

parapoxvirus_simulation:
	@echo " Building parapoxvirus_simulation..."
	make -C $(SIMULATION_DIR) parapoxvirus_simulation

clean:
	@echo " Cleaning..."
	make -C $(ACTOR_DIR) clean
	make -C $(SIMULATION_DIR) clean

.PHONY: clean actor_framework parapoxvirus_simulation
