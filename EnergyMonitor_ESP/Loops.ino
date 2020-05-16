

void Always_loop()
{
  OTA_Check();
}


void Fast_Loop()
{
    Blynk.run();
    server.handleClient();
    PIR_Check(); 
    MQTT_loop();  
}



void Mid1_Loop()
{
	FetchEMData();
  //EM_DayEnergy_calc();    <<--------------------------------------------------
  EM_Data_Calc();  
  LCD_Update();
  REC_Device_Capture();
}



void Mid2_Loop()
{
  Check_PIR_Condition();
}



void Slow_Loop()
{  
  LCD_backlight_check();
  //MQTT_publish();           <<--------------------------------------------------

  RunOnce_30mins();
}


void VerySlow_Loop()
{
   //Blynk_Graph_DataPush();   <<--------------------------------------------------
}



void RunOnce_30mins()
{
  if(millis()/1000 - lastrun_30mins > 30 * 60 || lastrun_30mins == 0)
  {
    lastrun_30mins = millis()/1000;
        
  }
}
