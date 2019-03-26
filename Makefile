# Exam number: B136013

#DIRECTORIES
BASE_DIR := .
FRAMEWORK_DIR := $(BASE_DIR)/actor_framework
SIMULATION_DIR := $(BASE_DIR)/simulation

actor_framework:
	@echo " Building actor_framework..."
	make -C $(FRAMEWORK_DIR) actor_framework.so

simulation:
	@echo " Building simulation..."
	make -C $(SIMULATION_DIR) simulation

clean:
	@echo " Cleaning..."
	make -C $(SIMULATION_DIR) clean
	make -C $(FRAMEWORK_DIR) clean

.PHONY: clean actor_framework simulation
