#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include "utility.h"
#include "grabber.h"
#include "countdown.h"

#define SENSOR_DIRECTION_FILE_NAME "/sys/class/gpio/gpio49/direction"

#define SENSOR_VALUE_FILE_NAME "/sys/class/gpio/gpio49/value"

#define SENSOR_GPIO_NUMBER 49

#define BUFFER_MAX_SIZE 10

#define POLL_INTERVAL_MS 100 // or 10?

static _Bool active = false;

void* Sensor_sensorThread(void* arg);
static _Bool stopping = false;
static pthread_t sensorThreadId;



void Sensor_init(void)
{
	Utility_exportGpio(SENSOR_GPIO_NUMBER);
	Utility_writeToFile(SENSOR_DIRECTION_FILE_NAME, "in");
	active = true;
	pthread_create(&sensorThreadId, NULL, Sensor_sensorThread, NULL);
	return;
}

void Sensor_cleanup(void)
{
//	stopping = true;
	pthread_join(sensorThreadId, NULL);
	return;
}

void Sensor_setActive(void)
{
	active = true;
	return;
}

void Sensor_setIdle(void)
{
	active = false;
	return;
}

void* Sensor_sensorThread(void* arg)
{
	while (!stopping) {
		if (active) {
			char buffer[BUFFER_MAX_SIZE];
			Utility_readFromFile(SENSOR_VALUE_FILE_NAME, buffer, BUFFER_MAX_SIZE);
			if (strncmp(buffer, "1", 1) == 0) {
				printf("Sensor detected someone!\n");
				Sensor_setIdle();
				Grabber_init();
				Countdown_countdown();
				Grabber_cleanup();
			}
		}
		Utility_sleepMilliseconds(POLL_INTERVAL_MS);
	}
	pthread_exit(NULL);
}

