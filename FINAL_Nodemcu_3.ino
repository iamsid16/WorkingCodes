#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid        = "Realme 8pro";
const char* password    = "iamsiddy";
const char* mqtt_server = "3.214.158.175";
int         mqtt_port   = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const int Relay10 =  D1; 
const int Relay11 =  D2;
const int Relay12 =  D3;
const int Relay13 =  D4; 
const int Relay14 =  D5;
const int Relay15 =  D6;
const int Relay16 =  D7;

void setup() {
  pinMode(Relay10, OUTPUT);
  pinMode(Relay11, OUTPUT);
  pinMode(Relay12, OUTPUT);
  pinMode(Relay13, OUTPUT);
  pinMode(Relay14, OUTPUT);
  pinMode(Relay15, OUTPUT);
  pinMode(Relay16, OUTPUT);
  digitalWrite(Relay10,HIGH);
  digitalWrite(Relay11,HIGH);
  digitalWrite(Relay12,HIGH);
  digitalWrite(Relay13,HIGH);
  digitalWrite(Relay14,HIGH);
  digitalWrite(Relay15,HIGH);
  digitalWrite(Relay16,HIGH);
  delay(1000);
  Serial.begin(115200);
  setupWifi();       // setup WiFi connection
  setupMqtt();       // setup mqtt connection
  delay(1000);
}

void setupWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {delay(500); Serial.print(".");}
}

void setupMqtt() {
  mqttClient.setServer(mqtt_server, mqtt_port);
  // invoke "callback()" function when there is an incoming message
  mqttClient.setCallback(callback);   
}

void reconnect() {
   while (!mqttClient.connected()) {
      String clientId = String(ESP.getChipId());
      if (mqttClient.connect(clientId.c_str())) {Serial.println("connected");}
      // subscribe to an MQTT topic     
      mqttClient.subscribe("Relays");
  }
}

void loop() {
  if (!mqttClient.connected()) {reconnect();}
  mqttClient.loop();
  delay(10);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {Serial.print((char)payload[i]);}
  Serial.println();

  payload[length] = '\0'; // terminate the incoming list of charas as a String

  // act on the incoming command
  if(strcmp(((char *)payload), "REL10ON") == 0){digitalWrite(Relay10,LOW);}               // F3 ON
  else if(strcmp(((char*)payload),"REL10OFF") == 0){digitalWrite(Relay10,HIGH);}          // F3 OFF
  else if(strcmp(((char *)payload), "REL11ON") == 0){digitalWrite(Relay11,LOW);}          // F6 ON 
  else if(strcmp(((char*)payload),"REL11OFF") == 0){digitalWrite(Relay11,HIGH);}          // F6 OFF
  else if(strcmp(((char *)payload), "REL12ON") == 0){digitalWrite(Relay12,LOW);}          // L4 ON
  else if(strcmp(((char*)payload),"REL12OFF") == 0){digitalWrite(Relay12,HIGH);}          // L4 OFF
  else if(strcmp(((char *)payload), "REL13ON") == 0){digitalWrite(Relay13,LOW);}          // L5 ON
  else if(strcmp(((char*)payload),"REL13OFF") == 0){digitalWrite(Relay13,HIGH);}          // L5 OFF
  else if(strcmp(((char *)payload), "REL14ON") == 0){digitalWrite(Relay14,LOW);}          // F4 ON
  else if(strcmp(((char*)payload),"REL14OFF") == 0){digitalWrite(Relay14,HIGH);}          // F4 OFF
  else if(strcmp(((char *)payload), "REL15ON") == 0){digitalWrite(Relay15,LOW);}          // F5 ON
  else if(strcmp(((char*)payload),"REL15OF") == 0){digitalWrite(Relay15,HIGH);}           // F5 OFF
  else if(strcmp(((char *)payload), "REL16ON") == 0){digitalWrite(Relay16,LOW);}          // F8 ON     
  else if(strcmp(((char*)payload),"REL16OFF") == 0){digitalWrite(Relay16,HIGH);}          // F8 OFF
  else{Serial.println("Unexpected message");}
}
