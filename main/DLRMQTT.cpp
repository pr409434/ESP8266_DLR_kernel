#include "DLRMQTT.h"
#include "DLRMQTTMessages.h"



//#define MQTT_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> callback
void ICACHE_FLASH_ATTR _MQTTcallback( char* topic , byte* payload , unsigned int length )
{
	addLog( 0 , LOG_DEBUG , "MQTT message %s , topic: %s\n"
			, topic
		);
	Serial.println("\n/*********************************************/");
	for (int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
	}
	Serial.println("/*********************************************/\n");
	Serial.println();
}



// connect (clientID, username, password, willTopic, willQoS, willRetain, willMessage)

ICACHE_FLASH_ATTR DLRMQTT::DLRMQTT()
{
	if( name == nullptr )
	{
		name = (char *)  "MQTT";
	}
	priority = 0;
	server   = mqtt_server;
	//port     = mqtt_port;
	username = mqtt_username;
	password = mqtt_password;
	_tcp_Client  = new WiFiClient(); //WiFiClientSecure _tcp_Client;
	_mqtt_client = new PubSubClient( *_tcp_Client );
	//_tcp_Client.setCACert(ca_crt, ca_crt_len);
    //_tcp_Client.setCertificate(dlr_000_crt, dlr_000_crt_len);
    //_tcp_Client.setPrivateKey(dlr_000_key, dlr_000_key_len);
	_mqtt_client->setServer(   server.c_str() , port );
    _mqtt_client->setCallback( _MQTTcallback );
}

/*
DLRMQTT::DLRMQTT( const char* param_server  , uint16_t param_port , const char* param_username , const char* param_password )
{
	//_tcp_Client  = new WiFiClient(); //WiFiClientSecure _tcp_Client;
	//_mqtt_client = new PubSubClient( *_tcp_Client );
	server   = param_server;
	port     = param_port;
	username = param_username;
	password = param_password;
	if( name == nullptr )
	{
		name = "MQTT";
	}
	priority = 0;
	//_tcp_Client.setCACert(ca_crt, ca_crt_len);
    //_tcp_Client.setCertificate(dlr_000_crt, dlr_000_crt_len);
    //_tcp_Client.setPrivateKey(dlr_000_key, dlr_000_key_len);
	_mqtt_client->setServer(   server.c_str() , port );
    _mqtt_client->setCallback( _MQTTcallback );
}
*/

ICACHE_FLASH_ATTR DLRMQTT::~DLRMQTT()
{
	
}

error_t ICACHE_FLASH_ATTR DLRMQTT::loop()
{
	/*
	if (
			   (   WiFi.status() == WL_CONNECTED )
			&& ( ! _mqtt_client->connected() )
			&& (   _timer_reconnect.expired() )
		)
	{
		reconnect();
	}
	if (
			   (   WiFi.status() == WL_CONNECTED )
			&& (   _mqtt_client->connected() )
			&& (   _timer_sysinfo.expired() )
		)
	{
		this->sysinfo();
	}
	*/
	if( _mqtt_client->loop() )
	{
		size_t _tcp_Client_size = _tcp_Client->availableForWrite();
		if ( _tcp_Client_size >= 2900 )
		{
			//addLog( 0 , LOG_DEBUG , "DLRMQTT availableForWrite: %ld\n" , _tcp_Client_size );
			if( ! MQTTMessagesQueue.empty() )
			{
				addLog( 0 , LOG_DEBUG , "_mqtt_client->publish...\n" );
				_mqtt_client->publish( MQTTMessagesQueue.top()->topic.c_str() ,
									   MQTTMessagesQueue.top()->payload.text
									 );
				delete MQTTMessagesQueue.top();
				MQTTMessagesQueue.pop();
			}
			/*
			if (   _timer_sysinfo.expired() )
			{
				this->sysinfo();
				addLog( 0 , LOG_DEBUG , "DLRMQTT availableForWrite: %ld to %ld\n"
						, _tcp_Client_size
						, _tcp_Client_size - _tcp_Client->availableForWrite()
						);
			}
			*/
		}
	} else
	{
		reconnect();
	}
}

