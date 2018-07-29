/*
Student Name: Justin Jonathan Liman
Student ID: 301224835
File: seg_display.h

Module to spawn a separate background thread to display
the number of arrays sorted in the last second on the
Zen cape's 14-segment display.
*/

#ifndef _SEG_DISPLAY_H_
#define _SEG_DISPLAY_H_

// Initialize, terminate, and cleanup the background thread
// which controls the Zen cape's 14-segment display and
// displays the number of arrays sorted in the last second.
void Seg_Display_init(int num);
void Seg_Display_cleanup(void);

#endif
