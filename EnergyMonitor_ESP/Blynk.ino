

//---------------------------------------------

void Blynk_Config()
{
  Blynk.config(BlynkAuth);
  while (Blynk.connect(1000) == false);  
  lcd.print(" Blynk");  
  
  terminal.println();   
  terminal.print("Restarted...");
  terminal.println();
  //terminal.print(" - ");
  //terminal.println(WiFi.localIP());
  terminal.println("Additional: OTA"); 
  terminal.flush(); 
}


void Blynk_Graph_DataPush()
{
  Blynk.virtualWrite(V2, i_sum);
  Blynk.virtualWrite(V3, p_sum);
  Blynk.virtualWrite(V4, e_sum_Today);
  Blynk.virtualWrite(V7, e_sum_ThisMonth);
}


/**********************************************************/


BLYNK_WRITE(V0)
{
// Terminal
  String Data = param.asStr();
  if(Data == String("OTA")) 
      OTA_Mode = 1;
  if(Data == String("REC"))       // Future: for use with Data Analytics
      REC_Mode = 1;
}

BLYNK_READ(V1)
{
  Blynk.virtualWrite(V1, v_avg);
}

BLYNK_READ(V2)
{
  Blynk.virtualWrite(V2, i_sum);
}

BLYNK_READ(V3)
{
  Blynk.virtualWrite(V3, p_sum);
}

BLYNK_READ(V4)
{
  Blynk.virtualWrite(V4, e_sum_Today);
}

BLYNK_READ(V5)
{
  Blynk.virtualWrite(V5, pf_avg);
}

BLYNK_READ(V6)
{
  Blynk.virtualWrite(V6, f_avg);
}

BLYNK_READ(V7)
{
  Blynk.virtualWrite(V7, e_sum_ThisMonth);
}






//---------------------- PIR ---------------------


BLYNK_WRITE(V11)
{
  TimeInputParam t(param);
  
  if (t.hasStartTime() && t.hasStopTime())
  {
    EveningTimeSet.StartTime_hr = t.getStartHour();
    EveningTimeSet.StartTime_mins = t.getStartMinute();
    EveningTimeSet.StopTime_hr = t.getStopHour();
    EveningTimeSet.StopTime_mins = t.getStopMinute();
    
    Serial.print(EveningTimeSet.StartTime_hr);
    Serial.print(":");
    Serial.print(EveningTimeSet.StartTime_mins);
    Serial.print(" - ");
    Serial.print(EveningTimeSet.StopTime_hr);
    Serial.print(":");
    Serial.println(EveningTimeSet.StopTime_mins);
  }
}


BLYNK_READ(V12)
{
  Blynk.virtualWrite(V12, EveningTrigger);
}

BLYNK_WRITE(V13)
{
  int input13 = param.asInt();
  if(input13 > 0)
    Enable_Evening = 1;
  else
    Enable_Evening = 0;
}

BLYNK_WRITE(V14)
{
  int Reset = param.asInt();
  if(Reset > 0)
    PIR_Reset_Flags();
}

BLYNK_READ(V15)
{
  Blynk.virtualWrite(V15, MorningTrigger);
}

BLYNK_WRITE(V16)
{
  TimeInputParam t(param);
  
  if (t.hasStartTime() && t.hasStopTime())
  {
    MorningTimeSet.StartTime_hr = t.getStartHour();
    MorningTimeSet.StartTime_mins = t.getStartMinute();
    MorningTimeSet.StopTime_hr = t.getStopHour();
    MorningTimeSet.StopTime_mins = t.getStopMinute();
    
    Serial.print(MorningTimeSet.StartTime_hr);
    Serial.print(":");
    Serial.print(MorningTimeSet.StartTime_mins);
    Serial.print(" - ");
    Serial.print(MorningTimeSet.StopTime_hr);
    Serial.print(":");
    Serial.println(MorningTimeSet.StopTime_mins);
  }
}

BLYNK_WRITE(V17)
{
  int input17 = param.asInt();
  if(input17 > 0)
    Enable_Morning = 1;
  else
    Enable_Morning = 0;
}
