#COMPILER OPTIONS
CFLAGS := -g -Wall -O3 -cc=icc
CC := mpicc

#DIRECTORIES
BASE_DIR := .
SRC_DIR := $(BASE_DIR)
HEADER_DIR := $(BASE_DIR)
BUILD_DIR := $(BASE_DIR)
EXE_DIR := $(BASE_DIR)
ACTOR_DIR := ../actor_framework

#FILES
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
EXE := $(EXE_DIR)/parapoxvirus_simulation

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER_DIR)/%.h
	$(CC) $(CFLAGS) -c -o $@ $<

parapoxvirus_simulation:
	@echo " Creating parapoxvirus_simulation executable..."
	make $(OBJ_FILES)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) $^

clean:
	@echo " Cleaning..."
	@rm -rf parapoxvirus_simulation $(BUILD_DIR)/*.o

.PHONY: clean parapoxvirus_simulation
