CFLAGS += -Wall

BUILD_DIR = build

TRASH = $(BUILD_DIR)

all: run

maze: maze.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@ $(LDFLAGS)

maze1: 00_arg_size.patch maze
	patch maze.c < $<
	$(CC) $(CFLAGS) maze.c -o $(BUILD_DIR)/$@ $(LDFLAGS)

maze2: 01_arg_chars.patch maze1
	patch maze.c < $<
	$(CC) $(CFLAGS) maze.c -o $(BUILD_DIR)/$@ $(LDFLAGS)

maze3: 02_arg_seed.patch maze2
	patch maze.c < $<
	$(CC) $(CFLAGS) maze.c -o $(BUILD_DIR)/$@ $(LDFLAGS)

run: maze3
	./$(BUILD_DIR)/maze
	./$(BUILD_DIR)/maze1 8
	./$(BUILD_DIR)/maze2 ' @' 5
	./$(BUILD_DIR)/maze3 42 ' @' 5
	./$(BUILD_DIR)/maze3 42 ' @' 5
	./$(BUILD_DIR)/maze3 17 ' @' 5

clean:
	rm -rf $(TRASH)

.PHONY: all clean
