/*
Student Name: Justin Jonathan Liman
Student ID: 301224835
File: main.c
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "seg_display.h"
#include "audioMixer.h"
#include "joystick.h"
#include "sensor.h"
#include "utility.h"

#define ONE_SOURCE_FILE  "countdown-wav-files/1.wav"
#define TWO_SOURCE_FILE  "countdown-wav-files/2.wav"
#define THREE_SOURCE_FILE  "countdown-wav-files/3.wav"
#define FOUR_SOURCE_FILE  "countdown-wav-files/4.wav"
#define FIVE_SOURCE_FILE  "countdown-wav-files/5.wav"
#define SIX_SOURCE_FILE  "countdown-wav-files/6.wav"
#define SEVEN_SOURCE_FILE  "countdown-wav-files/7.wav"
#define EIGHT_SOURCE_FILE  "countdown-wav-files/8.wav"
#define NINE_SOURCE_FILE  "countdown-wav-files/9.wav"
#define TEN_SOURCE_FILE "countdown-wav-files/10.wav"
#define START_SOURCE_FILE "countdown-wav-files/start.wav"
#define FAIL_SOURCE_FILE "countdown-wav-files/fail.wav"
#define BUZZER_SOURCE_FILE "countdown-wav-files/buzzer.wav"
#define SUCCESS_SOURCE_FILE "countdown-wav-files/success.wav"

static wavedata_t one;
static wavedata_t two;
static wavedata_t three;
static wavedata_t four;
static wavedata_t five;
static wavedata_t six;
static wavedata_t seven;
static wavedata_t eight;
static wavedata_t nine;
static wavedata_t ten;
static wavedata_t start;
static wavedata_t fail;
static wavedata_t buzzer;
static wavedata_t success;

static _Bool inCountdown = true;

static void loadWaveFiles(void);
static void freeWaveFiles(void);

void Countdown_init(void)
{
	AudioMixer_init();
	loadWaveFiles();
}

void Countdown_cleanup(void)
{
	AudioMixer_cleanup();
	freeWaveFiles();
}

void Countdown_countdown()
{
	Joystick_init();
	inCountdown = true;
	AudioMixer_queueSound(&buzzer);

	Utility_sleepMilliseconds(1500);
	AudioMixer_queueSound(&start);
	Utility_sleepMilliseconds(3500);
	wavedata_t COUNTDOWN_WAV[] = {one, two, three, four, five, six, seven, eight, nine, ten};
	for (int i = 9; i >= 0; i--) {
		Seg_Display_init(i+1);
		AudioMixer_queueSound(&COUNTDOWN_WAV[i]);
		sleep(1);
		Seg_Display_cleanup();
		if (!inCountdown) {
			break;
		}
	}
	if(Joystick_getPasswordCorrect()) {
		AudioMixer_queueSound(&success);
		Utility_sleepMilliseconds(6000);
	}
	else {
		AudioMixer_queueSound(&fail);
		Utility_sleepMilliseconds(6000);
	}
	Joystick_cleanup();
	Sensor_setActive();
	return;
}

void Countdown_abort(void)
{
	inCountdown = false;
	return;
}

static void loadWaveFiles(void)
{
	AudioMixer_readWaveFileIntoMemory(ONE_SOURCE_FILE, &one);
	AudioMixer_readWaveFileIntoMemory(TWO_SOURCE_FILE, &two);
	AudioMixer_readWaveFileIntoMemory(THREE_SOURCE_FILE, &three);
	AudioMixer_readWaveFileIntoMemory(FOUR_SOURCE_FILE, &four);
	AudioMixer_readWaveFileIntoMemory(FIVE_SOURCE_FILE, &five);
	AudioMixer_readWaveFileIntoMemory(SIX_SOURCE_FILE, &six);
	AudioMixer_readWaveFileIntoMemory(SEVEN_SOURCE_FILE, &seven);
	AudioMixer_readWaveFileIntoMemory(EIGHT_SOURCE_FILE, &eight);
	AudioMixer_readWaveFileIntoMemory(NINE_SOURCE_FILE, &nine);
	AudioMixer_readWaveFileIntoMemory(TEN_SOURCE_FILE, &ten);
	AudioMixer_readWaveFileIntoMemory(START_SOURCE_FILE, &start);
	AudioMixer_readWaveFileIntoMemory(FAIL_SOURCE_FILE, &fail);
	AudioMixer_readWaveFileIntoMemory(BUZZER_SOURCE_FILE, &buzzer);
	AudioMixer_readWaveFileIntoMemory(SUCCESS_SOURCE_FILE, &success);
	return;
}

static void freeWaveFiles(void)
{
	AudioMixer_freeWaveFileData(&one);
	AudioMixer_freeWaveFileData(&two);
	AudioMixer_freeWaveFileData(&three);
	AudioMixer_freeWaveFileData(&four);
	AudioMixer_freeWaveFileData(&five);
	AudioMixer_freeWaveFileData(&six);
	AudioMixer_freeWaveFileData(&seven);
	AudioMixer_freeWaveFileData(&eight);
	AudioMixer_freeWaveFileData(&nine);
	AudioMixer_freeWaveFileData(&ten);
	AudioMixer_freeWaveFileData(&start);
	AudioMixer_freeWaveFileData(&fail);
	AudioMixer_freeWaveFileData(&buzzer);
	AudioMixer_freeWaveFileData(&success);
	return;
}


