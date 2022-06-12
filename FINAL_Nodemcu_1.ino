#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "EmonLib.h"                   // Include Emon Library
#include "DHTesp.h"

EnergyMonitor emon1;

#define Relay1 R1;
#define Relay2 R2;
#define Relay3 R3;

// Update these with values suitable for your network.
const char* ssid        = "Realme 8pro";
const char* password    = "iamsiddy";
const char* mqtt_server = "3.214.158.175";
int         mqtt_port   = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

int deviceid;
DHTesp dht;
String pubString;
char pubChars[100];

const int R1 = D4; //pin number 2
const int R2 = D6; //pin number 12
const int R3 = D7; //pin number 13

void setupWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500); 
    Serial.print(".");
  }
  digitalWrite(16, LOW); 
}

void setupMqtt() {
  mqttClient.setServer(mqtt_server, mqtt_port);
  // invoke "callback()" function when there is an incoming message
  mqttClient.setCallback(callback);   
}

void reconnect() {
   while (!mqttClient.connected())
   {
      String clientId = String(ESP.getChipId());
      if (mqttClient.connect(clientId.c_str())) {Serial.println("connected");}
      // subscribe to an MQTT topic     
      mqttClient.subscribe("Relays");
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(16, OUTPUT);
  digitalWrite(R1, HIGH); // by default led will be low
  digitalWrite(R2, HIGH); // by default led will be low
  digitalWrite(R3, HIGH); // by default led will be low

  dht.setup(4, DHTesp::DHT11); // GPIO4(D2)
  
  setupWifi();       // setup WiFi connection
  setupMqtt();       // setup mqtt connection

  emon1.voltage(A0, 234.26, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(A0, 111.1);       // Current: input pin, calibration.
  delay(1000);
}

void loop() {
  if (!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop();
  delay(10);
}
void sendData(int deviceid)
{
          pubString = "{\"deviceid\":" + String(deviceid);                          
                         
                    //dht11 data to mqtt

          float humidity = dht.getHumidity();
          float temperature = dht.getTemperature();
          Serial.print("Humidity(%) =  ");
          Serial.print(humidity, 1);
          Serial.print("; ");
          Serial.print("Temperature(C) = ");
          Serial.print(temperature, 1);
          Serial.println();
          String value_dhth = String(humidity);
          pubString += ",\"humidity(%)\":" + value_dhth;
          String value_dhtt = String(temperature);
          pubString += ",\"temp(C)\":" + value_dhtt;

                          //sct data to mqtt                     

          emon1.calcVI(20,20);                 // Calculate all. No.of half wavelengths (crossings), time-out
          float Irms = emon1.calcIrms(1480);   //emon1.Irms;
          float current = Irms*0.150;          //current
          float power = current*230;           // power
          Serial.print("Current(A)= ");
          Serial.print(current, 1);
          Serial.println("; ");
          Serial.print("Power(W) = ");
          Serial.print(power, 1);
          Serial.println();
          String value_current = String(current);
          pubString += ",\"Current(I)\":" + value_current;
          String value_power = String(power);
          pubString += ",\"Power(W)\":" + value_power + "}";
                            
                            // publishing data
                  
          pubString.toCharArray(pubChars, pubString.length() + 1);
          mqttClient.publish("DHT&SCT", pubChars);
  }
void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  payload[length] = '\0'; // terminate the incoming list of charas as a String

  // act on the incoming command
  if(strcmp(((char *)payload), "GETDATASEC1") == 0) 
  {
    deviceid = 14374600;
    sendData(deviceid);
    delay(10);
  }
  else if(strcmp(((char *)payload), "GETDATASEC2") == 0) 
  {
    deviceid = 14403329;
    sendData(deviceid);
    delay(10);
  }
  else if(strcmp(((char *)payload), "GETDATASEC3") == 0) 
  {
    deviceid = 1797030;
    sendData(deviceid);
    delay(10);
  }
  else if(strcmp(((char *)payload), "GETDATASEC4") == 0) 
  {
    deviceid = 14376745;
    sendData(deviceid);
    delay(10);
  }
  else if(strcmp(((char*)payload),"L1OFF") == 0)  {digitalWrite(R1,HIGH);}       // L1 OFF 
  else if(strcmp(((char *)payload),"L1ON") == 0)  {digitalWrite(R1,LOW);}        // L1 ON
  else if(strcmp(((char*)payload),"L8OFF") == 0)  {digitalWrite(R2,HIGH);}       // L8 OFF
  else if(strcmp(((char *)payload),"L8ON") == 0)  {digitalWrite(R2,LOW);}        // L8 ON
  else if(strcmp(((char*)payload),"F1OFF") == 0)  {digitalWrite(R3,HIGH);}       // F1 OFF
  else if(strcmp(((char *)payload),"F1ON") == 0)  {digitalWrite(R3,LOW);}        // F1 ON 
  else if(strcmp(((char*)payload),"ALLOFF") == 0)  {digitalWrite(R1,HIGH);digitalWrite(R2,HIGH);digitalWrite(R3,HIGH);} // L1 OFF, L8 OFF, F1 OFF
  else{Serial.println("Unexpected message");}
}
