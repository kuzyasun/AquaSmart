#include "TimeUtil.h"


#define countof(a) (sizeof(a) / sizeof(a[0]))

void ntpTimeUpdate(TimeUtil * time)
{
	time->getNtpTime();
}

void TimeUtil::printRtcDateTime(const RtcDateTime& dt)
{
	char datestring[20];

	snprintf_P(datestring,
		countof(datestring),
		PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
		dt.Month(),
		dt.Day(),
		dt.Year(),
		dt.Hour(),
		dt.Minute(),
		dt.Second());
	Serial.println(datestring);
}

String TimeUtil::getDateTimeString(const strDateTime dt)
{
	char datestring[20];

	snprintf_P(datestring,
		countof(datestring),
		PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
		dt.day,
		dt.month,
		dt.year,
		dt.hour,
		dt.minute,
		dt.second);

	return String(datestring);
}

//time-d.nist.gov
//ch.pool.ntp.org
//time.nist.gov
TimeUtil::TimeUtil():
	NTPch("ch.pool.ntp.org")
	//asyncTime("ch.pool.ntp.org")
{

}

void TimeUtil::setup()
{
	setupRtc();
}

strDateTime TimeUtil::getNtpTime()
{
	// first parameter: Time zone in floating point (for India); second parameter: 1 for European summer time; 2 for US daylight saving time (not implemented yet)
	dateTime = NTPch.getNTPtime(2.0, 1);
	//dateTime = asyncTime.getNTPtime(1.0, 1);
	NTPch.printDateTime(dateTime);
	ntpTime = dateTime;
	return dateTime;
}

String TimeUtil::getNtpTimeString()
{
	return getDateTimeString(getNtpTime());
}

void TimeUtil::updateNtpTime()
{
	ntmTimeUpdater.once(3, ntpTimeUpdate, this);
}



strDateTime TimeUtil::getRtcTime()
{
	RtcDateTime now = Rtc.GetDateTime();
	strDateTime ctime;

	ctime.year = now.Year();
	ctime.month = now.Month();
	ctime.day = now.Day();
	ctime.dayofWeek = now.DayOfWeek();
	ctime.hour = now.Hour();
	ctime.minute = now.Minute();
	ctime.second = now.Second();
	rtcTime = ctime;
	return ctime;
}

String TimeUtil::getRtcTimeString()
{
	return getDateTimeString(getRtcTime());
}

void TimeUtil::setupRtc() {
	//--------RTC SETUP ------------
	Serial.print("Compiled date:");
	//SDA SCL
	Rtc.Begin(PIN_RTC_SDA, PIN_RTC_SCL);
	// if you are using ESP-01 then uncomment the line below to reset the pins to
	// the available pins for SDA, SCL
	// Wire.begin(0, 2); // due to limited pins, use pin 0 and 2 for SDA, SCL
	RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
	printRtcDateTime(compiled);

	if (!Rtc.IsDateTimeValid())
	{
		// Common Cuases:
		//    1) first time you ran and the device wasn't running yet
		//    2) the battery on the device is low or even missing
		Serial.println("RTC lost confidence in the DateTime! Setting compilation time.");
		// following line sets the RTC to the date & time this sketch was compiled
		// it will also reset the valid flag internally unless the Rtc device is
		// having an issue
		Rtc.SetDateTime(compiled);
	}

	if (!Rtc.GetIsRunning())
	{
		Serial.println("RTC was not actively running, starting now");
		Rtc.SetIsRunning(true);
	}

	RtcDateTime now = Rtc.GetDateTime();
	Serial.print("RTC time:  ");
	printRtcDateTime(now);
	if (now < compiled)
	{
		Serial.println("RTC is older than compile time!  (Updating DateTime)");
		Rtc.SetDateTime(compiled);
	}
	else if (now > compiled)
	{
		Serial.println("RTC is newer than compile time. (this is expected)");
	}
	else if (now == compiled)
	{
		Serial.println("RTC is the same as compile time! (not expected but all is fine)");
	}
	// never assume the Rtc was last configured by you, so
	// just clear them to your needed state
	Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);
}
