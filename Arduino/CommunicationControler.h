/**
 * @file CommunicationControler.h
 *
 * @author Nathan Schaaphuizen
 * @date 24 okt. 2016
 */

#ifndef COMMUNICATIONCONTROLER_H_
#define COMMUNICATIONCONTROLER_H_

#include "TaskBase.h"
#include "AtClient.h"
#include "HttpClient.h"
#include "Queue.h"
#include "ArrayList.h"
#include "Aes.h"
#include "GsmClient.h"

/**
 * @interface CommunicationListener
 * @brief Interface class to receive to incoming messages from remote server.
 */
class CommunicationListener{
public:
	/**
	 * Virtual deconstructor required by compiler, does nothing.
	 */
	virtual ~CommunicationListener(){}

	/**
	 * Prototype function triggered when a message is received from server.
	 * @param messageId the id of the message send to get this response.
	 * @param responseStatus the response status of the server. Or -1 if there was a error sending.
	 * @param response the response if there was one. If not this will be an empty String.
	 */
	virtual void onMessageReceived(long long messageId, int responseStatus, const String &response) = 0;
};

/**
 * @class CommunicationControler
 * @brief Task that handles all communication with remote server.
 *
 * All communication request are queued and will be voided if no connection to the server was established.
 * A packet both send and received from the server should not exceed 1024 bytes including encryption.
 * Keep in mind that Base64 bloats you package by 30%.
 * Communication will be done via the HTTP1.1 protocol with the body as plain text. Although keep alive can
 * technically be used it is highly recommended that both the client and server closes the connection as soon
 * as possible to save on battery life.
 */
class CommunicationControler : public TaskBase{
private:
	static CommunicationControler* instance;
	static long long globalMessageCounter;

	struct Package{
		bool post;
		long long messageId;
		CommunicationListener* callbackObj;
		String* message; 	//Don't forget to delete after reading from package!
		String* url; 		//Don't forget to delete after reading form package!
	};

	struct SmsPackage{
		String *number;  //Don't forget to delete after reading from package!
		String *text;	 //Don't forget to delete after reading from package!
	};

	HttpClient http;
	GsmClient &gsm;

	Queue<Package> sendQueue;
	Queue<SmsPackage> smsQueue;

	Aes *aes;
	String ip;
	String phoneNumber;
	String ccid;
	bool ready;

	void encrypt(String &message);
	void decrypt(String &message);

	void sendSms();
    void sendInternet();

    CommunicationControler();

public:
    /**
     * Get the CommunicationControler instance.
     * @return A reference to the CommunicationControler that can be used.
     */
    static CommunicationControler & getInstance();

	/**
	 * Implementation of the BaseTask interface.
	 */
	void run() override;

	/**
	 * Enable encryption. All messages already in queue will be encrypted before sending.
	 * The plain text will first be encrypted use a 128 bit AES key. After that the binary data
	 * will be encoded as a Base64 String and send to the server as a plain text.
	 * The response from the server is also expected to be encrypted and encoded in the same way.
	 * Note: encryption will only be applied to the body of the HTTP request.
	 * @param key the encryption key.
	 */
	void enableEncryption(const String& key);

	/**
	 * Disable encryption. All messages already in queue will no longer be encrypted before sending.
	 * The response from the server is also expected to have no encryption.
	 */
	void disableEncryption();

	/**
	 * Send a POST request to a remote server.
	 * @param url the server URL or address. May contain parameters. Will not be encrypted.
	 * @param data the body to send to the server. Will be encrypted if encryption is enabled.
	 * @param callback the callback object to deliver the server response to.
	 * @return A message id that can be used to identify the response later.
	 */
	long long sendPostRequest(const String &url, const String& data, CommunicationListener *callback = nullptr);

	/**
	 * Send a GET request to a remote server.
	 * @param url the server URL or address. May contain parameters. Will not be encrypted.
	 * @param callback the callback object to deliver the server response to.
	 * @return A message id that can be used to identify the response later.
	 */
	long long sendGetRequest(const String &url, CommunicationListener *callback = nullptr);

	/**
	 * Send a SMS.
	 * @param number the phone number to send the SMS to. Must be complete number include the country prefix.
	 * e.g. +31654650997
	 * SMSes are queued and lost if the cannot be send.
	 * @param text the SMS body.
	 */
	void sendSms(const String &number, const String &text);

	/**
	 * Get the IP address. The IP address can only be obtained when a successful connection is made.
	 * The IP address will be obtain once when the system starts.
	 * @return The IP address as a String.
	 */
	String getIpAddress();

	/**
	 * Get the phone number associated with the SIM card.
	 * Note: not all devices support this function.
	 * @return The phone number as a String.
	 */
	String getPhoneNumber();

	/**
	 * Get the ICCID (Integrated Circuit Card Identifier) associated with the SIM card.
	 * @return The ICCID as a String.
	 */
	String getCcid();
};

#endif /* COMMUNICATIONCONTROLER_H_ */
