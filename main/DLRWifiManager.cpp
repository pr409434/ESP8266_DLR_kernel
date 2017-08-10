#include "DLRWifiManager.h"



#include "local/id_local.h"
/***********************************************
    Content: id_local.h
************************************************
const char* ssid        = "................";
const char* password    = "................";
const char* mqtt_server = "................";
***********************************************/


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

DLRWifiManager::DLRWifiManager()
{
	if( name == nullptr )
	{
		name = (char *) "WifiManager";
	}
	priority = 0;
}	
DLRWifiManager::~DLRWifiManager()
{
	
}

error_t DLRWifiManager::setup()
{
	WiFi.onEvent( WiFiEvent );
	// sNTP config
	configTime( 2 * 3600 , 0 , // , 1 * 3600,
				"192.168.1.47",
				"192.168.1.27",
				"pool.ntp.org");
	// Connect to WiFi network
	WiFi.begin( ssid , password );

    // Wait for connection
	_timer_reconnect.countdown( 30 ); // 30 seconds
}

error_t DLRWifiManager::loop()
{
	if( WiFi.status() != WL_CONNECTED )
	{
		if( _timer_reconnect.expired() )
		{
			WiFi.reconnect();
			_timer_reconnect.countdown( 30 );
		}
	} else
	{
		if ( system_boot_time == 0 )
		{
			time_t _actual_timestamp = time( NULL );
			if( _actual_timestamp > (2017 - 1970)*(365*24*3600) ) 
			{
				system_boot_time = _actual_timestamp - ( millis()/1000);
				addLog( 0 , LOG_DEBUG , "NTP on\tcurrent time: %s\n" , ctime(&_actual_timestamp) );
				addLog( 0 , LOG_DEBUG , "system_boot_time: %s\n" , ctime(&system_boot_time) );
			}
		}
	}
}

error_t DLRWifiManager::reconnect_STA()
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