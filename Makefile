.PHONY: clean all
CC=gcc
CFLAGS=-Wall -lpthread -lrt -fPIE
SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
EXECUTABLE=$(BIN_DIR)/hRace

all: $(BIN_DIR)/hRace

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR)/hRace: $(OBJ_DIR)/hRace.o $(OBJ_DIR)/oterm.o | $(BIN_DIR)
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean: 
	$(RM) $(OBJ_DIR)/*.o $(BIN_DIR)/hRace