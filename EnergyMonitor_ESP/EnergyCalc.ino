

bool EM_Data_Integrity_test(float val)
{
  if(!isnan(val))
  {
    // consider it a valid number
    if (val > -100.0 && val < 99999.0)
      return true;
  }
  return false;
}




void FetchEMData()
{
  
  float temp = 0;
  
  temp = EM_Ph1.voltage();
  if(EM_Data_Integrity_test(temp))
    v1  = temp;  
  temp = EM_Ph1.current();
  if(EM_Data_Integrity_test(temp))
    i1  = temp;  
  temp = EM_Ph1.power();
  if(EM_Data_Integrity_test(temp))
    p1  = temp;
  temp = EM_Ph1.energy();
  if(EM_Data_Integrity_test(temp))
    e1  = temp;
  temp = EM_Ph1.pf();
  if(EM_Data_Integrity_test(temp))
    pf1  = temp;
  temp = EM_Ph1.frequency();
  if(EM_Data_Integrity_test(temp))
    f1  = temp;

  if(isnan(temp))
    EM_Ph1_Error = 1;
  else
    EM_Ph1_Error = 0;
    

  temp = EM_Ph2.voltage();
  if(EM_Data_Integrity_test(temp))
    v2  = temp;  
  temp = EM_Ph2.current();
  if(EM_Data_Integrity_test(temp))
    i2  = temp;  
  temp = EM_Ph2.power();
  if(EM_Data_Integrity_test(temp))
    p2  = temp;
  temp = EM_Ph2.energy();
  if(EM_Data_Integrity_test(temp))
    e2  = temp;
  temp = EM_Ph2.pf();
  if(EM_Data_Integrity_test(temp))
    pf2  = temp;
  temp = EM_Ph2.frequency();
  if(EM_Data_Integrity_test(temp))
    f2  = temp;

  if(isnan(temp))
    EM_Ph2_Error = 1;
  else
    EM_Ph2_Error = 0;
    
  temp = EM_Ph3.voltage();
  if(EM_Data_Integrity_test(temp))
    v3  = temp;  
  temp = EM_Ph3.current();
  if(EM_Data_Integrity_test(temp))
    i3  = temp;  
  temp = EM_Ph3.power();
  if(EM_Data_Integrity_test(temp))
    p3  = temp;
  temp = EM_Ph3.energy();
  if(EM_Data_Integrity_test(temp))
    e3  = temp;
  temp = EM_Ph3.pf();
  if(EM_Data_Integrity_test(temp))
    pf3  = temp;
  temp = EM_Ph3.frequency();
  if(EM_Data_Integrity_test(temp))
    f3  = temp;

  if(isnan(temp))
    EM_Ph3_Error = 1;
  else
    EM_Ph3_Error = 0;
}


/*
void Serial_Print_EMData()
{
  Serial.println("EM Data:");
  
  if(EM_Ph1_Error)
    Serial.println("EM1 com error");
  if(EM_Ph2_Error)
    Serial.println("EM2 com error");
  if(EM_Ph3_Error)
    Serial.println("EM3 com error");

  if(EM_Ph1_Error && EM_Ph2_Error && EM_Ph3_Error)
    {}
  else
  {
    Serial.print("Voltage(R,Y,B):");
    Serial.print(v1); Serial.print(", ");
    Serial.print(v2); Serial.print(", ");
    Serial.println(v3);
  
    Serial.print("Current(R,Y,B):");
    Serial.print(i1); Serial.print(", ");
    Serial.print(i2); Serial.print(", ");
    Serial.println(i3);
  
    Serial.print("Power(R,Y,B):");
    Serial.print(p1); Serial.print(", ");
    Serial.print(p2); Serial.print(", ");
    Serial.println(p3);
  
    Serial.print("Energy(R,Y,B):");
    Serial.print(e1); Serial.print(", ");
    Serial.print(e2); Serial.print(", ");
    Serial.println(e3);
  
    Serial.print("pf(R,Y,B):");
    Serial.print(pf1); Serial.print(", ");
    Serial.print(pf2); Serial.print(", ");
    Serial.println(pf3);
  
    Serial.print("Frequency(R,Y,B):");
    Serial.print(f1); Serial.print(", ");
    Serial.print(f2); Serial.print(", ");
    Serial.println(f3);
  }
}

*/
void EM_Data_Calc()
{
  v_avg  = (v1+v2+v3)/3;
  i_sum  = (i1+i2+i3);
  p_sum  = (p1+p2+p3);
  if(v_avg==0 && i_sum==0)
    pf_avg = 0;
  else
    pf_avg = p_sum / (v_avg*i_sum);
  f_avg  = (f1+f2+f3)/3;
  e_sum_Today  = (Energy.e1_Today+Energy.e2_Today+Energy.e3_Today);
  e_sum_ThisMonth  = (Energy.e1_ThisMonth+Energy.e2_ThisMonth+Energy.e3_ThisMonth);  
}





