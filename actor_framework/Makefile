#COMPILER OPTIONS
CFLAGS := -g -std=c++11 -Wwrite-strings # -Wall
CC := mpicxx

#DIRECTORIES
BASE_DIR := .
SRC_DIR := $(BASE_DIR)
BUILD_DIR := $(BASE_DIR)

#FILES
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

actor_framework:
	@echo " Compiling actor_framework objective files..."
	make $(OBJ_FILES)

parapoxvirus_simulation:
	@echo " Creating parapoxvirus_simulation executable..."
	make $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $^ -o $@

clean:
	@echo " Cleaning..."
	@rm -rf $(BUILD_DIR)/*.o

.PHONY: clean parapoxvirus_simulation actor_framework
