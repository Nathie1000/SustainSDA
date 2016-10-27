/*
 * Debug.h
 *
 *  Created on: 12 okt. 2016
 *      Author: Nathan
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <Arduino.h>

//Define the serial port used for debug printing.
//Comment-out (or un-define) to disable debugging.
#define DEBUG Serial

//Do not edit below here!
#ifdef DEBUG
#define DEBUG_BEGIN(X) DEBUG.begin(X);
#define PRINTLN(X) DEBUG.println(X); DEBUG.flush();
#define PRINT(X) DEBUG.print(X); DEBUG.flush();
#else
#define DEBUG_BEGIN(X)
#define PRINTLN(X)
#define PRINT(X)
#endif


#endif /* DEBUG_H_ */