void EM_DayEnergy_calc()
{
 if(EM_Ph1_Error && EM_Ph2_Error && EM_Ph3_Error)
    {}
 else
 {

  bool timeValid = Time_NTP_isValid();
  if(timeValid)
  {        
    // Get time
    String timeStr;
    struct tm timeinfo;
    getLocalTime(&timeinfo);

    int hourNow  = timeinfo.tm_hour;
    int minsNow = timeinfo.tm_min;     
    int DayToday = timeinfo.tm_mday;
    int currentMonth = timeinfo.tm_mon+1;
    int currentYear = timeinfo.tm_year-100;


    if((hourNow == 0 && !E_Calc_Complete) || ForceEnergyStart_Update) 
    {    
      MQTT_publish();
      //Blynk_Graph_DataPush();
      //PIR_Reset_Flags();

      EnergyStartValues.e1_DayStart = e1;
      EnergyStartValues.e2_DayStart = e2;
      EnergyStartValues.e3_DayStart = e3;    

      if(DayToday == 1 || ForceEnergyStart_Update)
        {
          EnergyStartValues.e1_MonthStart = e1;
          EnergyStartValues.e2_MonthStart = e2;
          EnergyStartValues.e3_MonthStart = e3;
        }
        
      Memory_Write();
      E_Calc_Complete = 1;
      ForceEnergyStart_Update = 0;
    }
    
    if(hourNow > 0)
      E_Calc_Complete = 0;
    
    Energy.e1_Today        = e1 - EnergyStartValues.e1_DayStart;
    Energy.e2_Today        = e2 - EnergyStartValues.e2_DayStart;
    Energy.e3_Today        = e3 - EnergyStartValues.e3_DayStart;
    
    Energy.e1_ThisMonth      = e1 - EnergyStartValues.e1_MonthStart;
    Energy.e2_ThisMonth      = e2 - EnergyStartValues.e2_MonthStart;
    Energy.e3_ThisMonth      = e3 - EnergyStartValues.e3_MonthStart;
   }
 }
}


void Memory_Read()
{
  uint addr = 250;
  struct EnergyVar2 Mem_Read_Var;

  EEPROM.get(addr,Mem_Read_Var);

  EnergyStartValues.e1_DayStart = Mem_Read_Var.e1_DayStart;
  EnergyStartValues.e2_DayStart = Mem_Read_Var.e2_DayStart;
  EnergyStartValues.e3_DayStart = Mem_Read_Var.e3_DayStart;
  EnergyStartValues.e1_MonthStart = Mem_Read_Var.e1_MonthStart;
  EnergyStartValues.e2_MonthStart = Mem_Read_Var.e2_MonthStart;
  EnergyStartValues.e3_MonthStart = Mem_Read_Var.e3_MonthStart;
 
  E_Calc_Complete = 1;

 
  Serial.println("Memory Read");
  Serial.print(Mem_Read_Var.e1_DayStart);  Serial.print("  ");
  Serial.print(Mem_Read_Var.e2_DayStart);  Serial.print("  ");
  Serial.print(Mem_Read_Var.e3_DayStart);  Serial.print("  ");
  Serial.print(Mem_Read_Var.e1_MonthStart);  Serial.print("  ");
  Serial.print(Mem_Read_Var.e2_MonthStart);  Serial.print("  ");
  Serial.print(Mem_Read_Var.e3_MonthStart);  Serial.println("  ");

}

void Memory_Write()
{    
  uint addr = 250;
  struct EnergyVar2 Mem_Write_Var;

  Mem_Write_Var.e1_DayStart = EnergyStartValues.e1_DayStart;
  Mem_Write_Var.e2_DayStart = EnergyStartValues.e2_DayStart;
  Mem_Write_Var.e3_DayStart = EnergyStartValues.e3_DayStart;
  Mem_Write_Var.e1_MonthStart = EnergyStartValues.e1_MonthStart;
  Mem_Write_Var.e2_MonthStart = EnergyStartValues.e2_MonthStart;
  Mem_Write_Var.e3_MonthStart = EnergyStartValues.e3_MonthStart;
  
  EEPROM.put(addr,Mem_Write_Var);
  EEPROM.commit();
}


void REC_Device_Capture()         // Future: for use with Data Analytics
{
  if(REC_Mode)
  {
    REC_Mode = 0;
  }
}
