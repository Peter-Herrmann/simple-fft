CC = gcc
CFLAGS = -Wall -Wextra -I./src  # Added -I./src for include path
LDFLAGS = -lm
TARGET = build/fft_test
SRC_DIR = src
TEST_DIR = test
BUILD_DIR = build

# Source and Object files
SRC_FILES = $(SRC_DIR)/fft.c
TEST_FILES = $(TEST_DIR)/test.c
OBJ_FILES = $(BUILD_DIR)/fft.o $(BUILD_DIR)/test.o

all: directories $(TARGET)

# Create the necessary directories
directories:
	@mkdir -p $(BUILD_DIR)

# Linking the executable
$(TARGET): $(OBJ_FILES)
	$(CC) -o $(TARGET) $(OBJ_FILES) $(LDFLAGS)

# Compiling object files
$(BUILD_DIR)/fft.o: $(SRC_DIR)/fft.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/test.o: $(TEST_DIR)/test.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Run the executable
run: $(TARGET)
	./$(TARGET)

# Clean the build directory
clean:
	rm -f $(TARGET) $(OBJ_FILES)

.PHONY: all directories run clean
