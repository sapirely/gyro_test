#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#include <stdint.h>

int main() {
    // Open Device
    //const char* iio_device_path = "/dev/iio:device0"; // 0 is gyro, 1 is accel
    char iio_device_path[1000];
    printf("Enter the IIO device path: ");
    if (fgets(iio_device_path, sizeof(iio_device_path), stdin) == NULL) {
        perror("Failed to read input");
        exit(1);
    }

    // Remove the trailing newline character from the input
    size_t len = strlen(iio_device_path);
    if (len > 0 && iio_device_path[len - 1] == '\n') {
        iio_device_path[len - 1] = '\0';
    }

    int iio_fd = open(iio_device_path, O_RDONLY);
    if (iio_fd < 0) {
        perror("Failed to open IIO device");
        exit(1);
    }
    printf("Opened IIO device");

    char buffer[8];
    // Read Sensor Data
    int i = 0;
    bool notRead = true;
    while (notRead && i < 100)
    {
        ssize_t num_bytes = read(iio_fd, buffer, sizeof(buffer));
        if (num_bytes < 0) {
            perror("Failed to read sensor data");

        }
        else
        {
            notRead = false;
        }
    }

    if (notRead)
    {
        perror("exiting");
        close(iio_fd);
        exit(1);
    }
    
    printf("Opened IIO device");

    // Interpret the raw data based on the LSM6DSx sensor data format and scaling
    // For example, if the data is 16-bit signed integers:
    int16_t gyro_x = buffer[0];
    int16_t gyro_y = buffer[1];
    int16_t gyro_z = buffer[2];
    // int16_t gyro_x = (buffer[0] << 8) | buffer[1];
    // int16_t gyro_y = (buffer[2] << 8) | buffer[3];
    // int16_t gyro_z = (buffer[4] << 8) | buffer[5];

    printf("RAW Gyroscope Data:\n");
    printf("X-Axis: %x\n", gyro_x);
    printf("Y-Axis: %x\n", gyro_y);
    printf("Z-Axis: %d\n", gyro_z);

    // Apply scaling if necessary based on sensor data format and sensor datasheet
    // For example, to convert raw accelerometer data to meters per second squared:
    float gyro_scale = 250 / (float)INT16_MAX; // // Assuming +/- 250 degrees/s range (16-bit signed integer range is -32768 to 32767)
    float gyro_x_deg_per_s = gyro_x * gyro_scale;
    float gyro_y_deg_per_s = gyro_y * gyro_scale;
    float gyro_z_deg_per_s = gyro_z * gyro_scale;

    // Similarly, you can convert gyroscope data to degrees per second or radians per second based on the sensor datasheet.

    printf("Gyroscope Data (degrees/s):\n");
    printf("X-Axis: %.2f\n", gyro_x_deg_per_s);
    printf("Y-Axis: %.2f\n", gyro_y_deg_per_s);
    printf("Z-Axis: %.2f\n", gyro_z_deg_per_s);

    close(iio_fd);
    return 0;

}