#include "DLRWifiManager.h"

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



error_t DLRWifiManager::setup()
{
	WiFi.onEvent( WiFiEvent );

	// Connect to WiFi network
	WiFi.begin( ssid , password );

    // Wait for connection
	while ( WiFi.status() != WL_CONNECTED )
	{
		delay(500);
		Serial.print(".");
    }
}

error_t DLRWifiManager::loop()
{
	/*
	if(!client.connected())
	{
		reconnect();
	}
	*/
}

error_t DLRWifiManager:reconnect()
{
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