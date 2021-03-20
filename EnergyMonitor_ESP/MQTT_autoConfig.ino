

/******************** LIB **************************************/
#define MQTT_MAX_PACKET_SIZE 1024  // < ----- Change in lib: This is because the defaul maxium length is 128b. So just go to PubSubClient.h and change #define MQTT_MAX_PACKET_SIZE 128 to #define MQTT_MAX_PACKET_SIZE 1024
#include <PubSubClient.h>
#include <ArduinoJson.h>


/********************* Var *************************************/
WiFiClient espClient;
PubSubClient client(espClient);

long lastReconnectAttempt = 0;


/******************** Config **************************************/
const char* mqtt_server     = SECRET_MQTT_Server;
const char* mqtt_user       = SECRET_MQTT_User;
const char* mqtt_password   = SECRET_MQTT_Pass;


#define MQTT_CONFIG_Vavg      "homeassistant/sensor/EnergyMonitor/Vavg/config"
#define MQTT_CONFIG_Isum      "homeassistant/sensor/EnergyMonitor/Isum/config"
#define MQTT_CONFIG_Psum      "homeassistant/sensor/EnergyMonitor/Psum/config"
#define MQTT_CONFIG_Etoday    "homeassistant/sensor/EnergyMonitor/Etoday/config"
#define MQTT_CONFIG_Emonth    "homeassistant/sensor/EnergyMonitor/Emonth/config"
#define MQTT_CONFIG_Etotal    "homeassistant/sensor/EnergyMonitor/Etotal/config"
#define MQTT_CONFIG_PF        "homeassistant/sensor/EnergyMonitor/PF/config"
#define MQTT_CONFIG_FREQ      "homeassistant/sensor/EnergyMonitor/FREQ/config"

#define MQTT_TOPIC_STATE      "HA/EnergyMonitor/state"


// Will Topic - Availability
#define MQTT_TOPIC_WILL       "HA/EnergyMonitor/status"     
#define MQTT_OFFLINE          "Offline"
#define MQTT_ONLINE           "Active"


/**************** External Functions ************************************/

void MQTT_config()
{
	client.setServer(mqtt_server, 1883);
 	client.setCallback(MQTT_MessageRecd_callback);
}


void MQTT_loop()
{
	if (!client.connected())
    	MQTT_reconnect();              
  
  	client.loop();
}


void MQTT_publish()
{   
      // Use arduinojson.org/v6/assistant to compute the capacity.
      const size_t capacity = JSON_OBJECT_SIZE(12);
      DynamicJsonDocument doc(capacity);
      
      doc["Vavg"]     = String(v_avg);
      doc["Isum"]     = String(i_sum);
      doc["Psum"]     = String(p_sum);
      doc["Etoday"]   = String(e_sum_Today);
      doc["Emonth"]   = String(e_sum_ThisMonth);
      doc["Etotal"]   = String(e1+e2+e3);
      doc["Freq"]     = String(f_avg);
      doc["PFavg"]    = String(pf_avg);
      
      char data[256];
      serializeJson(doc, data, sizeof(data));
      client.publish(MQTT_TOPIC_STATE, data, true);
      Serial.println(data);

      MQTT_publish_Deep_Dive();
}


/**************** Internal Functions ************************************/

void MQTT_reconnect() 
{
  if (millis()/1000 - lastReconnectAttempt > 30 || lastReconnectAttempt == 0) 
  {
      Serial.println("MQTT reconnecting");
      
      //boolean connect (clientID, [username, password], [willTopic, willQoS, willRetain, willMessage], [cleanSession])
      if (client.connect(DeviceHostName, mqtt_user, mqtt_password, MQTT_TOPIC_WILL, 1, true, MQTT_OFFLINE)) 
      {
        //MQTT_publish_config_Vavg();
        //MQTT_publish_config_Isum();
        //MQTT_publish_config_Psum();
        //MQTT_publish_config_Etoday();
        //MQTT_publish_config_Emonth();
        //MQTT_publish_config_Etotal();
        //MQTT_publish_config_PF();
        //MQTT_publish_config_FREQ();
        //client.subscribe("inTopic"); 

        Serial.println("MQTT connected");
        client.publish(MQTT_TOPIC_WILL, MQTT_ONLINE, true);
      }      
      lastReconnectAttempt = millis()/1000;
  }
}




void MQTT_MessageRecd_callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) 
    Serial.print((char)payload[i]);
  Serial.println();
}



void MQTT_publish_config_Vavg() 
{
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const int capacity = JSON_OBJECT_SIZE(5); //  <-------- Change it as reqd
  StaticJsonDocument<capacity> root;  

  root["name"] = "Voltage Avg";
  root["stat_t"] = MQTT_TOPIC_STATE;
  root["unit_of_meas"] = "V";
  root["val_tpl"] = "{{value_json.Vavg}}";
  
  char message[256];
  serializeJson(root, message,sizeof(message));
  client.publish(MQTT_CONFIG_Vavg, message, true);
  //Serial.println(message);
}


void MQTT_publish_config_Isum() 
{
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const int capacity = JSON_OBJECT_SIZE(5); //  <-------- Change it as reqd
  StaticJsonDocument<capacity> root;  

  root["name"] = "Current Sum";
  root["stat_t"] = MQTT_TOPIC_STATE;
  root["unit_of_meas"] = "A";
  root["val_tpl"] = "{{value_json.Isum}}";
  
  char message[256];
  serializeJson(root, message,sizeof(message));
  client.publish(MQTT_CONFIG_Isum, message, true);
  //Serial.println(message);
}

