/**
 * @file DateTimeControler.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 11 nov. 2016
 */

#include "DateTimeControler.h"
#include "Flag.h"
#include "Timer.h"
#include "GsmClient.h"
#include "Debug.h"
#include <TimeLib.h>

DateTimeControler * DateTimeControler::insatnce = nullptr;

DateTimeControler & DateTimeControler::getInstance(){
	if(insatnce == nullptr){
		insatnce = new DateTimeControler();
	}
	return *insatnce;
}


DateTimeControler::DateTimeControler():
TaskBase(2, "DateTimeTask"),
state(USE_NONE),
syncTimer(3000),
gsm(GsmClient::getInstance())
{

}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

void DateTimeControler::run(){
	PRINTLN("-----------------DateTime Task Start-----------");
	setSyncProvider(getTeensy3Time);
	while(true){
		syncFlag.wait();
		if(state == USE_GPRS){
			float latitude, longtitude;
			String date, time;
			gsm.getLocationAndTime(latitude, longtitude, date ,time);
		}

		digitalClockDisplay();
		sleep(1000);
	}
}

void  DateTimeControler::onTimeout(Timer &timer){
	syncFlag.set();
}
