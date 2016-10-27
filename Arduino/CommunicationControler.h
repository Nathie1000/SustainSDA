/*
 * CommunicationTask.h
 *
 *  Created on: 24 okt. 2016
 *      Author: Nathan
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

class CommunicationListener{
public:
	virtual ~CommunicationListener(){}
	virtual void onMessageReceived(const String &msg) = 0;
};

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

	CommunicationControler(int priority);
	void run() override;
	void send(const String& data, int type = 0);
	void addCommunicationTaskListener(CommunicationListener &communicationTaskListener);
};




#endif /* COMMUNICATIONCONTROLER_H_ */
