BIN_DIR := bin
TARGET := $(BIN_DIR)/lsm
INC := 
#INC := -I include
SRC := src
OBJS := $(patsubst $(SRC)/%.c, $(BIN_DIR)/%.o, $(wildcard $(SRC)/*.c))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

$(OBJS): $(BIN_DIR)/%.o: $(SRC)/%.c | $(BIN_DIR)
	$(CC) -c $< $(INC) -o $@ -Wall -Wextra -Werror

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf bin
