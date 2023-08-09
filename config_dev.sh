#!/bin/bash

#####
#GYRO
#####
# Disable buffered mode

echo 0 > /sys/bus/iio/devices/iio\:device0/buffer/enable

# Subscribe to data elements

echo 1 > /sys/bus/iio/devices/iio\:device0/scan_elements/in_anglvel_x_en
echo 1 > /sys/bus/iio/devices/iio\:device0/scan_elements/in_anglvel_y_en
echo 1 > /sys/bus/iio/devices/iio\:device0/scan_elements/in_anglvel_z_en
echo 1 > /sys/bus/iio/devices/iio\:device0/scan_elements/in_timestamp_en

# Set sampling frequency

echo 13 > /sys/bus/iio/devices/iio\:device0/sampling_frequency

# Set buffer length

echo 2 > /sys/bus/iio/devices/iio\:device0/buffer/length

# Enable buffered mode

echo 1 > /sys/bus/iio/devices/iio\:device0/buffer/enable

###############
# Accelerometer
###############

# Disable buffered mode

echo 0 > /sys/bus/iio/devices/iio\:device1/buffer/enable

# Subscribe to data elements

echo 1 > /sys/bus/iio/devices/iio\:device1/scan_elements/in_accel_x_en
echo 1 > /sys/bus/iio/devices/iio\:device1/scan_elements/in_accel_y_en
echo 1 > /sys/bus/iio/devices/iio\:device1/scan_elements/in_accel_z_en
echo 1 > /sys/bus/iio/devices/iio\:device1/scan_elements/in_timestamp_en

# Set sampling frequency

echo 13 > /sys/bus/iio/devices/iio\:device1/sampling_frequency

# Set buffer length

echo 2 > /sys/bus/iio/devices/iio\:device1/buffer/length

# Enable buffered mode

echo 1 > /sys/bus/iio/devices/iio\:device1/buffer/enable