all:
	mkdir -p $(HOME)/cmpt433/public/countdown-wav-files/
	cp -R countdown-wav-files/* $(HOME)/cmpt433/public/countdown-wav-files/
	arm-linux-gnueabihf-gcc -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror main.c countdown.c audioMixer.c seg_display.c joystick.c sensor.c grabber.c utility.c -o $(HOME)/cmpt433/public/countdown  -L$(HOME)/cmpt433/public/asound_lib_BBB -lpthread -lasound -lv4l2 -lv4lconvert -ljpeg
	