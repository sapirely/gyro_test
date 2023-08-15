#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include  <signal.h>
#include <sys/time.h>

static int iio_fd;
static float gyro_scale = 0.000152716; // in_anglvel_scale;
static float acc_scale = 0.000598205; // in_accel_scale;

static struct timeval tvStart, tvEnd;
static uint64_t totalReadTime = 0, readTime = 0, triesCnt=0;


uint64_t TimeDiffUsec(struct timeval tvStart, struct timeval tvEnd){
   return (1000000 * tvEnd.tv_sec + tvEnd.tv_usec) - (1000000 * tvStart.tv_sec + tvStart.tv_usec);
}

void sigintHandler(int sig) {
    signal(sig, SIG_IGN);
    printf("\nCaught Ctrl+C signal. Performing cleanup...\n");
    // Add your cleanup code here
    // Close files, release resources, etc.

    printf("avReadTime = %lluusec\n", totalReadTime/triesCnt);
    
    // Exit the program gracefully
    close(iio_fd);
    exit(EXIT_SUCCESS);
}

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
    printf("X-Axis: %.2f\tY-Axis: %.2f\tZ-Axis: %.2f\n", x_deg, y_deg, z_deg);
}



int main() {

    // BEFORE RUNNING THIS:
    // enable the parameters and 
    // Open Device
    //const char* iio_device_path = "/dev/iio:device0"; // 0 is gyro, 1 is accel
    char iio_device_path[1000];
    int16_t gyro_x, gyro_y, gyro_z;

    signal(SIGINT, sigintHandler);

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

    iio_fd = open(iio_device_path, O_RDONLY);
    if (iio_fd < 0) {
        perror("Failed to open IIO device\n");
        exit(1);
    }
    printf("Opened IIO device\n");

    char buffer[16];
    // Read Sensor Data - little endian
    //int i = 0;
    bool notRead = true;
    //while (i < 100)

    while (true)
    {
        gettimeofday(&tvStart,NULL);
        ssize_t num_bytes = read(iio_fd, buffer, sizeof(buffer));
        gettimeofday(&tvEnd,NULL);
        readTime = TimeDiffUsec(tvStart, tvEnd);
        totalReadTime+=readTime;
        triesCnt++;
        if (num_bytes < 0) {
            perror("Failed to read sensor data\n");
        }
        else
        {
            
            //printf("read %ld bytes\n", num_bytes);
            //printHexBuffer(buffer, num_bytes);
            // data is signed 16bit, little endian
            gyro_x = (buffer[1] << 8) | buffer[0];
            gyro_y = (buffer[3] << 8) | buffer[2];
            gyro_z = (buffer[5] << 8) | buffer[4];
            if (strstr(iio_device_path, "device0") != NULL) {
                printDataWithScale(gyro_x, gyro_y, gyro_z, gyro_scale, "gyro");
            } else {
                printDataWithScale(gyro_x, gyro_y, gyro_z, acc_scale, "accel");
            }
            printf("X-raw: %d\tY-raw: %d\tZ-raw: %d\n", gyro_x, gyro_y, gyro_z);
            notRead = false;
        }

        if (triesCnt >= UINT64_MAX-1 || totalReadTime >= UINT64_MAX-1)
        {
            triesCnt = 0;
            totalReadTime = 0;
        }
        //i++;
    }

    if (notRead)
    {
        perror("exiting\n");
        close(iio_fd);
        exit(1);
    }
    
    printf("Opened IIO device\n");

    printf("avReadTime = %lluusec\n", totalReadTime/triesCnt);

    // // data is signed 16bit, little endian
    // int16_t gyro_x = (buffer[1] << 8) | buffer[0];
    // int16_t gyro_y = (buffer[3] << 8) | buffer[2];
    // int16_t gyro_z = (buffer[5] << 8) | buffer[4];

    // printf("RAW Gyroscope Data: (dec)\n");
    // printf("X-Axis: %d\n", gyro_x);
    // printf("Y-Axis: %d\n", gyro_y);
    // printf("Z-Axis: %d\n", gyro_z);

    // 0.000152716 = in_anglvel_scale
    // float gyro_scale = 0.000152716; // in_anglvel_scale;
    // float acc_scale = 0.000598205; // in_accel_scale;

    if (strstr(iio_device_path, "device0") != NULL) {
        printDataWithScale(gyro_x, gyro_y, gyro_z, gyro_scale, "gyro");
    } else {
        printDataWithScale(gyro_x, gyro_y, gyro_z, acc_scale, "accel");
    }

    close(iio_fd);
    return 0;

}