/*
Student Name: Justin Jonathan Liman & Jack (Tzu Chieh) Kao
Student ID: 301224835 & 301227859
File: joystick.h

Module to spawn a separate background thread that reads the joystick.
Depending on the direction pressed, it will either change the volume (up/down),
change the tempo (left/right), or cycle the beats (pushed).
*/

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

typedef enum {
	JOYSTICK_NO_DIRECTION = 0,
	JOYSTICK_UP,
	JOYSTICK_RIGHT,
	JOYSTICK_DOWN,
	JOYSTICK_LEFT,
	JOYSTICK_PUSHED
} JoystickDirection;

void Joystick_init(void); // Export GPIO pins for all joystick directions and change direction to in
void Joystick_cleanup(void);
JoystickDirection Joystick_readDirection(void);
_Bool Joystick_getPasswordCorrect(void);

#endif
