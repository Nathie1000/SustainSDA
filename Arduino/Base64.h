/*
 * Base64.h
 *
 *  Created on: 28 sep. 2016
 *      Author: Nathan
 */

#ifndef BASE64_H_
#define BASE64_H_

#include <Arduino.h>

String base64_encode(unsigned char const* , unsigned int len);
String base64_decode(const String  &s);


#endif /* BASE64_H_ */
