/**
 * @file Base64.h
 *
 * @author Nathan Schaaphuizen
 * @date 28 sep. 2016
 */

#ifndef BASE64_H_
#define BASE64_H_

#include <Arduino.h>

/**
 * Encode array of bytes as a Base64 String.
 * @param bytes_to_encode the bytes to encode.
 * @param in_len the length of the array of bytes.
 * @return A Base64 String.
 */
String base64_encode(unsigned char const *bytes_to_encode , unsigned int in_len);

/**
 * Decode a Base64 String to a normal String.
 * @param s the Base64 String to decode.
 * @return A normal String.
 */
String base64_decode(const String  &s);


#endif /* BASE64_H_ */
