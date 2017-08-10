

/*
wifi_station_set_hostname("xxxxx");
wifi_station_get_hostname();
*/
#include "Configuration.h"
#include "DLRObject.h"
#include "DLRMessages.h"
#include "DLRObjectManager.h"
#include "DLRMessagesManager.h"
#include "DLRWifiManager.h"




WiFiClient espClient;
//WiFiClientSecure espClient;
PubSubClient client( espClient );




long lastMsg = 0;
char msg[50];
int value = 0;


time_t   system_boot_time         = 0;
uint64_t MicroTimeStampTimeOffset = 0;
uint64_t LastMicroTimeStamp       = micros();

void setup()
{
	Serial.begin( 115200 );
	Serial.println("\n\n/*********************************************/");
	Serial.println();
	Serial.println("/*********************************************/");

	ObjectsQueue.push_back( new DLRMessagesQueueManager() );
	ObjectsQueue.push_back( new DLRObjectManager() );
	ObjectsQueue.push_back( new DLRWifiManager() );
	
	//espClient.setCACert(ca_crt, ca_crt_len);
    //espClient.setCertificate(dlr_000_crt, dlr_000_crt_len);
    //espClient.setPrivateKey(dlr_000_key, dlr_000_key_len);
    //client.setServer( mqtt_server , 1883 ); // 9001 ); //1883 ); // 8883);
    //client.setCallback(callback);

	ObjectManager->main_setup();
	addLog( 0 , LOG_DEBUG , "Exit setup() -> ObjectQueue.size(): %ld\n" , ObjectsQueue.size() );
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
	/*
    while(!client.connected())
	{
		addLog( 0 , LOG_DEBUG , "Attempting MQTT connection...\n");
        if( client.connect("dlr_000", "dlr", "dragon"))
		{
			addLog( 0 , LOG_DEBUG , "MQTT connected to: %s\n" , mqtt_server );
            // Resubscribe to all your topics here so that they are
            // resubscribed each time you reconnect
			client.publish("outTopic", "hello world");
			client.subscribe("inTopic");
        } else {
			addLog( 0 , LOG_DEBUG , "failed, rc=%d retry again in 5 seconds\n" , client.state() );
			// Wait 5 seconds before retrying
			delay(5000);
        }
    }
	*/
}


void loop()
{
	ObjectManager->main_loop();


	/*
	if(!client.connected())
	{
		reconnect();
	}
	yield();
	client.loop();
	*/
	/*
	long now = millis();
	if ( now - lastMsg > 2000 )
	{
		lastMsg = now;
		++value;
		snprintf( msg, 75, "hello world #%ld", value);
		client.publish("outTopic", msg );
		//addLog( 0 , LOG_DEBUG , "Publish message: %s\n", msg );
	}
	*/
}

