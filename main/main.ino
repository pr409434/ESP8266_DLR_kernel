
#include "local/id_local.h"
/***********************************************
    Content: id_local.h
************************************************
const char* ssid        = "................";
const char* password    = "................";
const char* mqtt_server = "................";
***********************************************/
/*
wifi_station_set_hostname("xxxxx");
wifi_station_get_hostname();
*/
#include "Configuration.h"

void WiFiEvent( WiFiEvent_t event )
{
	switch( event )
	{
		case WIFI_EVENT_STAMODE_GOT_IP:
			addLog( 0 , LOG_DEBUG , "WiFi connected. IP address:%s hostname:%s SSID: %s\n"
						, WiFi.localIP().toString().c_str()
						, WiFi.hostname().c_str()
						, WiFi.SSID().c_str()
					);
			break;
		case WIFI_EVENT_STAMODE_DISCONNECTED:
			addLog( 0 , LOG_DEBUG , "WiFi client lost connection\n");
			break;
		case WIFI_EVENT_STAMODE_CONNECTED:
			addLog( 0 , LOG_DEBUG , "WiFi client connected\n");
			break;
		case WIFI_EVENT_STAMODE_AUTHMODE_CHANGE:
			addLog( 0 , LOG_DEBUG , "WiFi client authentication mode changed.\n");
			break;
		/*
		// THIS IS A NEW CONSTANT ENABLE WITH UPDATED SDK
		case WIFI_STAMODE_DHCP_TIMEOUT:
			addLog( 0 , LOG_DEBUG , "WiFi client DHCP timeout reached.");
			break;
		*/
		case WIFI_EVENT_SOFTAPMODE_STACONNECTED:
			addLog( 0 , LOG_DEBUG , "WiFi accesspoint: new client connected. Clients: %d\n" , WiFi.softAPgetStationNum() );
			break;
		case WIFI_EVENT_SOFTAPMODE_STADISCONNECTED:
			addLog( 0 , LOG_DEBUG , "WiFi accesspoint: client disconnected. Clients: %d\n" , WiFi.softAPgetStationNum() );
			break;
		case WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED:
			//addLog( 0 , LOG_DEBUG , "WiFi accesspoint: probe request received.\n" );
			break; 
		default:
			addLog( 0 , LOG_DEBUG , "WiFiEvent: unlnow event received.\n" );
			break;
	}
}


WiFiClient espClient;
//WiFiClientSecure espClient;
PubSubClient client(espClient);




long lastMsg = 0;
char msg[50];
int value = 0;

/*
DLRObject demo;
DLRObject demo_01;
DLRObject demo_02;
DLRObject demo_03;	
*/
void setup()
{
	Serial.begin( 115200 );
	Serial.println("\n\n/*********************************************/");
	
	ObjectManager.main_setup();
	addLog( 0 , LOG_DEBUG , "Start setup() -> ObjectManager.size(): %ld\n" , ObjectManager.size() );
	
	WiFi.onEvent( WiFiEvent );

	
	// Connect to WiFi network
	WiFi.begin(ssid, password);
	Serial.print("Connect to WiFi network:");
    // Wait for connection
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
    }
	Serial.println();
	
	addLog( 0 , LOG_DEBUG , "Connected to %s %s\n" , ssid , WiFi.localIP().toString().c_str() );
	
	configTime( 2 * 3600 , 0 , // , 1 * 3600,
				"0.pool.ntp.org",
				"1.pool.ntp.org",
				"2.pool.ntp.org");
	uint16_t timeout = 0;
	time_t timestamp = time( NULL );
	while (
			( timestamp == 0 ) &&
			( timeout <  30  )
		  )
	{
		yield();
		delay( 5000 );
		timestamp = time( NULL );
		timeout += 5;
	}
	if( timestamp > (2017 - 1970)*(365*24*3600) ) 
	{
		system_boot_time = timestamp - ( millis()/1000);
		addLog( 0 , LOG_DEBUG , "NTP on\n");
	}
	else
	{
		addLog( 0 , LOG_DEBUG , "NTP off\n");
	}
	
	addLog( 0 , LOG_DEBUG , "ESP8266 SDK Version: %s\n" , ESP.getSdkVersion() );
	
	//espClient.setCACert(ca_crt, ca_crt_len);
    //espClient.setCertificate(dlr_000_crt, dlr_000_crt_len);
    //espClient.setPrivateKey(dlr_000_key, dlr_000_key_len);
	
    client.setServer( mqtt_server , 1883 ); // 9001 ); //1883 ); // 8883);
    client.setCallback(callback);
	
	addLog( 0 , LOG_DEBUG , "Exit setup() -> ObjectManager.size(): %ld\n" , ObjectManager.size() );
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
		snprintf( msg, 75, "hello world #%ld", value);
		client.publish("outTopic", msg );
		//addLog( 0 , LOG_DEBUG , "Publish message: %s\n", msg );
		/*
		Serial.print("s:");
		Serial.print( Serial.availableForWrite() );
		Serial.println();
		Serial.print("Publish message: ");
		Serial.println(msg);
		Serial.print("s:");
		Serial.print( Serial.availableForWrite() );
		Serial.println();
		
		*/
		/*
		Serial.print("MessagesQueue.size: ");
		Serial.print( MessagesQueue.size() );
		Serial.println();
		*/
		//Serial.println( (String_Format( "B_MessagesQueue.size: %d" , MessagesQueue.size() )).c_str() );
		//addLog( 0 , LOG_DEBUG , "#%ld\tMessagesQueue.size: %ld\n" , value , MessagesQueue.size() );
	}
}

