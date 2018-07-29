/*
Student Name: Justin Jonathan Liman & Jack (Tzu Chieh) Kao
Student ID: 301224835 & 301227859
File: joystick.c

Implementation of the module specified in joystick.h
*/

#include "joystick.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include "utility.h"
#include "countdown.h"

#define JOYSTICK_UP_DIRECTION_FILE_NAME "/sys/class/gpio/gpio26/direction"
#define JOYSTICK_RIGHT_DIRECTION_FILE_NAME "/sys/class/gpio/gpio47/direction"
#define JOYSTICK_DOWN_DIRECTION_FILE_NAME "/sys/class/gpio/gpio46/direction"
#define JOYSTICK_LEFT_DIRECTION_FILE_NAME "/sys/class/gpio/gpio65/direction"
#define JOYSTICK_PUSHED_DIRECTION_FILE_NAME "/sys/class/gpio/gpio27/direction"

#define JOYSTICK_UP_VALUE_FILE_NAME "/sys/class/gpio/gpio26/value"
#define JOYSTICK_RIGHT_VALUE_FILE_NAME "/sys/class/gpio/gpio47/value"
#define JOYSTICK_DOWN_VALUE_FILE_NAME "/sys/class/gpio/gpio46/value"
#define JOYSTICK_LEFT_VALUE_FILE_NAME "/sys/class/gpio/gpio65/value"
#define JOYSTICK_PUSHED_VALUE_FILE_NAME "/sys/class/gpio/gpio27/value"

#define JOYSTICK_UP_GPIO_NUMBER 26
#define JOYSTICK_RIGHT_GPIO_NUMBER 47
#define JOYSTICK_DOWN_GPIO_NUMBER 46
#define JOYSTICK_LEFT_GPIO_NUMBER 65
#define JOYSTICK_PUSHED_GPIO_NUMBER 27

#define PASSWORD_SIZE 4
static JoystickDirection joystickPassword[PASSWORD_SIZE] = {JOYSTICK_LEFT, JOYSTICK_RIGHT, JOYSTICK_UP, JOYSTICK_DOWN};
static bool passwordCorrect = false;

#define BUFFER_MAX_SIZE 1024

void* Joystick_joystickThread(void* arg);
static _Bool stopping = false;
static pthread_t joystickThreadId;



void Joystick_init(void)
{
	Utility_exportGpio(JOYSTICK_UP_GPIO_NUMBER);
	Utility_exportGpio(JOYSTICK_RIGHT_GPIO_NUMBER);
	Utility_exportGpio(JOYSTICK_DOWN_GPIO_NUMBER);
	Utility_exportGpio(JOYSTICK_LEFT_GPIO_NUMBER);
	Utility_exportGpio(JOYSTICK_PUSHED_GPIO_NUMBER);

	Utility_writeToFile(JOYSTICK_UP_DIRECTION_FILE_NAME, "in");
	Utility_writeToFile(JOYSTICK_RIGHT_DIRECTION_FILE_NAME, "in");
	Utility_writeToFile(JOYSTICK_DOWN_DIRECTION_FILE_NAME, "in");
	Utility_writeToFile(JOYSTICK_LEFT_DIRECTION_FILE_NAME, "in");
	Utility_writeToFile(JOYSTICK_PUSHED_DIRECTION_FILE_NAME, "in");

	passwordCorrect = false;
	stopping = false;
	pthread_create(&joystickThreadId, NULL, Joystick_joystickThread, NULL);

	return;
}

void Joystick_cleanup(void)
{
	stopping = true;
	pthread_join(joystickThreadId, NULL);
	return;
}

JoystickDirection Joystick_readDirection(void)
{
	JoystickDirection currentDirection = JOYSTICK_NO_DIRECTION;
	char buffer[BUFFER_MAX_SIZE];

	Utility_readFromFile(JOYSTICK_UP_VALUE_FILE_NAME, buffer, BUFFER_MAX_SIZE);
	if (strncmp(buffer, "0", 1) == 0) {
		currentDirection = JOYSTICK_UP;
		return currentDirection;
	}
	*buffer = '\0';

	Utility_readFromFile(JOYSTICK_RIGHT_VALUE_FILE_NAME, buffer, BUFFER_MAX_SIZE);
	if (strncmp(buffer, "0", 1) == 0) {
		currentDirection = JOYSTICK_RIGHT;
		return currentDirection;
	}
	*buffer = '\0';

	Utility_readFromFile(JOYSTICK_DOWN_VALUE_FILE_NAME, buffer, BUFFER_MAX_SIZE);
	if (strncmp(buffer, "0", 1) == 0) {
		currentDirection = JOYSTICK_DOWN;
		return currentDirection;
	}
	*buffer = '\0';

	Utility_readFromFile(JOYSTICK_LEFT_VALUE_FILE_NAME, buffer, BUFFER_MAX_SIZE);
	if (strncmp(buffer, "0", 1) == 0) {
		currentDirection = JOYSTICK_LEFT;
		return currentDirection;
	}
	*buffer = '\0';

	Utility_readFromFile(JOYSTICK_PUSHED_VALUE_FILE_NAME, buffer, BUFFER_MAX_SIZE);
	if (strncmp(buffer, "0", 1) == 0) {
		currentDirection = JOYSTICK_PUSHED;
		return currentDirection;
	}
	*buffer = '\0';

	return currentDirection;
}

bool Joystick_getPasswordCorrect(void) {
	return passwordCorrect;
}

void* Joystick_joystickThread(void* arg)
{
	JoystickDirection userInput[PASSWORD_SIZE];
	for (int i = 0; i < PASSWORD_SIZE; i++) {
		userInput[i] = JOYSTICK_NO_DIRECTION;
	}

	int currInput = 0;
	while (!stopping) {
		JoystickDirection currDirection = JOYSTICK_NO_DIRECTION;
		currDirection = Joystick_readDirection();

		if (currDirection != JOYSTICK_NO_DIRECTION) {
			if (currDirection == JOYSTICK_PUSHED) {
				stopping = true;
				bool isPasswordCorrect = true;
				for (int i = 0; i < PASSWORD_SIZE; i++) {
					if (userInput[i] != joystickPassword[i]) {
						isPasswordCorrect = false;
					}
				}
				if (currInput != PASSWORD_SIZE) {
					isPasswordCorrect = false;
				}
				passwordCorrect = isPasswordCorrect;
				Countdown_abort();
			}
			else {
				if (currInput < PASSWORD_SIZE) {
					userInput[currInput] = currDirection;
				}
				currInput++;
			}
		}

		while (currDirection != JOYSTICK_NO_DIRECTION) {
			currDirection = Joystick_readDirection();
		}
	}
	pthread_exit(NULL);
}

