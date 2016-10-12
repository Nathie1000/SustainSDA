/*
 * Rijndael.h
 *
 *  Created on: 28 sep. 2016
 *      Author: Nathan
 */

#ifndef AES_H_
#define AES_H_

#include <Arduino.h>

//http://www.efgh.com/software/rijndael.htm
class Aes{
private:
	#define KEYLENGTH(keybits) ((keybits)/8)
	#define RKLENGTH(keybits)  ((keybits)/8+28)
	#define NROUNDS(keybits)   ((keybits)/32+6)

	unsigned long int *encryptRk;
	unsigned long int *decryptRk;
	unsigned char *key;

	int encryptRounds;
	int decryptRounds;

	int rijndaelSetupEncrypt(unsigned long *rk, const unsigned char *key, int keybits);
	int rijndaelSetupDecrypt(unsigned long *rk, const unsigned char *key, int keybits);
	void rijndaelEncrypt(const unsigned long *rk, int nrounds, const unsigned char plaintext[16], unsigned char ciphertext[16]);
	void rijndaelDecrypt(const unsigned long *rk, int nrounds, const unsigned char ciphertext[16], unsigned char plaintext[16]);


public:
	static const unsigned int KEY_BITS_128;
	static const unsigned int KEY_BITS_192;
	static const unsigned int KEY_BITS_256;

	Aes(const String &password, unsigned int keybits = KEY_BITS_128);
	~Aes();

	void encrypt(unsigned char *plaintext, unsigned char *ciphertext, int size);
	void decrypt(unsigned char *ciphertext, unsigned char *plaintext, int size);

};

#endif /* AES_H_ */
