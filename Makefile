#DIRECTORIES
BASE_DIR := .
SIMULATION_DIR := $(BASE_DIR)/parapoxvirus_simulation

parapoxvirus_simulation:
	@echo " Building parapoxvirus_simulation..."
	make -C $(SIMULATION_DIR) parapoxvirus_simulation

clean:
	@echo " Cleaning..."
	make -C $(SIMULATION_DIR) clean

.PHONY: clean parapoxvirus_simulation
