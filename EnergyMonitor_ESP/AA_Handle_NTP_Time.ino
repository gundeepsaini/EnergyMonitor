


#include "time.h"


// Timezone Info: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define TimeZone "CET-1CEST,M3.5.0,M10.5.0/3"

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }

      // Get time
    String timeStr;
    getLocalTime(&timeinfo);

    int hourNow  = timeinfo.tm_hour;
    int minsNow = timeinfo.tm_min;     
    int DayToday = timeinfo.tm_mday;
    int currentMonth = timeinfo.tm_mon+1;
    int currentYear = timeinfo.tm_year-100;    
  
  // Convert to HH:MM:SS
  char locTime[9];
  sprintf(locTime, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  // Convert to HH:MM:SS
  char locdate[9];
    sprintf(locdate, "%02d.%02d.%02d", timeinfo.tm_year-100, timeinfo.tm_mon+1, timeinfo.tm_mday);

  Serial.print(locdate);
  Serial.print(" ");
  Serial.println(locTime);
}


void Config_Time()
{
  struct tm timeinfo;

  Serial.println("Setting up time");
  configTime(0, 0, NTP_Server_local);    // First connect to NTP server, with 0 TZ offset
  
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("  Failed to obtain time");
    return;
  }
  Serial.printf("  Setting Timezone to ", TimeZone);
  setenv("TZ", TimeZone, 1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();

  printLocalTime();

}





bool Time_NTP_isValid()
{    
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("  Failed to obtain time");
    return 0;
  }

  return 1;
}


void Time_NTP_Update()
{
  printLocalTime();
}














