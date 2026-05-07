SRC_DIR = src
OBJ_DIR = bin
LIB_DIR = lib

CC = g++
CFLAGS = -Wall -I$(LIB_DIR)

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))


TARGET = encoder

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

rebuild: clean all

run: $(TARGET)
	./$(TARGET)

debug:
	@echo "SRCS = $(SRCS)"
	@echo "OBJS = $(OBJS)"

.PHONY: all clean rebuild run debug
