#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid        = "Realme 8pro";
const char* password    = "iamsiddy";
const char* mqtt_server = "3.214.158.175";
int         mqtt_port   = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

int prvState = LOW;             // we start, assuming no motion detected
int val = 0; 
int deviceid =0;

String pubString;
char pubChars[100];

int ldr = A0;  // Analog pin A0
long ldrValue = 0;
int pir = 13;  // Digital pin D7


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500);}
  digitalWrite(2, LOW);
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
   } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  //code for pir
  pinMode(pir, INPUT);   // declare pir sensor as input
  //device id
  deviceid=ESP.getChipId();
  pinMode(2, OUTPUT); // builtin led for wifi connection status
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  //client.loop();
  readSensor();
}
void sendData(){
                        //deviceid to mqtt
                        
                  pubString = "{\"deviceid\":" + String(deviceid);
                  Serial.print("deviceid = ");
                  Serial.print(deviceid);
                  Serial.print("; ");
            
                      //pir data to mqtt
                            
                  //long state_pir = digitalRead(pir);
                  Serial.print("pir_Sensor_value = ");
                  Serial.print(prvState);
                  Serial.print("; ");
                  String value_pir = String(prvState);
                  pubString += ",\"pir\":" + value_pir;       
                          
                           //ldr data to mqtt
                    
                  int ldrValue = analogRead(ldr);
                  ldrValue = map(ldrValue, 0, 1024, 1204, 0  );
                  Serial.print("ldr_Sensor_value = ");
                  Serial.print(ldrValue);
                  Serial.println("; ");
                  String value_ldr = String(ldrValue);
                  pubString += ",\"ldr\":" + value_ldr+ "}";        
                  
                            // publishing data
                  
                  pubString.toCharArray(pubChars, pubString.length() + 1);
                  client.publish("BEAMSENSORDATA", pubChars);
  }
void readSensor(){

                  val = digitalRead(pir);      
                  if (val == HIGH) 
                  {     if (prvState == LOW) { prvState = HIGH; sendData();}
                        else { delay(10000);}
                  } 
                  else
                  {
                      if (prvState == HIGH) { prvState = LOW; sendData();}
                      else {;}
                  }
}
