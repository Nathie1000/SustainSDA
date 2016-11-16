/*
 * DiagnosticsTool.h
 *
 *  Created on: 21 sep. 2016
 *      Author: Nathan
 *
 */
////\cond
#ifndef DIAGNOSTICSTOOL_H_
#define DIAGNOSTICSTOOL_H_

#include "AtClient.h"
#include "HttpClient.h"

class DiagnosticsTool{
private:
	AtClient &at;
	HttpClient &http;

public:
	DiagnosticsTool(AtClient &at, HttpClient &http);

	void runDiagnostic(HardwareSerial &serial);
};




#endif /* DIAGNOSTICSTOOL_H_  */
//// \endcond
