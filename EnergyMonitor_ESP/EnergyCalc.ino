

void FetchEMData()
{
  v1  = EM_Ph1.voltage();
  i1  = EM_Ph1.current();
  p1  = EM_Ph1.power();
  e1  = EM_Ph1.energy();
  pf1 = EM_Ph1.pf();
  f1  = EM_Ph1.frequency();

  if(isnan(v1))
    EM_Ph1_Error = 1;
  else
    EM_Ph1_Error = 0;
    

  v2  = EM_Ph2.voltage();
  i2  = EM_Ph2.current();
  p2  = EM_Ph2.power();
  e2  = EM_Ph2.energy();
  pf2 = EM_Ph2.pf();
  f2  = EM_Ph2.frequency();

  if(isnan(v2))
    EM_Ph2_Error = 1;
  else
    EM_Ph2_Error = 0;
    
  v3  = EM_Ph3.voltage();
  i3  = EM_Ph3.current();
  p3  = EM_Ph3.power();
  e3  = EM_Ph3.energy();
  pf3 = EM_Ph3.pf();
  f3  = EM_Ph3.frequency();

  if(isnan(v3))
    EM_Ph3_Error = 1;
  else
    EM_Ph3_Error = 0;
}



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


void EM_Data_Calc()
{
  v_avg  = (v1+v2+v3)/3;
  i_sum  = (i1+i2+i3);
  p_sum  = (p1+p2+p3);
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

  bool timeValid = Time_NTP_updateVar();
  if(timeValid)
  {
    int hourNow  = time_now.hr;  
    int DayToday = time_now.day;

    if((hourNow == 0 && !E_Calc_Complete) || ForceEnergyStart_Update) 
    {    
      MQTT_publish();
      Blynk_Graph_DataPush();
      PIR_Reset_Flags();

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
    terminal.println("WIP"); 
    terminal.flush(); 
  }
}
