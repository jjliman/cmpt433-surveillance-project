/*
Student Name: Justin Jonathan Liman & Jack (Tzu Chieh) Kao
Student ID: 301224835 & 301227859
File: utility.h

Module containing useful general-purpose functions.
*/

#ifndef UTILITY_H_
#define UTILITY_H_

void Utility_writeToFile(char *fileName, char *message);
void Utility_readFromFile(char *fileName, char *buffer, int bufferSize);
void Utility_sleepMilliseconds(long milliseconds);
void Utility_exportGpio(int gpioNum);


#endif
