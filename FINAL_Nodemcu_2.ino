 #include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid        = "Realme 8pro";
const char* password    = "iamsiddy";
const char* mqtt_server = "3.214.158.175";
int         mqtt_port   = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const int Relay4 =  D2; 
const int Relay5 =  D3;
const int Relay6 =  D4;
const int Relay7 =  D5; 
const int Relay8 =  D6;
const int Relay9 =  D7;

void setup() {
  pinMode(Relay4, OUTPUT);
  pinMode(Relay5, OUTPUT);
  pinMode(Relay6, OUTPUT);
  pinMode(Relay7, OUTPUT);
  pinMode(Relay8, OUTPUT);
  pinMode(Relay9, OUTPUT);
  digitalWrite(Relay4,HIGH);
  digitalWrite(Relay5,HIGH);
  digitalWrite(Relay6,HIGH);
  digitalWrite(Relay7,HIGH);
  digitalWrite(Relay8,HIGH);
  digitalWrite(Relay9,HIGH);
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
    if(strcmp(((char *)payload), "REL4ON") == 0){digitalWrite(Relay4,LOW);}             // L2 ON
    else if(strcmp(((char*)payload),"REL4OFF") == 0){digitalWrite(Relay4,HIGH);}        // L2 OFF
    else if(strcmp(((char *)payload),"REL5ON") == 0){digitalWrite(Relay5,LOW);}         // L7 ON
    else if(strcmp(((char*)payload),"REL5OFF") == 0){digitalWrite(Relay5,HIGH);}        // L7 OFF
    else if(strcmp(((char *)payload),"REL6ON") == 0){digitalWrite(Relay6,LOW);}         // F2 ON
    else if(strcmp(((char*)payload),"REL6OFF") == 0){digitalWrite(Relay6,HIGH);}        // F2 OFF
    else if(strcmp(((char *)payload),"REL7ON") == 0){digitalWrite(Relay7,LOW);}         // F7 ON     
    else if(strcmp(((char*)payload),"REL7OFF") == 0){digitalWrite(Relay7,HIGH);}        // F7 OFF
    else if(strcmp(((char *)payload),"REL8ON") == 0){digitalWrite(Relay8,LOW);}         // L3 ON
    else if(strcmp(((char*)payload),"REL8OFF") == 0){digitalWrite(Relay8,HIGH);}        // L3 OFF
    else if(strcmp(((char *)payload),"REL9ON") == 0){digitalWrite(Relay9,LOW);}         // L6 ON
    else if(strcmp(((char*)payload),"REL9OFF") == 0){digitalWrite(Relay9,HIGH);}        // L6 OFF
  else{Serial.println("Unexpected message");}
}
