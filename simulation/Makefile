#COMPILER OPTIONS
CFLAGS := -g -std=c++11 -Wall -O2
CC := mpicxx

#DIRECTORIES
BASE_DIR := .
SRC_DIR := $(BASE_DIR)
BUILD_DIR := $(BASE_DIR)
EXE_DIR := $(BASE_DIR)
ACTOR_DIR := ../actor_framework

#FILES
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
ACTOR_SRC_FILES := $(wildcard $(ACTOR_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))
ACTOR_OBJ_FILES := $(patsubst $(ACTOR_DIR)/%.cpp, $(ACTOR_DIR)/%.o, $(ACTOR_SRC_FILES))
EXE := $(EXE_DIR)/simulation

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

simulation:
	@echo " Creating simulation executable..."
	make $(OBJ_FILES)
	$(CC) -L../actor_framework -l:actor_framework.so -o $@ $(OBJ_FILES) $^

clean:
	@echo " Cleaning..."
	@rm -rf simulation $(BUILD_DIR)/*.o

.PHONY: clean simulation
