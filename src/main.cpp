#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include <creds.h> // Wifi Creds
#include <ArduinoJson.h>

// connect to resources
const char *ssid = STASSID;
const char *password = STAPSK;
const char* mqttServer = MQTT_SERVER;    //Enter Your mqttServer address
const char* mqttUser = MQTT_USER; //User
const char* mqttPassword = MQTT_PASSWD; //Password


// NeoPixel LEDS Configuration
#define NUM_LEDS 50
// #define NUM_LEDS 200
#define LED_DATA_PIN D5

void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


WiFiClient espClient;
// PubSubClient client(espClient);
PubSubClient client(mqttServer, 1883, MQTTcallback, espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;



void setup() {
  // put your setup code here, to run once:
  // Setup 
  delay(10);
	Serial.begin(115200);
  // set mode to wifi station (reconnects)
  WiFi.mode(WIFI_STA);
	// Connect to wifi
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		// Wait for connection
    delay(500);
    // Print connection status message
		Serial.println("Connecting to WiFi..");
	}
  Serial.print("Connected to WiFi SSID/IP Address:");
  Serial.print(WiFi.SSID());
  Serial.print("/");
  Serial.println(WiFi.localIP());
	// Set DNS Name
	if (MDNS.begin("D154A3")) {
    Serial.println("MDNS responder started");
  }
  // Connect to MQTT
  if (client.connect("D154A3", mqttUser, mqttPassword)) {
    Serial.println("Connected to MQTT Broker");
    client.publish("home","hello world");
    client.subscribe("home/lights/D154A3");
  };
  // client.setCallback(MQTTcallback);
	// Setup FastLEDs
	// LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
	// Set initial brightness
	// LEDS.setBrightness(250);
}

void loop() {
  // put your main code here, to run repeatedly:
  client.loop();
}