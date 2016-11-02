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
	virtual void onMessageReceived(const String &msg) = 0;
};

/**
 * @class CommunicationControler
 * Task that handles all communication with remote server.
 */
class CommunicationControler : public TaskBase{
private:
	static const String URL;
	static const String KEY;

	struct Package{
		char type;
		int size;
		char data[128 + 1];
	};

	AtClient at;
	HttpClient http;
	Queue<Package> sendQueue;
	ArrayList<CommunicationListener*> communicationTaskListeners;
	Aes aes;
	GsmClient gsm;

public:
	/**
	 * Create new object.
	 * @param priority the priority of the task.
	 */
	explicit CommunicationControler(int priority);

	/**
	 * Implementation of the BaseTask interface.
	 */
	void run() override;

	/**
	 * Send data to server. This function adds the data to a inner Queue and blocks if this Queue is full.
	 * When the data will be actually send is up to the scheduler.
	 * @param data the data to be send. This may not exceed 128 bytes.
	 * @param type the type of data. Currently not uses. Default = 0.
	 */
	void send(const String& data, int type = 0);

	/**
	 * Add a CommunicationListener.
	 * @param communicationListener the CommunicationListener to be added.
	 */
	void addCommunicationListener(CommunicationListener &communicationListener);
};




#endif /* COMMUNICATIONCONTROLER_H_ */
