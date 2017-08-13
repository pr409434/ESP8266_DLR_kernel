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
	if( _mqtt_client->loop() )
	{
		size_t _tcp_Client_size = _tcp_Client->availableForWrite();
		if ( _tcp_Client_size >= 2900 )
		{
			if( ! MQTTMessagesQueue.empty() )
			{
				_mqtt_client->publish( MQTTMessagesQueue.top()->topic.c_str() ,
									   (uint8_t*) MQTTMessagesQueue.top()->payload.text ,
									   (uint8_t)  MQTTMessagesQueue.top()->lenght
									 );
				delete MQTTMessagesQueue.top();
				MQTTMessagesQueue.pop();
			}
		}
	} else
	{
		reconnect();
	}
	if( _timer_module_info.expired() )
	{
		module_info();
	}
	return( 0 );
}

error_t ICACHE_FLASH_ATTR DLRMQTT::setup()
{
	_timer_reconnect.countdown( 10 );
	_timer_module_info.countdown( 60 );
	return( 0 );
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
			this->module_info();
            // Resubscribe to all your topics here so that they are
            // resubscribed each time you reconnect
			_mqtt_client->subscribe("inTopic");
        } else {
			addLog( 0 , LOG_DEBUG , "failed, rc=%d retry again in 10 seconds\n" , _mqtt_client->state() );
			_timer_reconnect.countdown( 10 ); // 30 seconds
        }
    }
	return( 0 );
}
error_t ICACHE_FLASH_ATTR DLRMQTT::module_info()
{
	_timer_module_info.countdown( 60 );
	return( 0 );
}

	