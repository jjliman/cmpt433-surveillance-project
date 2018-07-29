#include "countdown.h"
#include "joystick.h"
#include "grabber.h"
#include "sensor.h"


int main(int argc, char* argv[])
{
	Countdown_init();
//	Countdown_countdown();
	Sensor_init();
	Sensor_cleanup();
	Countdown_cleanup();
	return 0;
}
