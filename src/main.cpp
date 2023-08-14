#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int relay = 0;

const char* mqtt_server = "<MQTT BROKER IP>"; 
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");

  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }
   Serial.print(message);
  if(message == "0") {digitalWrite(relay,LOW);}
  if(message == "1") {digitalWrite(relay,HIGH);}

  Serial.println();
}

void setup() {
  pinMode(relay,OUTPUT);

  Serial.begin(115200);
  Serial.println("Readings will apear here.");

  WiFi.begin("<SSID>","<PASSWORD>"); // Add your Wi-Fi name and password here
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  mqttClient.setServer(mqtt_server,1883);
  mqttClient.setCallback(callback);

  if(mqttClient.connect("relay001")){ // Device id whe connecting to the broker
    Serial.println("relay001 - MQTT - OK");
    mqttClient.subscribe("relay001/onoff"); // Topic
  }else{
    Serial.println("relay001 - MQTT - ERROR");
  }
}

void loop() {
  mqttClient.loop();
}