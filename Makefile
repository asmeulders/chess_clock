CC      = gcc
CFLAGS  = -Wall -Wextra -g -MMD -MP
LDFLAGS = -lncurses

SRC_DIR   = src
BUILD_DIR = build
BIN_DIR   = bin

TARGET  = $(BIN_DIR)/chess_clock
SRCS    = $(wildcard $(SRC_DIR)/*.c)
OBJS    = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS   = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: all clean

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)