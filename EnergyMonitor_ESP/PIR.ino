
void PIR_Config()
{
  pinMode(PIR_Pin, INPUT);

  Blynk.syncVirtual(V16);
  Blynk.syncVirtual(V11);
  Blynk.syncVirtual(V13);
  Blynk.syncVirtual(V17);
  Blynk.virtualWrite(V15, MorningTrigger);
  Blynk.virtualWrite(V12, EveningTrigger);
}


void PIR_Check()
{
  bool PIR_State = digitalRead(PIR_Pin);

  if(PIR_State) 
  { 
    LCD_backlight_ON();
    
    if(PIR_IFTTT_Ready)
        PIR_IFTTT_POST();

    if(MQTT_send_again_check(PIR_State))
        MQTT_publish_PIR(PIR_State);
  }
}


void Check_PIR_Condition()
{
  bool  PIR_Time_OK = false,
        Trigger_OK  = false,
        Other_OK    = false;

  // Time Check
  bool timeValid = Time_NTP_updateVar();
  
  if(timeValid)
  {
    int hr_now  = time_now.hr;
    int mins_now  = time_now.min;


      // Morning
    if(hr_now >= MorningTimeSet.StartTime_hr && hr_now <= MorningTimeSet.StopTime_hr)
      if(mins_now >= MorningTimeSet.StartTime_mins && mins_now <= MorningTimeSet.StopTime_mins)
          {
            if(Enable_Morning)  
              PIR_Time_OK = true;
            if(!MorningTrigger)
              Trigger_OK = true;
          }

    // Evening
    if(hr_now >= EveningTimeSet.StartTime_hr && hr_now <= EveningTimeSet.StopTime_hr)
      if(mins_now >= EveningTimeSet.StartTime_mins && mins_now <= EveningTimeSet.StopTime_mins)
        {
          if(Enable_Evening)
            PIR_Time_OK = true;
          if(!EveningTrigger)
            Trigger_OK = true;
        }



    // Misc
    Other_OK = false;    // Disabled here <- using MQTT and NodeRed for control


    PIR_IFTTT_Ready = PIR_Time_OK &&
                      Trigger_OK &&
                      Other_OK;
  
  }
}




// Runs once at 00:00 midnight
void PIR_Reset_Flags()
{
  MorningTrigger = 0;
  Blynk.virtualWrite(V15, MorningTrigger);
  EveningTrigger = 0;
  Blynk.virtualWrite(V12, EveningTrigger);
}



void PIR_IFTTT_POST()
{
  bool updateTerminal = true;  
  bool POST_Status1 = updateIFTTTEvent("PIR1_LivingRoom");
  bool POST_Status2 = updateIFTTTEvent("PIR1_Kitchen");
  bool POST_Status = POST_Status1 && POST_Status2;

  if(POST_Status)
    {
      bool timeValid = Time_NTP_updateVar();
      int hr_now  = time_now.hr;  
      
      if(hr_now >= MorningTimeSet.StartTime_hr && hr_now <= MorningTimeSet.StopTime_hr)
          {
            MorningTrigger = 1;
            Blynk.virtualWrite(V15, MorningTrigger);
          }
      if(hr_now >= EveningTimeSet.StartTime_hr && hr_now <= EveningTimeSet.StopTime_hr)
          {
            EveningTrigger = 1;
            Blynk.virtualWrite(V12, EveningTrigger);
          }
      Check_PIR_Condition();
    }

    if(updateTerminal) 
    { 
      terminal.println();
      //terminal.println(timeClient.getFormattedTime());
      terminal.print("IFTT - ");
      if(POST_Status)
          terminal.print("success");
      else
          terminal.print("fail");
      terminal.flush();
    }
}



bool updateIFTTTEvent(char *IFTTT_Event)
{
  bool response = false;
  const char *host = "maker.ifttt.com";
  const int httpPort = 80;
  WiFiClient client;
  
  Serial.print("Connecting to ");
  Serial.println(host);
  
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("Connection failed");
    return response;
  }
  
  String url = "/trigger/";
  url += IFTTT_Event;
  url += "/with/key/";
  url += MakerIFTTT_Key;
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  while(client.connected())
  {
    if(client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    } else 
    {
      // No data yet, wait a bit
      delay(5);
    };
    response = true;
  }
  
  Serial.println("closing connection");
  client.stop();
  return response;
}




//-------------------- PIR MQTT -----------------------------

// Motion Sensor
#define MQTT_CONFIG_PIR        "homeassistant/sensor/PIR/Lobby/config"
#define MQTT_TOPIC_STATE_PIR   "homeassistant/sensor/PIR/Lobby/"

unsigned long MQTT_PIR_heartbeat_timestamp;
unsigned long MQTT_PIR_last_ON_msg_timestamp;
bool MQTT_PIR_last_ON_msg_state;



void MQTT_publish_PIR(bool PIR_State)
{   
    client.publish(MQTT_TOPIC_STATE_PIR, PIR_State, true);
    //Serial.println(data);
}


bool MQTT_PIR_send_again_check(bool PIR_State)
{
  // higher priority to time compared to state change, 
  //  i.e. module will wait for time delay (e.g. 30sec) before sending a message again even 
  //       if the state has changed during the delay period

  if(millis()/1000 - MQTT_PIR_last_ON_msg_timestamp > 30)  // Atleast 30 sec have passsed since last transmission
  {
    //if(PIR_State != MQTT_PIR_last_ON_msg_state)
      {
        MQTT_PIR_last_ON_msg_state = PIR_State;
        MQTT_PIR_last_ON_msg_timestamp = millis()/1000;
        MQTT_PIR_heartbeat_timestamp = millis()/1000;

        return true;
      }    
  }
  return false;
}


// Acts as heartbeat and also sends zero state (no motion) 
void MQTT_PIR_heartbeat()
{
  if(millis()/1000 - MQTT_PIR_heartbeat_timestamp > 60)  // send every 60 sec
  {
    MQTT_PIR_heartbeat_timestamp = millis()/1000;
    MQTT_publish_PIR(false);
  }
}