void MQTT_publish_config_Psum() 
{
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const int capacity = JSON_OBJECT_SIZE(5); //  <-------- Change it as reqd
  StaticJsonDocument<capacity> root;  

  root["name"] = "Power Sum";
  root["stat_t"] = MQTT_TOPIC_STATE;
  root["unit_of_meas"] = "W";
  root["val_tpl"] = "{{value_json.Psum}}";
  
  char message[256];
  serializeJson(root, message,sizeof(message));
  client.publish(MQTT_CONFIG_Psum, message, true);
  //Serial.println(message);
}

void MQTT_publish_config_Etoday() 
{
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const int capacity = JSON_OBJECT_SIZE(5); //  <-------- Change it as reqd
  StaticJsonDocument<capacity> root;  

  root["name"] = "Energy Today";
  root["stat_t"] = MQTT_TOPIC_STATE;
  root["unit_of_meas"] = "kWh";
  root["val_tpl"] = "{{value_json.Etoday}}";
  
  char message[256];
  serializeJson(root, message,sizeof(message));
  client.publish(MQTT_CONFIG_Etoday, message, true);
  //Serial.println(message);
}

void MQTT_publish_config_Emonth() 
{
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const int capacity = JSON_OBJECT_SIZE(5); //  <-------- Change it as reqd
  StaticJsonDocument<capacity> root;  

  root["name"] = "Energy Month";
  root["stat_t"] = MQTT_TOPIC_STATE;
  root["unit_of_meas"] = "kWh";
  root["val_tpl"] = "{{value_json.Emonth}}";
  
  char message[256];
  serializeJson(root, message,sizeof(message));
  client.publish(MQTT_CONFIG_Emonth, message, true);
  //Serial.println(message);
}

void MQTT_publish_config_Etotal() 
{
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const int capacity = JSON_OBJECT_SIZE(5); //  <-------- Change it as reqd
  StaticJsonDocument<capacity> root;  

  root["name"] = "Energy Total";
  root["stat_t"] = MQTT_TOPIC_STATE;
  root["unit_of_meas"] = "kWh";
  root["val_tpl"] = "{{value_json.Etotal}}";
  
  char message[256];
  serializeJson(root, message,sizeof(message));
  client.publish(MQTT_CONFIG_Etotal, message, true);
  //Serial.println(message);
}

void MQTT_publish_config_PF() 
{
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const int capacity = JSON_OBJECT_SIZE(5); //  <-------- Change it as reqd
  StaticJsonDocument<capacity> root;  

  root["name"] = "Power Factor avg";
  root["stat_t"] = MQTT_TOPIC_STATE;
  root["unit_of_meas"] = "-";
  root["val_tpl"] = "{{value_json.PFavg}}";
  
  char message[256];
  serializeJson(root, message,sizeof(message));
  client.publish(MQTT_CONFIG_PF, message, true);
  //Serial.println(message);
}

void MQTT_publish_config_FREQ() 
{
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const int capacity = JSON_OBJECT_SIZE(5); //  <-------- Change it as reqd
  StaticJsonDocument<capacity> root;  

  root["name"] = "Frequency avg";
  root["stat_t"] = MQTT_TOPIC_STATE;
  root["unit_of_meas"] = "hz";
  root["val_tpl"] = "{{value_json.Freq}}";
  
  char message[256];
  serializeJson(root, message,sizeof(message));
  client.publish(MQTT_CONFIG_FREQ, message, true);
  //Serial.println(message);
}



bool MQTT_hasValueChanged(float newValue, float prevValue, float maxDiff) 
{
  return !isnan(newValue) && (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}




//-------------------- PIR MQTT -----------------------------

// Motion Sensor
#define MQTT_CONFIG_PIR        "homeassistant/sensor/PIR/Lobby/config"
#define MQTT_TOPIC_STATE_PIR   "homeassistant/sensor/PIR/Lobby/state"

unsigned long MQTT_PIR_heartbeat_timestamp;
unsigned long MQTT_PIR_last_ON_msg_timestamp;
bool MQTT_PIR_last_ON_msg_state;



void MQTT_publish_PIR(bool PIR_State)
{   
    if(PIR_State)
      client.publish(MQTT_TOPIC_STATE_PIR, "ON", true);
    else
      client.publish(MQTT_TOPIC_STATE_PIR, "OFF", true);
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
  if(millis()/1000 - MQTT_PIR_heartbeat_timestamp > 60 || MQTT_PIR_heartbeat_timestamp==0)  // send every 60 sec
  {
    MQTT_PIR_heartbeat_timestamp = millis()/1000;
    MQTT_publish_PIR(false);
  }
}




//-------------------- Device Analytics -----------------------------

#define MQTT_TOPIC_STATE_DEEP_DIVE   "homeassistant/Analytics/Power/state"

void MQTT_publish_Deep_Dive()
{   
      // Use arduinojson.org/v6/assistant to compute the capacity.
      const size_t capacity = JSON_OBJECT_SIZE(10);
      DynamicJsonDocument doc(capacity);
      
      doc["P1"]     = String(p1);
      doc["P2"]     = String(p2);
      doc["P3"]     = String(p3);
      doc["PF1"]    = String(pf1);
      doc["PF2"]    = String(pf2);
      doc["PF3"]    = String(pf3);
      
      char data[256];
      serializeJson(doc, data, sizeof(data));
      client.publish(MQTT_TOPIC_STATE_DEEP_DIVE, data, true);
      Serial.println(data);
}