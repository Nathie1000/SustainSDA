/**
 * @file CommunicationControler.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 24 okt. 2016
 */
#include "CommunicationControler.h"
#include "TaskBase.h"
#include "AtClient.h"
#include "HttpClient.h"
#include "Queue.h"
#include "ArrayList.h"
#include "Aes.h"
#include "Base64.h"
#include <math.h>
#include "Debug.h"
#include "SustainWork.h"

CommunicationControler* CommunicationControler::instance = nullptr;
long long CommunicationControler::globalMessageCounter = 0;

CommunicationControler & CommunicationControler::getInstance(){
	if(instance == nullptr){
		instance = new CommunicationControler();
	}
	return *instance;
}

CommunicationControler::CommunicationControler():
TaskBase(2, "CommunicationTask"),
http(AtClient::getInstance()),
gsm(GsmClient::getInstance()),
sendQueue(20),
smsQueue(10),
aes(nullptr),
ip("0.0.0.0"),
ready(false)
{
	pinMode(2, OUTPUT);
	digitalWrite(2, LOW);
}


void CommunicationControler::encrypt(String &message){
	//Add space padding to make the string length a multiple of 16.
	//This is required for AES.
	while(message.length() % 16 != 0){
		message += " ";
	}
	//Encrypt using AES.
	unsigned char *buffer = new unsigned char[message.length()];
	aes->encrypt((unsigned char*)message.c_str(), buffer, message.length());
	//Encode using Base64.
	message = base64_encode(buffer, message.length());
	delete buffer;
}

void CommunicationControler::decrypt(String &message){
	//Decode using Base64
	String ciphertext = base64_decode(message);
	//Decrypt using AES.
	unsigned char *decryptBuffer = new unsigned char[ciphertext.length() + 1];
	aes->decrypt((unsigned char*)ciphertext.c_str(), decryptBuffer, ciphertext.length());
	decryptBuffer[ciphertext.length()] = '\0';
	String plainResponse((const char*)decryptBuffer);
	delete decryptBuffer;
	message = plainResponse.trim();
}

void CommunicationControler::sendSms(){
	SmsPackage p = smsQueue.pop();
	String number = *p.number;
	delete p.number;
	String text = *p.text;
	delete p.text;
	if(gsm.isDeviceOpen()){
		if(!gsm.sendSms(number, text)){
			PRINTLN("Error Sensing SMS.");
		}
	}
	else{
		PRINTLN("Error AT device not connected.");
	}
}

void CommunicationControler::sendInternet(){
	Package p = sendQueue.pop();
	String message = *p.message;
	delete p.message;
	String url = *p.url;
	delete p.url;
	if(http.isDeviceOpen()){
		if(!http.isConnected()){
			for(int i=0; i<3 && !http.connect(); i++){
				sleep(3000);
			}
		}

		if(http.isConnected()){
			//Encrypt if necessary.
			if(aes != nullptr){
				encrypt(message);
			}
			//Post or get data.
			String rsp;
			if(p.post){
				rsp = http.post(url, message, HttpClient::CONTENT_TYPE_PLAIN_TEXT);
			}
			else{
				rsp = http.get(url);
			}
			int status = http.getStatus();

			//Decrypt if necessary.
			if(aes != nullptr && rsp.length() > 0){
				decrypt(rsp);
			}

			if(p.callbackObj != nullptr){
				p.callbackObj->onMessageReceived(p.messageId, status, rsp);
			}
		}
		else{
			PRINTLN("Error no Internet connection.")
		}
	}
	else{
		PRINTLN("Error AT device not connected.");
	}
}

void CommunicationControler::run(){
	PRINTLN("-----------------Communication Task Start-----------");
	if(http.openDevice()){
		//Set the pin code if needed.
		if(gsm.getPinState() == GsmClient::SIM_PIN){
			PRINTLN("PIN required, setting pin code '0000'.")
			if(!gsm.setPinCode("0000")){
				PRINTLN("Failed to set PIN.");
			}
		}
		//Try to connect to Internet a few times.
		for(int i=0; i<5 && !http.connect(); i++){
			sleep(1000);
		}
		//Get the IP address.
		if(http.isConnected()){
			ip = http.getIp();
		}
		ready = true;
	}
	else{
		PRINTLN("No AT device found, Communication Task suspend.");
		suspend();
	}

	PRINTLN("IS this working????");

	while(true){
		const Waitable *w = wait(smsQueue | sendQueue);
		if(w == &smsQueue){
			sendSms();
		}
		else{
			sendInternet();
		}
	}
}

void CommunicationControler::enableEncryption(const String& key){
	if(aes != nullptr){
		delete aes;
	}
	aes = new Aes(key);
}

void CommunicationControler:: disableEncryption(){
	delete aes;
}

long long CommunicationControler::sendPostRequest(const String &url, const String& data, CommunicationListener *callback){
	Package p;
	p.callbackObj = callback;
	p.message = new String(data);
	p.url = new String(url);
	p.messageId = globalMessageCounter++;
	p.post = true;
	//Do not push to queue if task is suspended to prevent the queue form filling and blocking the whole system.
	if(ready)sendQueue.push(p);
	return p.messageId;
}

long long CommunicationControler::sendGetRequest(const String &url, CommunicationListener *callback){
	Package p;
	p.callbackObj = callback;
	p.message = new String();
	p.url = new String(url);
	p.messageId = globalMessageCounter++;
	p.post = false;
	//Do not push to queue if task is suspended to prevent the queue form filling and blocking the whole system.
	if(ready)sendQueue.push(p);
	return p.messageId;
}

void CommunicationControler::sendSms(const String &number, const String &text){
	SmsPackage p;
	p.number = new String(number);
	p.text = new String(text);
	//Do not push to queue if task is suspended to prevent the queue form filling and blocking the whole system.
	smsQueue.push(p);
}

String CommunicationControler::getIpAddress(){
	return ip;
}
