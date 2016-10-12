/*
 * DiagnosticsTool.cpp
 *
 *  Created on: 21 sep. 2016
 *      Author: Nathan
 */

#include "DiagnosticsTool.h"
#include "AtClient.h"
#include "HttpClient.h"

DiagnosticsTool::DiagnosticsTool(AtClient &at, HttpClient &http):
at(at),
http(http)
{

}

void DiagnosticsTool::runDiagnostic(HardwareSerial &serial){

//	//Test AT connection
//	String response = at.execute("AT");
//	serial.println("AT Connect: " + response);
//	if(!response.equals(AtClient::AT_OK))return;
//
//	//Test pin
//	//Test signal strength
//	response = at.execute("AT+CPIN?");
//	serial.println("PIN: " + response);
//	if(response.equals(AtClient::AT_ERROR) || response.equals(AtClient::AT_TIMEOUT))return;
//
//	//Test signal strength
//	response = at.execute("AT+CSQ");
//	serial.println("Signal: " + response);
//	if(response.equals(AtClient::AT_ERROR) || response.equals(AtClient::AT_TIMEOUT))return;
//
//	//Test GPRS
//
//
//	//Test Ip
//	response = http.getIp();
//	serial.println("Ip: " + response);
//	if(response.equals(AtClient::AT_ERROR) || response.equals(AtClient::AT_TIMEOUT))return;
}


