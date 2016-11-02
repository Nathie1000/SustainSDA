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

const String CommunicationControler::URL = "http://sustain.net23.net/echo.php";
//const String CommunicationControler::URL = "http://doestnotexits.net/echo2.php";
//const String CommunicationControler::URL = "http://sustain.net23.net/echo2.php";
const String CommunicationControler::KEY = "password";

CommunicationControler::CommunicationControler(int priority):
TaskBase(priority, "CommunicationTask"),
at(Serial1, 9600),
http(at),
sendQueue(10),
aes(KEY),
gsm(at)
{
	pinMode(2, OUTPUT);
	digitalWrite(2, LOW);
}

void CommunicationControler::run(){
	PRINTLN("-----------------Communication Task Start-----------");
	if(at.connect()){
		String rdy;
		PRINTLN(String(at.execute("AT", rdy)) + " | " + rdy);

		PRINTLN(String("PIN: ") + gsm.getPinState());
		PRINTLN(String("Signal Quality: ") + gsm.getSignalQuality());

		float lon, lat;
		String date, time;
		if(gsm.getLocationAndTime(lat, lon, date, time)){
			PRINT(String("Lon: ") + String(lon, 6));
			PRINT(String("Lat: ") + String(lat,6));;
			PRINTLN(String("Date: ") + date);
			PRINTLN(String("Time: ") + time);
		}

		for(int i=0; i<5 && !http.connect(); i++){
			sleep(1000);
		}
		if(http.isConnected()){
			PRINTLN(String("IP: ") + http.getIp());
		}

		PRINTLN("Done");
		//Serial3.println("HTML:\r" + http.post("http://sustain.net23.net/echo.php?test=9", "test2=5", HttpClient::CONTENT_TYPE_POST));

	}
	else{
		PRINTLN("AT failed");
	}

	while(true){
		Package p = sendQueue.pop();

		if(at.connect()){
			if(!http.isConnected()){
				for(int i=0; i<3 && !http.connect(); i++){
					sleep(3000);
				}
			}

			if(http.isConnected()){
				String plainText(p.data);
				//Add space padding to make the string length a multiple of 16.
				//This is required for AES.
				while(plainText.length() % 16 != 0){
					plainText += " ";
				}
				unsigned char buffer[plainText.length()];
				aes.encrypt((unsigned char*)plainText.c_str(), buffer, plainText.length());
				String base64Ciphertext = base64_encode(buffer, plainText.length());

				//Post data.
				String rsp = http.post(URL, p.data /* base64Ciphertext  */);
				int status = http.getStatus();

				if(status != 200){
					PRINTLN(String("External server error: ") + status);
				}
				else if(rsp.length() > 0){
					String ciphertext = base64_decode(rsp);
					unsigned char decryptBuffer[ciphertext.length() + 1];
					aes.decrypt((unsigned char*)ciphertext.c_str(), decryptBuffer, ciphertext.length());
					decryptBuffer[ciphertext.length()] = '\0';
					String plainResponse((const char*)decryptBuffer);
					plainResponse = plainResponse.trim();

					for(CommunicationListener *com : communicationTaskListeners){
						com->onMessageReceived(rsp /* plainResponse */);
					}
				}
			}
			else{
				PRINTLN("Error no Internet connection.")
			}
		}
		else{
			PRINTLN("Error AT not connected.");
		}
	}
}

void CommunicationControler::send(const String& data, int type){
	Package p;
	p.type = type;
	p.size = min(data.length(), 128U);
	p.data[p.size] = '\0';

	for(int i=0; i<p.size ; i++){
		p.data[i] = data[i];
	}

	sendQueue.push(p);
}

void CommunicationControler::addCommunicationListener(CommunicationListener &communicationListener){
	communicationTaskListeners.add(&communicationListener);
}
