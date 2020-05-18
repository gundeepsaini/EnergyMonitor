/*-----------------------------------------

            Energy Monitor

Original Date: Dec 2019 (v01)
Device: ESP8266 NodeMcU
Block Diagram & Circuit Diagram: OneNote Workbook

Pin Connections:
  EM UART Arduino Rx/Tx   : 4/5
  LCD (I2C)               : 12/14
  Push Button             : TO-DO
  PIR Motion Sensor       : 13

Note:
  - Reset ESP after upload, disable IPv6, reset adapter, allow port in firewall
  - send "OTA" on blynk terminal to enter dedicated mode
    or navigate to "ip/OTA" for OTA through web portal
  
To-Do:
  - Optimze and use structures to read and store values
 

Changes:
  - 16.05.2020
    Created separate Github Project, removed version numbers
    Changed time handling to include DST effects (day light saving)    
    Minor code standardization
    Motion Sensor (PIR) changed to independent MQTT device within EnergyMonitor (handled in NodeRed)  
    TODO: Photos/Videos to be added

  - 22.03.2020
    Updated code and moved to Github
    Shifted personal config data to Secret.h file

  - 27.02.2020
    Added PIR Sensor output to trigger IFTTT functions
   
  - 01.02.2020
    Updated version for RPi
    Change of webpage format to table
        
  - 25.01.2020
    Energy Calculation revamped
    LCD switched off after 1 short loop

  - 13.12.2019
    Ongoing changes

------------------------------------------- */


/* ------------- LIB ------------------------------ */
#include "Secrets.h"
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <PZEM004Tv30.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
//#include <TimeLib.h>
//#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>
//#include <WidgetTimeInput.h>
//#include <WidgetRTC.h>


/* ------------- CONFIG VAR ------------------------------ */
unsigned long looptime_Fast = 0;    // in secs
unsigned long looptime_Mid1 = 2;    // in secs
unsigned long looptime_Mid2 = 10;   // in secs
unsigned long looptime_Slow       = 30;      // in secs
unsigned long looptime_VerySlow   = 15 *(60);      // in secs

// Only Enable if energy values stored at 00:00 have to be reset to current
bool ForceEnergyStart_Update = false;      

#define PIR_Pin 13  
#define Rx_Pin 4                  // To Tx of EM
#define Tx_Pin 5                  // To Rx of EM
#define Addr_Ph1 0xF1             // Address of Energy Meter
#define Addr_Ph2 0xF2
#define Addr_Ph3 0xF3   



/* ------------- VAR ------------------------------ */
const char* OTA_Password     = SECRET_Device_OTA_PASS; 
const char* ssid             = SECRET_WIFI_SSID2;
const char* pass             = SECRET_WIFI_PASS2;
const char* DeviceHostName   = SECRET_Device_Name1;
const char* BlynkAuth        = SECRET_BLYNK_AUTH1;
const char* MakerIFTTT_Key   = SECRET_MakerIFTTT_Key1;

float v1,v2,v3,v_avg;     
float i1,i2,i3,i_sum;
float p1,p2,p3,p_sum;     
float e1,e2,e3,e_sum_Today,e_sum_ThisMonth;
float pf1,pf2,pf3,pf_avg,pf_max;  
float f1,f2,f3,f_avg;
bool EM_Ph1_Error,EM_Ph2_Error,EM_Ph3_Error;

unsigned long lastrun_fast, lastrun_Mid1;
unsigned long lastrun_Mid2, lastrun_slow, lastrun_Veryslow;
unsigned long lastrun_30mins,lastrun_OTA;
unsigned long LCD_backlight_TMR;
bool OTA_Mode=0;
String webpage1="";
bool E_Calc_Complete=0,REC_Mode=0;
bool MorningTrigger, EveningTrigger, PIR_IFTTT_Ready;
bool Enable_Evening, Enable_Morning;

PZEM004Tv30 EM_Ph1(Rx_Pin, Tx_Pin, Addr_Ph1); 
PZEM004Tv30 EM_Ph2(Rx_Pin, Tx_Pin, Addr_Ph2);  
PZEM004Tv30 EM_Ph3(Rx_Pin, Tx_Pin, Addr_Ph3);  
//(lcd_Addr,En,Rw,Rs,d4,d5,d6,d7,backlighPin,t_backlighPol)
LiquidCrystal_I2C lcd(0x27,5,6,7,4,3,2,1,0,POSITIVE);
WidgetTerminal terminal(V0);
ESP8266WebServer server(80);

//TODO: Optimize these structures
struct OprTime
{ 
    int StartTime_hr;
    int StartTime_mins;
    int StopTime_hr;
    int StopTime_mins;
    bool DayTriggered;
};
struct EnergyVar
  { 
    float e1_Today;
    float e2_Today;
    float e3_Today;
    float e1_ThisMonth;
    float e2_ThisMonth;
    float e3_ThisMonth;
  };
struct EnergyVar2
  { 
    float e1_DayStart;
    float e2_DayStart;
    float e3_DayStart;
    float e1_MonthStart;
    float e2_MonthStart;
    float e3_MonthStart;
  };
struct EnergyVar Energy;
struct EnergyVar2 EnergyStartValues;
struct OprTime MorningTimeSet, EveningTimeSet;
struct time_struct
{
  bool valid;
  bool DST;  
  int hr;
  int min;
  int sec;
  int day;
  int month;
  int year;
  int yearDay;
  int weekDay;
};
time_struct time_now;


//------------------------------------------------------------


void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.begin(115200);
  Serial.println(DeviceHostName);

  Wire.begin(14,12); 
  lcd.begin(16,2);
  lcd.clear();
  LCD_backlight_ON();
  lcd.print(DeviceHostName);

  EEPROM.begin(512);

  wifi_station_set_hostname(DeviceHostName);
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, pass);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Serial.println("Wifi Error! Rebooting in 30s...");
    delay(30 *1000);
    digitalWrite(LED_BUILTIN, HIGH);
    ESP.restart();
    while(1);
  }
  
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  lcd.setCursor(0,1);
  lcd.print("Wifi");  

  Memory_Read();
  Time_NTP_Config();
  Blynk_Config();  
  WebServer_Config();
  OTA_Config();
  PIR_Config();
  MQTT_config();
  LCD_backlight_ON();
  
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Ready");
}


void loop() 
{
  // Always
  Always_loop();

  // Fast Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_fast > looptime_Fast) || lastrun_fast ==0))
  {
    lastrun_fast = millis()/1000;
    Fast_Loop();
  }

  // Mid1 Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Mid1 > looptime_Mid1) || lastrun_Mid1 ==0))
  {
    lastrun_Mid1 = millis()/1000;
    Mid1_Loop();
  }

  // Mid2 Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Mid2 > looptime_Mid2) || lastrun_Mid2 ==0))
  {
    lastrun_Mid2 = millis()/1000;
    Mid2_Loop();
  }

  // Slow Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_slow > looptime_Slow) || lastrun_slow ==0))
  {
    lastrun_slow = millis()/1000;
    Slow_Loop();
  }

    // Very Slow Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Veryslow > looptime_VerySlow) || lastrun_Veryslow ==0))
  {
    lastrun_Veryslow = millis()/1000;
    VerySlow_Loop();
  }
}
