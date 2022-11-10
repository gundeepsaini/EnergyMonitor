
void PIR_Config()
{
  pinMode(PIR_Pin, INPUT);
}


bool last_PIR_State = false;
bool PIR_State;
long PIR_check_tmr = 0;


void PIR_Check()
{
  PIR_State = digitalRead(PIR_Pin);

  
  if(PIR_State == 1 && PIR_check_tmr == 0)
  {
    PIR_check_tmr = millis();
  }
  else
  {
    if(PIR_State == 0)
        PIR_check_tmr = 0;
  }


  if(PIR_State == 1)
  {
    // PIR has given on signal for atleast 500ms. Only send trigger for the first 5 sec of motion. 
    //if (millis() - PIR_check_tmr > 500 ) //&& millis() - PIR_check_tmr < 5000 )  
         LCD_backlight_ON();                  
   
    if(MQTT_PIR_send_again_check(PIR_State))
         MQTT_publish_PIR(PIR_State);  
   }

}