/*
Student Name: Justin Jonathan Liman
Student ID: 301224835
File: seg_display.c

Implementation of the module specified in seg_display.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <pthread.h>
#include <time.h>

#define GPIO_EXPORT_FILE_NAME "/sys/class/gpio/export"

#define LEFT_DISPLAY_DIRECTION_FILE_NAME "/sys/class/gpio/gpio61/direction"
#define RIGHT_DISPLAY_DIRECTION_FILE_NAME "/sys/class/gpio/gpio44/direction"

#define LEFT_DISPLAY_VALUE_FILE_NAME "/sys/class/gpio/gpio61/value"
#define RIGHT_DISPLAY_VALUE_FILE_NAME "/sys/class/gpio/gpio44/value"

#define LEFT_DISPLAY_GPIO_NUMBER 61
#define RIGHT_DISPLAY_GPIO_NUMBER 44

#define GPIO_DIRECTION_OUT "out"

#define GPIO_VALUE_ON "1"
#define GPIO_VALUE_OFF "0"

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2C_DEVICE_ADDRESS 0x20

#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15

#define SEG_DISPLAY_WAIT_NS 5000000
#define SEG_DISPLAY_MAX_NUM 99

#define DIGIT_14SEG_0_LOW 0xA1
#define DIGIT_14SEG_1_LOW 0x80
#define DIGIT_14SEG_2_LOW 0x31
#define DIGIT_14SEG_3_LOW 0xB0
#define DIGIT_14SEG_4_LOW 0x90
#define DIGIT_14SEG_5_LOW 0xB0
#define DIGIT_14SEG_6_LOW 0xB1
#define DIGIT_14SEG_7_LOW 0x80
#define DIGIT_14SEG_8_LOW 0xB1
#define DIGIT_14SEG_9_LOW 0xB0

#define DIGIT_14SEG_0_HIGH 0x86
#define DIGIT_14SEG_1_HIGH 0x02
#define DIGIT_14SEG_2_HIGH 0x0E
#define DIGIT_14SEG_3_HIGH 0x0E
#define DIGIT_14SEG_4_HIGH 0x8A
#define DIGIT_14SEG_5_HIGH 0x8C
#define DIGIT_14SEG_6_HIGH 0x8C
#define DIGIT_14SEG_7_HIGH 0x06
#define DIGIT_14SEG_8_HIGH 0x8E
#define DIGIT_14SEG_9_HIGH 0x8E

static const unsigned char DIGITS_LOW_BYTE[] = {DIGIT_14SEG_0_LOW, DIGIT_14SEG_1_LOW, DIGIT_14SEG_2_LOW, DIGIT_14SEG_3_LOW, DIGIT_14SEG_4_LOW,
												DIGIT_14SEG_5_LOW, DIGIT_14SEG_6_LOW, DIGIT_14SEG_7_LOW, DIGIT_14SEG_8_LOW, DIGIT_14SEG_9_LOW};
static const unsigned char DIGITS_HIGH_BYTE[] = {DIGIT_14SEG_0_HIGH, DIGIT_14SEG_1_HIGH, DIGIT_14SEG_2_HIGH, DIGIT_14SEG_3_HIGH, DIGIT_14SEG_4_HIGH,
												 DIGIT_14SEG_5_HIGH, DIGIT_14SEG_6_HIGH, DIGIT_14SEG_7_HIGH, DIGIT_14SEG_8_HIGH, DIGIT_14SEG_9_HIGH};

int numToDisplay = 0;

static pthread_t id;
static bool stopping = false;

static void* Seg_Display_displayNum(void *arg);
static int Seg_Display_initI2cBus(char* bus, int address);
static void Seg_Display_writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
static void Seg_Display_writeToFile(char *fileName, char *message);
static void Seg_Display_exportGpio(int gpioNumber);
static void Seg_Display_turnOffDisplpay(void);


void Seg_Display_init(int num)
{
	Seg_Display_exportGpio(LEFT_DISPLAY_GPIO_NUMBER);
	Seg_Display_exportGpio(RIGHT_DISPLAY_GPIO_NUMBER);
	Seg_Display_writeToFile(LEFT_DISPLAY_DIRECTION_FILE_NAME, GPIO_DIRECTION_OUT);
	Seg_Display_writeToFile(RIGHT_DISPLAY_DIRECTION_FILE_NAME, GPIO_DIRECTION_OUT);
	stopping = false;
	numToDisplay = num;
	pthread_create(&id, NULL, Seg_Display_displayNum, NULL);
	return;
}

void Seg_Display_cleanup(void)
{
	stopping = true;
	pthread_join(id, NULL);
	return;
}

static void* Seg_Display_displayNum(void *arg)
{
	int i2cFileDesc = Seg_Display_initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
	long seconds = 0;
	long nanoseconds = SEG_DISPLAY_WAIT_NS;
	struct timespec reqDelay = {seconds, nanoseconds};
	Seg_Display_writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
	Seg_Display_writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);
	int num = numToDisplay;
	if (num > SEG_DISPLAY_MAX_NUM) {
		num = SEG_DISPLAY_MAX_NUM;
	}
	int rightDigit = num % 10;
	int leftDigit = num / 10;
	while (!stopping) {
		Seg_Display_turnOffDisplpay();
		Seg_Display_writeI2cReg(i2cFileDesc, REG_OUTA, DIGITS_LOW_BYTE[leftDigit]);
		Seg_Display_writeI2cReg(i2cFileDesc, REG_OUTB, DIGITS_HIGH_BYTE[leftDigit]);
		Seg_Display_writeToFile(LEFT_DISPLAY_VALUE_FILE_NAME, GPIO_VALUE_ON);
		nanosleep(&reqDelay, (struct timespec *) NULL);
		Seg_Display_turnOffDisplpay();
		Seg_Display_writeI2cReg(i2cFileDesc, REG_OUTA, DIGITS_LOW_BYTE[rightDigit]);
		Seg_Display_writeI2cReg(i2cFileDesc, REG_OUTB, DIGITS_HIGH_BYTE[rightDigit]);
		Seg_Display_writeToFile(RIGHT_DISPLAY_VALUE_FILE_NAME, GPIO_VALUE_ON);
		nanosleep(&reqDelay, (struct timespec *) NULL);
	}
	Seg_Display_turnOffDisplpay();
	close(i2cFileDesc);
	pthread_exit(NULL);
}

static int Seg_Display_initI2cBus(char* bus, int address)
{
	int i2cFileDesc = open(bus, O_RDWR);
	if (i2cFileDesc < 0) {
		printf("I2C DRV: Unable to open bus for read/write (%s)\n", bus);
		exit(-1);
	}

	int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
	if (result < 0) {
		printf("Unable to set I2C device to slave address.");
		exit(-1);
	}
	return i2cFileDesc;
}

static void Seg_Display_writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
	unsigned char buff[2];
	buff[0] = regAddr;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		printf("Unable to write to i2c register");
		exit(-1);
	}
	return;
}

static void Seg_Display_writeToFile(char *fileName, char *message)
{
	FILE *pWriteFile = fopen(fileName, "w");
	if (pWriteFile == NULL) {
		printf("ERROR OPENING %s FOR WRITE.\n", fileName);
		exit(-1);
	}

	int charWritten = fprintf(pWriteFile, "%s", message);
	if (charWritten <= 0) {
		printf("ERROR WRITING DATA TO %s.\n", fileName);
		exit(-1);
	}

	fclose(pWriteFile);
	return;
}

static void Seg_Display_exportGpio(int gpioNumber)
{
	FILE *pExportFile = fopen(GPIO_EXPORT_FILE_NAME, "w");
	if (pExportFile == NULL) {
		printf("ERROR: Unable to open export file.\n");
		exit(-1);
	}
	fprintf(pExportFile, "%d", gpioNumber);
	fclose(pExportFile);
	return;
}

static void Seg_Display_turnOffDisplpay(void)
{
	Seg_Display_writeToFile(LEFT_DISPLAY_VALUE_FILE_NAME, GPIO_VALUE_OFF);
	Seg_Display_writeToFile(RIGHT_DISPLAY_VALUE_FILE_NAME, GPIO_VALUE_OFF);
	return;
}

