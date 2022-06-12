 #include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define Relay4 R4;
#define Relay5 R5;
#define Relay6 R6;
#define Relay7 R7;
#define Relay8 R8;
#define Relay9 R9;

// Update these with values suitable for your network.
const char* ssid        = "Realme 8pro";
const char* password    = "iamsiddy";
const char* mqtt_server = "3.214.158.175";
int         mqtt_port   = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const int R4 =  D2; 
const int R5 =  D3;
const int R6 =  D4;
const int R7 =  D5; 
const int R8 =  D6;
const int R9 =  D7;

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
      mqttClient.subscribe("Relays");  
  }
}
void setup() {
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R6, OUTPUT);
  pinMode(R7, OUTPUT);
  pinMode(R8, OUTPUT);
  pinMode(R9, OUTPUT);
  pinMode(16, OUTPUT); // builtin led for wifi connection status
  digitalWrite(R4,HIGH);
  digitalWrite(R5,HIGH);
  digitalWrite(R6,HIGH);
  digitalWrite(R7,HIGH);
  digitalWrite(R8,HIGH);
  digitalWrite(R9,HIGH);
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
    if(strcmp(((char *)payload),"L2ON") == 0){digitalWrite(R4,LOW);}              // L2 ON
    else if(strcmp(((char*)payload),"L2OFF") == 0){digitalWrite(R4,HIGH);}        // L2 OFF
    else if(strcmp(((char *)payload),"L7ON") == 0){digitalWrite(R5,LOW);}         // L7 ON
    else if(strcmp(((char*)payload),"L7OFF") == 0){digitalWrite(R5,HIGH);}        // L7 OFF
    else if(strcmp(((char *)payload),"F2ON") == 0){digitalWrite(R6,LOW);}         // F2 ON
    else if(strcmp(((char*)payload),"F2OFF") == 0){digitalWrite(R6,HIGH);}        // F2 OFF
    else if(strcmp(((char *)payload),"F7ON") == 0){digitalWrite(R7,LOW);}         // F7 ON     
    else if(strcmp(((char*)payload),"F7OFF") == 0){digitalWrite(R7,HIGH);}        // F7 OFF
    else if(strcmp(((char *)payload),"L3ON") == 0){digitalWrite(R8,LOW);}         // L3 ON
    else if(strcmp(((char*)payload),"L3OFF") == 0){digitalWrite(R8,HIGH);}        // L3 OFF
    else if(strcmp(((char *)payload),"L6ON") == 0){digitalWrite(R9,LOW);}         // L6 ON
    else if(strcmp(((char*)payload),"L6OFF") == 0){digitalWrite(R9,HIGH);}        // L6 OFF
    else if(strcmp(((char*)payload),"ALLOFF") == 0){digitalWrite(R4,HIGH);digitalWrite(R5,HIGH);digitalWrite(R6,HIGH);digitalWrite(R7,HIGH);digitalWrite(R8,HIGH);digitalWrite(R9,HIGH);}  // L2 OFF, L7 OFF, F2 OFF, F7 OFF, L3 OFF, L6 OFF
    else{Serial.println("Unexpected message");}
}
