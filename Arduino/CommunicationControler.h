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
 * Interface class to receive to incoming messages from remote server.
 */
class CommunicationListener{
public:
	/**
	 * Virtual deconstructor required by compiler, does nothing.
	 */
	virtual ~CommunicationListener(){}

	/**
	 * Prototype function triggered when a message is received from server.
	 * @param msg the message.
	 */
	virtual void onMessageReceived(long long messageId, int responseStatus, const String &response) = 0;
};

/**
 * @class CommunicationControler
 * Task that handles all communication with remote server.
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
	 * The response from the server is also expected to be encrypted and encodes in the same way in the same way.
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
	 */
	long long sendPostRequest(const String &url, const String& data, CommunicationListener *callback = nullptr);
	long long sendGetRequest(const String &url, CommunicationListener *callback = nullptr);

	void sendSms(const String &number, const String &text);
	String getIpAddress();
	bool isReady();

};

#endif /* COMMUNICATIONCONTROLER_H_ */
