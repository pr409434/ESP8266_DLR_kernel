
#include "..\id_local.h"
/***********************************************
    Content: id_local.h
************************************************
const char* ssid        = "................";
const char* password    = "................";
const char* mqtt_server = "................";
***********************************************/
#include "certificates.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>

#include <stdint.h>
#include <list>
#include <functional>

#include "DLRObject.hpp"
#include "MessagesQueue.hpp"
#include "DLRObjectManager.hpp"


void WiFiEvent(WiFiEvent_t event) {
    switch(event) {
        case WIFI_EVENT_STAMODE_GOT_IP:
            Serial.println("WiFi connected. IP address: " + String(WiFi.localIP().toString()) + " hostname: "+ WiFi.hostname() + "  SSID: " + WiFi.SSID());
            break;
        case WIFI_EVENT_STAMODE_DISCONNECTED:
            Serial.println("WiFi client lost connection");
            break;
        case WIFI_EVENT_STAMODE_CONNECTED:
            Serial.println("WiFi client connected");
            break;
        case WIFI_EVENT_STAMODE_AUTHMODE_CHANGE:
            Serial.println("WiFi client authentication mode changed.");
            break;
        //case WIFI_STAMODE_DHCP_TIMEOUT:                             THIS IS A NEW CONSTANT ENABLE WITH UPDATED SDK
          //  Serial.println("WiFi client DHCP timeout reached.");
            //break;
        case WIFI_EVENT_SOFTAPMODE_STACONNECTED:
            Serial.println("WiFi accesspoint: new client connected. Clients: "  + String(WiFi.softAPgetStationNum()));
             break;
        case WIFI_EVENT_SOFTAPMODE_STADISCONNECTED:
            Serial.println("WiFi accesspoint: client disconnected. Clients: " + String(WiFi.softAPgetStationNum()));
             break;
        case WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED:
            //Serial.println("WiFi accesspoint: probe request received.");
            break; 
      }
}


WiFiClient espClient;
//WiFiClientSecure espClient;
PubSubClient client(espClient);




long lastMsg = 0;
char msg[50];
int value = 0;

DLRObject demo;
/*
DLRObject demo_01;
DLRObject demo_02;
DLRObject demo_03;	
*/
time_t system_boot_time = 0;
void setup()
{
	Serial.begin(115200);
	
	ObjectManager.main_setup();
	
	
	WiFi.onEvent(WiFiEvent);
	
	// Connect to WiFi network
	WiFi.begin(ssid, password);
	Serial.println("");

    // Wait for connection
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
    }
	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
	
	configTime( 2 * 3600 , 0 , // , 1 * 3600,
				"0.pool.ntp.org",
				"1.pool.ntp.org",
				"2.pool.ntp.org");

	Serial.print("NTP status:");
	uint16_t timeout = 0;
	time_t timestamp = time( NULL );
	while (
			(timestamp==0) &&
			( timeout <  30 )
		  )
	{
		yield();
		Serial.print(".");
		delay( 5000 );
		timestamp = time( NULL );
		timeout += 5;
	}
	if( timestamp > (2017 - 1970)*(365*24*3600) ) 
	{
		system_boot_time = timestamp - ( millis()/1000);
		Serial.println("\nNTP ok");
	}
	

	Serial.println("\nESP8266 SDK Version: " +String(ESP.getSdkVersion()));

	
	//espClient.setCACert(ca_crt, ca_crt_len);
    //espClient.setCertificate(dlr_000_crt, dlr_000_crt_len);
    //espClient.setPrivateKey(dlr_000_key, dlr_000_key_len);
	
    client.setServer( mqtt_server , 1883); // 9001 ); //1883 ); // 8883);
    client.setCallback(callback);
	
	Serial.print("ObjectManager.size(): ");
	Serial.print( ObjectManager.size() );
	Serial.println("");
}

void callback(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
	}
	Serial.println();

	/*
	// Switch on the LED if an 1 was received as first character
	if ((char)payload[0] == '1') {
		digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
		// but actually the LED is on; this is because
		// it is acive low on the ESP-01)
	} else {
		digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
	}
	*/
}


void reconnect() {
    while(!client.connected())
	{
		Serial.print("Attempting MQTT connection...");
        if( client.connect("dlr_000", "dlr", "dragon"))
		{
			Serial.println("connected");
            // Resubscribe to all your topics here so that they are
            // resubscribed each time you reconnect
			client.publish("outTopic", "hello world");
			client.subscribe("inTopic");
        } else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
        }
    }
}


void loop()
{
	
	if(!client.connected())
	{
		reconnect();
	}
	yield();
	client.loop();
	ObjectManager.main_loop();
	long now = millis();
	if ( now - lastMsg > 2000 )
	{
		lastMsg = now;
		++value;
		snprintf (msg, 75, "hello world #%ld", value);
		Serial.print("Publish message: ");
		Serial.println(msg);
		client.publish("outTopic", msg);
	}
}




/*******************************************************************************************
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "........";
const char* password = "........";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

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
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}
*************************************************/