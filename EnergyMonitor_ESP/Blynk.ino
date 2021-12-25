


//---------------------------------------------

void Blynk_Config()
{
  Blynk.config(BlynkAuth);
  while (Blynk.connect(1000) == false);  
  lcd.print(" Blynk");  
 
}


void Blynk_Graph_DataPush()
{
  Blynk.virtualWrite(V2, i_sum);
  Blynk.virtualWrite(V3, p_sum);
  Blynk.virtualWrite(V4, e_sum_Today);
  Blynk.virtualWrite(V7, e_sum_ThisMonth);
}


/**********************************************************/


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
