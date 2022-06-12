#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define Relay10 R10
#define Relay11 R11
#define Relay12 R12
#define Relay13 R13
#define Relay14 R14
#define Relay15 R15
#define Relay16 R16

// Update these with values suitable for your network.
const char* ssid        = "Realme 8pro";
const char* password    = "iamsiddy";
const char* mqtt_server = "3.214.158.175";
int         mqtt_port   = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const int R10 =  D1; 
const int R11 =  D2;
const int R12 =  D3;
const int R13 =  D4; 
const int R14 =  D5;
const int R15 =  D6;
const int R16 =  D7;

void setupWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {delay(500); Serial.print(".");}
  digitalWrite(16, LOW);
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
void setup() {
  pinMode(R10, OUTPUT);
  pinMode(R11, OUTPUT);
  pinMode(R12, OUTPUT);
  pinMode(R13, OUTPUT);
  pinMode(R14, OUTPUT);
  pinMode(R15, OUTPUT);
  pinMode(R16, OUTPUT);
  pinMode(16, OUTPUT); // builtin led for wifi connection status
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
  if(strcmp(((char *)payload),"F3ON") == 0){digitalWrite(R10,LOW);}               // F3 ON
  else if(strcmp(((char*)payload),"F3OFF") == 0){digitalWrite(R10,HIGH);}         // F3 OFF
  else if(strcmp(((char *)payload),"F6ON") == 0){digitalWrite(R11,LOW);}          // F6 ON 
  else if(strcmp(((char*)payload),"F6OFF") == 0){digitalWrite(R11,HIGH);}         // F6 OFF
  else if(strcmp(((char *)payload),"L4ON") == 0){digitalWrite(R12,LOW);}          // L4 ON
  else if(strcmp(((char*)payload),"L4OFF") == 0){digitalWrite(R12,HIGH);}         // L4 OFF
  else if(strcmp(((char *)payload),"L5ON") == 0){digitalWrite(R13,LOW);}          // L5 ON
  else if(strcmp(((char*)payload),"L5OFF") == 0){digitalWrite(R13,HIGH);}         // L5 OFF
  else if(strcmp(((char *)payload),"F4ON") == 0){digitalWrite(R14,LOW);}          // F4 ON
  else if(strcmp(((char*)payload),"F4OFF") == 0){digitalWrite(R14,HIGH);}         // F4 OFF
  else if(strcmp(((char *)payload),"F5ON") == 0){digitalWrite(R15,LOW);}          // F5 ON
  else if(strcmp(((char*)payload),"F5OFF") == 0){digitalWrite(R15,HIGH);}         // F5 OFF
  else if(strcmp(((char *)payload),"F8ON") == 0){digitalWrite(R16,LOW);}          // F8 ON     
  else if(strcmp(((char*)payload),"F8OFF") == 0){digitalWrite(R16,HIGH);}         // F8 OFF
  else if(strcmp(((char*)payload),"ALLOFF") == 0){digitalWrite(R10,HIGH);digitalWrite(R11,HIGH);digitalWrite(R12,HIGH);digitalWrite(R13,HIGH);digitalWrite(R14,HIGH);digitalWrite(R15,HIGH);digitalWrite(R16,HIGH);} // F3 OFF, F6 OFF, L4 OFF, L5 OFF, F4 OFF, F5 OFF, F8 OFF  
  else{Serial.println("Unexpected message");}
}
