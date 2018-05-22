//Based on https://www.kernel.org/doc/Documentation/i2c/dev-interface

#include "linux/i2c-dev.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define I2C_BUS 0
#define I2C_SLAVE_ADDRESS 0xa
#define I2C_REGISTER 0xa
//function to know if the device is charging or not
#define I2C_COMMAND_CHARGING 0x1
#define I2C_COMMAND_BATTERY_LEVEL 0x10


__s32 set_mode(int file, __u8 command){
    const __u8 reg = I2C_REGISTER;
    // Using SMBus commands
    const __s32 result = i2c_smbus_write_byte_data(file, reg, command);
    if (result < 0) {
        printf("Error %s when writing\n", strerror(errno));
        return(-1);
    } else {
	return (1);
    }
}

__s32 read_value(int file){
    const __u8 reg = I2C_REGISTER; 
    const __s32 result = i2c_smbus_read_byte_data(file, reg);
    if (result < 0) {
        printf("Error %s when reading byte\n", strerror(errno));
        return(-1);
    } else {
	return (result);
    }
}

int main() {
    const int adapter_nr = I2C_BUS;
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
    const int file = open(filename, O_RDWR);
    if (file < 0) {
        printf("Error %s occured when trying to open device %s, you might have forgot to insert i2c ralink module\n", strerror(errno),filename);
        exit(EXIT_FAILURE);
    }

    const int addr = I2C_SLAVE_ADDRESS;

    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        printf("Error %s occured when trying to do and ioctl call to file %s\n", strerror(errno),filename);
        exit(EXIT_FAILURE);
    }

    __s32 result;
    result = set_mode(file, I2C_COMMAND_CHARGING);
    if (result < 0 ) return EXIT_FAILURE;

    result = read_value(file);
    if (result < 0 ) return EXIT_FAILURE;
    printf("charging=%s\n", result == 0x40 ? "yes":"no");

    result = set_mode(file, I2C_COMMAND_BATTERY_LEVEL);
    if (result < 0 ) return EXIT_FAILURE;

    result = read_value(file);
    if (result < 0 ) return EXIT_FAILURE;
    printf("battery_level=%u\n", result);

    close(file);

    return(EXIT_SUCCESS);
}
