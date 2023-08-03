CC := gcc
CFLAGS := -Wall -Wextra -std=c99
LDFLAGS := -lm

# Add any additional libraries or flags if required

# Name of the executable
TARGET := gyro_test

# Source files
SRC := gyro_test.c
HDR := gyro_test.h

# Object files
OBJ := $(SRC:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJ)
