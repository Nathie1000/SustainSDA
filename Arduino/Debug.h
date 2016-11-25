/**
 * @file Debug.h
 *
 * @author Nathan Schaaphuizen
 * @date 12 okt. 2016
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <Arduino.h>

/**
 * Define the serial port used for debug printing.
 * Comment-out (or un-define) to disable debugging.
 */
#define DEBUG Serial

//Do not edit below here!
#ifdef DEBUG
/**Macro to begin serial connection. */
#define DEBUG_BEGIN(X) DEBUG.begin(X);
/**Macro to print line. */
#define PRINTLN(X) DEBUG.println(X); DEBUG.flush();
/**Macro to print. */
#define PRINT(X) DEBUG.print(X); DEBUG.flush();
#else
/**Macro to begin serial connection. */
#define DEBUG_BEGIN(X)
/**Macro to print line. */
#define PRINTLN(X)
/**Macro to print. */
#define PRINT(X)
#endif


#endif /* DEBUG_H_ */
