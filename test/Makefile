CC=gcc
CFLAGS= -Wall -Wextra -lm
TARGET=fft_test
OBJ=../fft.o test.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(CFLAGS)

test.o: test.c
	$(CC) -c test.c -o test.o $(CFLAGS)

../fft.o: ../fft.c
	$(CC) -c ../fft.c -o ../fft.o $(CFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all run clean
