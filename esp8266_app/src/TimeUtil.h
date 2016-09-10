#ifndef TimeUtil_h
#define TimeUtil_h
#define AQUASMART_H
#include "Arduino.h"
#include <RtcDateTime.h>
#include <RtcDS1307.h>
#include <RtcTemperature.h>
#include <RtcUtility.h>
#include <Wire.h>
#include <NTPtimeESP.h>
#include <time.h>
#include "Constants.h"


class TimeUtil;
class TimeUtil
{
private:
public:	
	TimeUtil();
	void printRtcDateTime(const RtcDateTime& dt);
	String getDateTimeString(const strDateTime dt);
	RtcDS1307 Rtc;
	NTPtime NTPch;
	//NTPAsyncUDP asyncTime;
	strDateTime dateTime;
	void setup();
	strDateTime getNtpTime();
	String getNtpTimeString();
	strDateTime getRtcTime();
	String getRtcTimeString();
	void setupRtc();
};
#endif
