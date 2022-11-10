
// https://randomnerdtutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/

#include <WiFiUdp.h>
#include "src/NTPClient/NTPClient.h"
#include <TimeLib.h>


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_Server_Address);

//String TimeZone = “CET-1CEST,M3.5.0,M10.5.0/3”; /* Rome https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv */


void Config_Time()
{
  timeClient.begin();
  
  //setenv("TZ",TimeZone, 1);
  //tzet();

  timeClient.setTimeOffset(1 * 60 * 60);   // Timezone: 1 * 3600 = GMT+1
  timeClient.update();

}




void Time_NTP_Update()
{
  timeClient.update();
}



bool Time_NTP_isValid()
{    
  bool time_isValid = 0;
  unsigned long epochTime = timeClient.getEpochTime();
  
  if(epochTime > 1577836800)  // check against custom date: 2020.01.01  00:00:00
    time_isValid = 1;
  else
    time_isValid = 0;  

  return time_isValid;
}



void Time_print_current_date_time() 
{
  unsigned long epochTime = timeClient.getEpochTime();

  int DayToday1 = day(epochTime) ;
  int currentMonth1 = month(epochTime);
  int currentYear1 = year(epochTime);


  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  
  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);  

  int currentHour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(currentHour);  

  int currentMinute = timeClient.getMinutes();
  Serial.print("Minutes: ");
  Serial.println(currentMinute); 
   
  int currentSecond = timeClient.getSeconds();
  Serial.print("Seconds: ");
  Serial.println(currentSecond);  

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);


  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);

  Serial.println("");

}  
