#COMPILER OPTIONS
CFLAGS := -Wc++11-extensions -std=c++11 # -g -Wall -O3 -cc=icc
CC := g++ # mpicc 

#DIRECTORIES
BASE_DIR := .
SRC_DIR := $(BASE_DIR)
HEADER_DIR := $(BASE_DIR)
BUILD_DIR := $(BASE_DIR)
EXE_DIR := $(BASE_DIR)
ACTOR_DIR := ../actor_framework

#FILES
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))
EXE := $(EXE_DIR)/parapoxvirus_simulation

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADER_DIR)/%.hpp
	$(CC) $(CFLAGS) -c -o $@ $<

parapoxvirus_simulation:
	@echo " Creating parapoxvirus_simulation executable..."
	# $(CC) $(CFLAGS)-g -c -o main.o main.cpp
	make $(OBJ_FILES)
	# $(CC) $(CFLAGS) -o parapoxvirus_simulation squirrel.o main.o $(ACTOR_DIR)/actor_framework.o
	@$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) $^ main.o $(ACTOR_DIR)/actor_framework.o

clean:
	@echo " Cleaning..."
	@rm -rf parapoxvirus_simulation $(BUILD_DIR)/*.o

.PHONY: clean parapoxvirus_simulation
