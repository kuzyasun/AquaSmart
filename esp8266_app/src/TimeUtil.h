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
#include <Ticker.h>


class TimeUtil;
class TimeUtil
{
private:
	bool _updateNtpTime;
	bool _syncRtcTime;
public:
	TimeUtil();
	void printRtcDateTime(const RtcDateTime& dt);
	String getDateTimeString(const strDateTime dt);
	RtcDS1307 Rtc;
	NTPtime NTPch;
	//NTPAsyncUDP asyncTime;
	strDateTime dateTime;
	strDateTime rtcTime;
	strDateTime ntpTime;
	void setup();
	strDateTime getNtpTime();
	String getNtpTimeString();
	strDateTime getRtcTime();
	String getRtcTimeString();
	void setupRtc();
	Ticker ntmTimeUpdater;
	void updateNtpTime();
	void handle();
};
#endif
