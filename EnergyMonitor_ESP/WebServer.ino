

void WebServer_Config()
{
  server.onNotFound(handleNotFound);
  server.on("/",Webpage_Home1);  
  server.on("/webpage1",Webpage_Home1);
  server.on("/OTA",Webpage_OTA);  
  server.on("/restart",Webpage_restart);
  server.on("/force",once_ForceEnergyStart_Update);

  server.begin();  
}


void Webpage_Home1()
{
  Prep_webpage1();
  server.send ( 200, "text/html", webpage1 );
  delay(10);
  webpage1 = "";
}

void Webpage_OTA()
{
  OTA_Mode = 1;
  server.send(200,"text/plain","OTA mode active now");
}


void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found! Send one of these: /OTA, /restart, /webpage1");
}

void Webpage_restart()
{
  server.send(200,"text/plain","Restarting ESP");
  Serial.println("Restart requested from webserver");
  delay(1000);
  ESP.restart();
  delay(1000);
  while(1);
}


void once_ForceEnergyStart_Update()
{
  ForceEnergyStart_Update =1;
  EM_DayEnergy_calc();
  ForceEnergyStart_Update =0;

  server.send(200,"text/plain","Energy values reset, check the web server");
}