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

	AtClient at;
	HttpClient http;
	Queue<Package> sendQueue;
	Queue<SmsPackage> smsQueue;

	Aes *aes;
	GsmClient gsm;
	String ip;
	bool ready;

	CommunicationControler();
	void encrypt(String &message);
	void decrypt(String &message);

	void sendSms();
    void sendInternet();

public:
	static CommunicationControler& getInstance();

	/**
	 * Implementation of the BaseTask interface.
	 */
	void run() override;

	void enableEncryption(const String& key);
	void disableEncryption();

	long long sendPostRequest(const String &url, const String& data, CommunicationListener *callback = nullptr);
	long long sendGetRequest(const String &url, CommunicationListener *callback = nullptr);

	void sendSms(const String &number, const String &text);
	String getIpAddress();
	bool isReady();

};

#endif /* COMMUNICATIONCONTROLER_H_ */
