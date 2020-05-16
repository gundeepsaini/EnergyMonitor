
void LCD_Update()
{
	// ---- ---- ---- ----
	// ---- ---- ---- ----

	lcd.clear();
	
	lcd.setCursor(0,0);
  	lcd.print("V:");
  	lcd.print(int(v_avg));   
  
	lcd.setCursor(8,0);
  	lcd.print("I:");
  	lcd.print(i_sum);

	lcd.setCursor(0,1);
  	lcd.print("P:");
  	lcd.print(int(p_sum)); 

	lcd.setCursor(8,1);
	lcd.print("PF:");
	lcd.print(pf_avg);	
}


void LCD_backlight_check()
{
  if(LCD_backlight_TMR > 0 &&
     millis()/1000 - LCD_backlight_TMR > 30 )
  {
     lcd.noBacklight();
     LCD_backlight_TMR = 0;
  }
}


void LCD_backlight_ON()
{
  lcd.backlight();
  LCD_backlight_TMR = millis()/1000;
}
