# Inspried by Chnossos @ stackoverflow,
# (https://stackoverflow.com/questions/30573481/how-to-write-a-makefile-with-separate-source-and-header-directories)

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Finding source and object files
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# used to filter out the separte main required for test and compiler binary
COMPILER_EXE := $(BIN_DIR)/compiler
COMPILER_OBS := $(filter-out obj/tester.o, $(OBJ))

TESTER_EXE := $(BIN_DIR)/tester
TESTER_OBS := $(filter-out obj/compiler.o, $(OBJ))


CPPFLAGS := -Isrc # -I is a preprocessor flag, not a compiler flag
CFLAGS	 := -Wall -Wextra -pedantic -g
LDFLAGS  :=  # -L is a linker flag
LDLIBS   :=  # Left empty if no libs are needed

.PHONY: all test clean


all: $(COMPILER_EXE)
	./$(COMPILER_EXE) < test/program

test: clean $(TESTER_EXE)
	./$(TESTER_EXE) test/parse_tests

# Linking of object files
$(COMPILER_EXE): $(COMPILER_OBS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(TESTER_EXE): $(TESTER_OBS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@	

# Compile src file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Create src and obj dir if they not exits
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR) # The @ disables the echoing of the command

#-include $(OBJ:.o=.d)	