error_t ICACHE_FLASH_ATTR DLRMQTT::setup()
{
	_timer_reconnect.countdown( 10 );
	_timer_sysinfo.countdown( 60 );
}

error_t ICACHE_FLASH_ATTR DLRMQTT::reconnect()
{
	if (
			   (    WiFi.status() == WL_CONNECTED )
			&& ( ! _mqtt_client->connected() )
			&& (   _timer_reconnect.expired() )
	)
	{
		
		addLog( 0 , LOG_DEBUG , "Attempting MQTT connection...\n");
        if( _mqtt_client->connect ( WiFi.hostname().c_str() , username.c_str(), password.c_str() ) )
		{
			addLog( 0 , LOG_DEBUG , "%s connected to: %s\n" , name , server.c_str() );
            // Resubscribe to all your topics here so that they are
            // resubscribed each time you reconnect
			//_mqtt_client->publish("outTopic", "hello world");
			this->sysinfo();
			_mqtt_client->subscribe("inTopic");
        } else {
			addLog( 0 , LOG_DEBUG , "failed, rc=%d retry again in 10 seconds\n" , _mqtt_client->state() );
			_timer_reconnect.countdown( 10 ); // 30 seconds
        }
    }
}
error_t ICACHE_FLASH_ATTR DLRMQTT::sysinfo()
{
	time_t timestamp = time( NULL );
	_mqtt_client->publish( String_Format( "/%s/sysinfo" , WiFi.hostname().c_str() ).c_str() ,
						   String_Format( "{\"timestamp\":%ld,\"uptime\":%ld,\"getFreeHeap\":%ld}"
											, timestamp
											, timestamp - system_boot_time
											, ESP.getFreeHeap()
											).c_str()
							);
	/*
		ESP.getSketchMD5()
        uint32_t getSketchSize();
        String getSketchMD5();
        uint32_t getFreeSketchSpace();

		String getResetReason();
        String getResetInfo();
	*/
	_mqtt_client->publish( String_Format( "/%s/wifi_sta_info" , WiFi.hostname().c_str() ).c_str() ,
						   String_Format(
									"{\"SSID:\":\"%s\",\"BSSID\":\"%s\",\"RSSI\":%d,\"channel\":%d}"
									, WiFi.SSID().c_str()
									, WiFi.BSSIDstr().c_str()
									, WiFi.RSSI()
									, WiFi.channel()
									).c_str()
							);
	/*
	WiFiPhyMode_t getPhyMode();
	typedef enum WiFiPhyMode
	{
		WIFI_PHY_MODE_11B = 1, WIFI_PHY_MODE_11G = 2, WIFI_PHY_MODE_11N = 3
	} WiFiPhyMode_t;
	*/
	_timer_sysinfo.countdown( 10 );
}
/*
error_t DLRMQTT::sysinfo()
{
	_mqtt_client->publish( 
			String_Format( "/%s/sysinfo" , WiFi.hostname().c_str() ).c_str() ,
			String_Format( "{\n" \
				"getChipId:          %ld\n" \
				",getSdkVersion:      %s\n" \
				",getCoreVersion:     %s\n" \
				",getFreeHeap:        %ld\n" \
				",getCpuFreqMHz:      %ld\n" \
				",getSketchSize:      %ld\n" \
				",getFreeSketchSpace: %ld\n" \
				",getCycleCount:      %ld\n" \
				"\n}"
				, ESP.getChipId()
				, ESP.getSdkVersion()
				, ESP.getCoreVersion().c_str()
				, ESP.getFreeHeap()
				, ESP.getCpuFreqMHz()
				, ESP.getSketchSize()
				, ESP.getFreeSketchSpace()
				, ESP.getFreeSketchSpace()
				, ESP.getCycleCount()
			).c_str()
		);
}
*/
	