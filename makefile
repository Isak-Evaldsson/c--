# Inspried by Chnossos @ stackoverflow,
# (https://stackoverflow.com/questions/30573481/how-to-write-a-makefile-with-separate-source-and-header-directories)

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/compiler
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CPPFLAGS := -Isrc # -I is a preprocessor flag, not a compiler flag
CFLAGS	 := -Wall -Wextra -pedantic -g
LDFLAGS  :=  # -L is a linker flag
LDLIBS   :=  # Left empty if no libs are needed

.PHONY: all clean


all: $(EXE)
	./$(EXE) < test/program

# Linking of object files
$(EXE): $(OBJ) | $(BIN_DIR)
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