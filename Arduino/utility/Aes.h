/**
 * @file Aes.h
 *
 * @author Nathan Schaaphuizen
 * @date 28 sep. 2016
 */

#ifndef AES_H_
#define AES_H_

#include <Arduino.h>

/**
 * @class Aes
 * Class used to encrypt data using the Advance Encryption Standard.
 * Original code can be found <a href=\"http://www.efgh.com/software/rijndael.htm\">here</a>.
 */
class Aes{
private:
	/** Macro to find the key length. */
	#define KEYLENGTH(keybits) ((keybits)/8)
	/** Macro to find the RK length. */
	#define RKLENGTH(keybits)  ((keybits)/8+28)
	/** Macro to find the amount of rounds. */
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
	/** Value used to encode with 128 bits. */
	static const unsigned int KEY_BITS_128;
	/** Value used to encode with 192 bits. */
	static const unsigned int KEY_BITS_192;
	/** Value used to encode with 256 bits. */
	static const unsigned int KEY_BITS_256;

	/**
	 * Create new object.
	 * @param password the encryption key.
	 * @param keybits the amount of bits to encode with. Valid values are: KEY_BITS_128, KEY_BITS_192 or KEY_BITS_256.
	 * Default = KEY_BITS_128
	 */
	Aes(const String &password, unsigned int keybits = KEY_BITS_128);

	/**
	 * Destory the object.
	 */
	~Aes();

	/**
	 * Encrypt data.
	 * The size of the both buffers must be the same.
	 * The size of the buffers must be a multiple of 16.
	 * @param plaintext the data to be encrypted.
	 * @param ciphertext the encrypted data.
	 * @param size size of both the plaintext and chiptertext array.
	 */
	void encrypt(unsigned char *plaintext, unsigned char *ciphertext, int size);

	/**
	 * Decrypt data.
	 * The size of the both buffers must be the same.
	 * The size of the buffers must be a multiple of 16. (This will always be the case if the data was
	 * successfully encrypted using AES.)
	 * @param ciphertext the data to be decrypted.
	 * @param plaintext the decrypted data.
	 * @param size size of both the chiptertext and plaintext array.
	 */
	void decrypt(unsigned char *ciphertext, unsigned char *plaintext, int size);

};

#endif /* AES_H_ */
