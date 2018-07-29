/*
Student Name: Justin Jonathan Liman & Jack (Tzu Chieh) Kao
Student ID: 301224835 & 301227859
File: utility.c

Implementation of the module specified in utility.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


#define GPIO_EXPORT_FILE_NAME "/sys/class/gpio/export"


void Utility_writeToFile(char *fileName, char *message)
{
	FILE *pWriteFile = fopen(fileName, "w");
	if (pWriteFile == NULL) {
		printf("ERROR OPENING %s FOR WRITE.\n", fileName);
		exit(EXIT_FAILURE);
	}

	int charWritten = fprintf(pWriteFile, "%s", message);
	if (charWritten <= 0) {
		printf("ERROR WRITING DATA TO %s.\n", fileName);
		exit(EXIT_FAILURE);
	}

	fclose(pWriteFile);
	return;
}

void Utility_readFromFile(char *fileName, char *buffer, int bufferSize)
{
	FILE *pReadFile = fopen(fileName, "r");
	if (pReadFile == NULL) {
		printf("ERROR OPENING %s FOR READ.\n", fileName);
		exit(EXIT_FAILURE);
	}

	fgets(buffer, bufferSize, pReadFile);

	fclose(pReadFile);
	return;
}

void Utility_exportGpio(int gpioNum)
{
	FILE *pExportFile = fopen(GPIO_EXPORT_FILE_NAME, "w");
	if (pExportFile == NULL) {
		printf("ERROR: Unable to open export file.\n");
		exit(EXIT_FAILURE);
	}
	fprintf(pExportFile, "%d", gpioNum);
	fclose(pExportFile);
}

void Utility_sleepMilliseconds(long milliseconds)
{
	struct timespec reqDelay;
	reqDelay.tv_sec = (milliseconds / 1000);
	reqDelay.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&reqDelay, (struct timespec *) NULL);
}
