#CC := gcc
CFLAGS := -Wall -Wextra -std=c99
LDFLAGS := -lm

# Add any additional libraries or flags if required

# Name of the executable
TARGET := gyro_test

# Source files
SRC := gyro_test.c iio_dummy.c 
HDR := gyro_test.h

# Object files
OBJ := $(SRC:.c=.o)

.PHONY: all clean

all: $(TARGET) iio_dummy gyro_test

iio_dummy: iio_dummy.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ iio_dummy.o -liio

gyro_test: gyro_test.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ gyro_test.o

# $(TARGET): $(OBJ)
# 	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ)

iio_dummy.o: iio_dummy.c 
	$(CC) $(CFLAGS) -c -o $@ $<

gyro_test.o: gyro_test.c $(HDR)
	$(CC) $(CFLAGS) -c -o $@ $<
# %.o: %.c $(HDR)
# 	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) iio_dummy $(OBJ)
