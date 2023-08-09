#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

void printHexBuffer(const char* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        // Use %02X format specifier to ensure each byte is printed as two digits with leading zeros
        printf("%02X ", (unsigned char)buffer[i]);
    }
    printf("\n");
}

void printDataWithScale(int16_t x, int16_t y, int16_t z, float scale, const char* device_str)
{
    float x_deg = x * scale;
    float y_deg = y * scale;
    float z_deg = z * scale;

    printf("%s Data (degrees/s):\n", device_str);
    printf("X-Axis: %.2f\n", x_deg);
    printf("Y-Axis: %.2f\n", y_deg);
    printf("Z-Axis: %.2f\n", z_deg);
}

int main() {

    // BEFORE RUNNING THIS:
    // enable the parameters and 
    // Open Device
    //const char* iio_device_path = "/dev/iio:device0"; // 0 is gyro, 1 is accel
    char iio_device_path[1000];
    printf("Enter the IIO device path: ");
    if (fgets(iio_device_path, sizeof(iio_device_path), stdin) == NULL) {
        perror("Failed to read input\n");
        exit(1);
    }

    // Remove the trailing newline character from the input
    size_t len = strlen(iio_device_path);
    if (len > 0 && iio_device_path[len - 1] == '\n') {
        iio_device_path[len - 1] = '\0';
    }

    int iio_fd = open(iio_device_path, O_RDONLY);
    if (iio_fd < 0) {
        perror("Failed to open IIO device\n");
        exit(1);
    }
    printf("Opened IIO device\n");

    char buffer[16];
    // Read Sensor Data - little endian
    int i = 0;
    bool notRead = true;
    while (i < 100)
    {
        ssize_t num_bytes = read(iio_fd, buffer, sizeof(buffer));
        if (num_bytes < 0) {
            perror("Failed to read sensor data\n");
        }
        else
        {
            printf("read %ld bytes\n", num_bytes);
            printHexBuffer(buffer, num_bytes);
            notRead = false;
        }
        i++;
    }

    if (notRead)
    {
        perror("exiting\n");
        close(iio_fd);
        exit(1);
    }
    
    printf("Opened IIO device\n");

    // data is signed 16bit, little endian
    int16_t gyro_x = (buffer[1] << 8) | buffer[0];
    int16_t gyro_y = (buffer[3] << 8) | buffer[2];
    int16_t gyro_z = (buffer[5] << 8) | buffer[4];

    // printf("RAW Gyroscope Data: (HEX)\n");
    // printf("X-Axis: %x\n", gyro_x);
    // printf("Y-Axis: %x\n", gyro_y);
    // printf("Z-Axis: %x\n", gyro_z);

    printf("RAW Gyroscope Data: (dec)\n");
    printf("X-Axis: %d\n", gyro_x);
    printf("Y-Axis: %d\n", gyro_y);
    printf("Z-Axis: %d\n", gyro_z);

    // 0.000152716 = in_anglvel_scale
    float gyro_scale = 0.000152716; // in_anglvel_scale;
    float acc_scale = 0.000598205; // in_accel_scale;

    if (strstr(iio_device_path, "device0") != NULL) {
        printDataWithScale(gyro_x, gyro_y, gyro_z, gyro_scale, "gyro");
    } else {
        printDataWithScale(gyro_x, gyro_y, gyro_z, acc_scale, "accel");
    }

    close(iio_fd);
    return 0;

}