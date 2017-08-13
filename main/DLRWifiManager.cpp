#include "DLRWifiManager.h"

		/*
        WiFiEventHandler onStationModeConnected(std::function<void(const WiFiEventStationModeConnected&)>);
        WiFiEventHandler onStationModeDisconnected(std::function<void(const WiFiEventStationModeDisconnected&)>);
        WiFiEventHandler onStationModeAuthModeChanged(std::function<void(const WiFiEventStationModeAuthModeChanged&)>);
        WiFiEventHandler onStationModeGotIP(std::function<void(const WiFiEventStationModeGotIP&)>);
        WiFiEventHandler onStationModeDHCPTimeout(std::function<void(void)>);
        WiFiEventHandler onSoftAPModeStationConnected(std::function<void(const WiFiEventSoftAPModeStationConnected&)>);
        WiFiEventHandler onSoftAPModeStationDisconnected(std::function<void(const WiFiEventSoftAPModeStationDisconnected&)>);
        WiFiEventHandler onSoftAPModeProbeRequestReceived(std::function<void(const WiFiEventSoftAPModeProbeRequestReceived&)>);
		// WiFiEventHandler onWiFiModeChange(std::function<void(const WiFiEventModeChange&)>);
		
		setOutputPower(float dBm);
		bool mode(WiFiMode_t); WIFI_PHY_MODE_11B = 1, WIFI_PHY_MODE_11G = 2, WIFI_PHY_MODE_11N = 3
		*/

void ICACHE_FLASH_ATTR WiFiEvent( WiFiEvent_t event )
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

ICACHE_FLASH_ATTR DLRWifiManager::DLRWifiManager()
{
	if( name == nullptr )
	{
		name = (char *) "WifiManager";
	}
	priority = 0;
}	
ICACHE_FLASH_ATTR DLRWifiManager::~DLRWifiManager()
{
	
}

error_t ICACHE_FLASH_ATTR DLRWifiManager::setup()
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
	_timer_reconnect.countdown( 5 ); // 30 seconds
	_timer_module_info.countdown( 10 );
	return( 0 );
}

error_t ICACHE_FLASH_ATTR DLRWifiManager::loop()
{
	if( WiFi.status() != WL_CONNECTED )
	{
		if( _timer_reconnect.expired() )
		{
			WiFi.reconnect();
			_timer_reconnect.countdown( 5 );
		}
	} else
	{
		if( _timer_module_info.expired() )
		{
			module_info();
		}
		if ( system_boot_time == 0 )
		{
			time_t _actual_timestamp = time( NULL );
			if( _actual_timestamp > (2017 - 1970)*(365*24*3600) ) 
			{
				system_boot_time = _actual_timestamp - ( millis()/1000);
				addLog( 0 , LOG_DEBUG , "NTP on\tcurrent time: %s" , ctime(&_actual_timestamp) );
				addLog( 0 , LOG_DEBUG , "system_boot_time: %s" , ctime(&system_boot_time) );
			}
		}
	}
	return( 0 );
}


error_t ICACHE_FLASH_ATTR DLRWifiManager::module_info()
{
	time_t timestamp = time( NULL );
	addLog( ObjectID , LOG_NOTICE ,
		"{\"timestamp\":%ld,\"SSID:\":\"%s\",\"BSSID\":\"%s\",\"RSSI\":%d,\"channel\":%d,\"WiFiPhyMode\":%d}\n"
					, timestamp
					, WiFi.SSID().c_str()
					, WiFi.BSSIDstr().c_str()
					, WiFi.RSSI()
					, WiFi.channel()
					, WiFi.getPhyMode()
				);
	if ( MQTT_MAX_PACKET_SIZE > 128 )
	{
		mqtt_publish( LOG_NOTICE , "Wifi" ,
			"{\"timestamp\":%ld,\"SSID:\":\"%s\",\"BSSID\":\"%s\",\"RSSI\":%d,\"channel\":%d,\"WiFiPhyMode\":%d}"
						, timestamp
						, WiFi.SSID().c_str()
						, WiFi.BSSIDstr().c_str()
						, WiFi.RSSI()
						, WiFi.channel()
						, WiFi.getPhyMode()
					);
	} else
	{
		mqtt_publish( LOG_NOTICE , "Wifi" ,
			"{\"SSID:\":\"%s\",\"BSSID\":\"%s\",\"RSSI\":%d,\"channel\":%d,\"WiFiPhyMode\":%d}"
						, WiFi.SSID().c_str()
						, WiFi.BSSIDstr().c_str()
						, WiFi.RSSI()
						, WiFi.channel()
						, WiFi.getPhyMode()
					);
	}
	
	/*
	WiFiPhyMode_t getPhyMode();
	typedef enum WiFiPhyMode
	{
		WIFI_PHY_MODE_11B = 1, WIFI_PHY_MODE_11G = 2, WIFI_PHY_MODE_11N = 3
	} WiFiPhyMode_t;
	*/
	_timer_module_info.countdown( 5 );
	return( 0 );
}

error_t ICACHE_FLASH_ATTR DLRWifiManager::reconnect_STA()
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
	return( 0 );
}