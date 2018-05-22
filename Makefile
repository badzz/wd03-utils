all: wd03-battery

wd03-battery: wd03-battery.o ./linux/i2c-dev.h
	$(CC) -o wd03-battery wd03-battery.c linux/i2c-dev.h

install:
