#include "DLRMQTT.h"
#include "DLRMQTTMessages.h"



//#define MQTT_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> callback
void ICACHE_FLASH_ATTR _MQTTcallback( char* mqtt_topic , byte* mqtt_payload , unsigned int mqtt_length )
{
	String topic( mqtt_topic );
	String payload( std::string( (char*) mqtt_payload , mqtt_length ).c_str() );
	addLog( 0 , LOG_DEBUG , "MQTT message topic:[%s] , payload: [%s]\n"
			, topic.c_str()
			, payload.c_str()
		);
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject( payload );
	if( root.success() )
	{
		POWER_RELAY_VALUE = ( root["power"] == 0 ? HIGH : LOW );
		addLog( 0 , LOG_DEBUG , "POWER_RELAY_VALUE:%d\n" , POWER_RELAY_VALUE );
	}
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
	//MQTTMessagesQueue
	if( MQTTMessagesQueue.size() > MAXIMUM_MQTTMESSAGES_QUEUE )
	{
		//DLRMQTTMessagesQueue Temporary_MQTTMessagesQueue;
		uint16_t counter = 0;
		uint16_t delete_counter = 0;
		while ( ! MQTTMessagesQueue.empty())
		{
			if( counter < MAXIMUM_MQTTMESSAGES_QUEUE )
			{
				Temporary_MQTTMessagesQueue.push( MQTTMessagesQueue.top() );
			} else
			{
				delete MQTTMessagesQueue.top();
				delete_counter += 1;
			}
			MQTTMessagesQueue.pop();
			counter += 1;
		}
		while ( ! Temporary_MQTTMessagesQueue.empty())
		{
			MQTTMessagesQueue.push( Temporary_MQTTMessagesQueue.top() );
			Temporary_MQTTMessagesQueue.pop();
		}
		addLog( ObjectID , LOG_WARNING , "DELETE %ld MQTTMessages\n" , delete_counter );
	}
	if( _mqtt_client->loop() )
	{
		size_t _tcp_Client_size = _tcp_Client->availableForWrite();
		if (
				   ( _tcp_Client_size >= 2900 )
				&& ( _timer_mqtt_send.expired() )
			)
		{
			if( ! MQTTMessagesQueue.empty() )
			{
				_mqtt_client->publish( MQTTMessagesQueue.top()->topic.c_str() ,
									   (uint8_t*) MQTTMessagesQueue.top()->payload.text ,
									   (uint8_t)  MQTTMessagesQueue.top()->lenght
									 );
				delete MQTTMessagesQueue.top();
				MQTTMessagesQueue.pop();
				_timer_mqtt_send.countdown_ms( DLR_MQTT_TIMER_MQTT_SEND_TIME_MS );
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
	module_info();
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
			_mqtt_client->subscribe("/ESP_2318C6/subdue");
			_timer_mqtt_send.countdown( 5 );
        } else {
			addLog( 0 , LOG_DEBUG , "failed, rc=%d retry again in 10 seconds\n" , _mqtt_client->state() );
			_timer_reconnect.countdown( 10 ); // 30 seconds
        }
    }
	return( 0 );
}
error_t ICACHE_FLASH_ATTR DLRMQTT::module_info()
{
	time_t timestamp   = time( NULL );
	addLog( ObjectID , LOG_NOTICE ,
			"{\"timestamp\":%ld,\"MQTTMessagesQueue\":%ld}\n"
				, timestamp
				, MQTTMessagesQueue.size()
				);
	mqtt_publish( LOG_INFO , "sysinfo" ,
			"{\"timestamp\":%ld,\"MQTTMessagesQueue\":%ld}\n"
				, timestamp
				, MQTTMessagesQueue.size()
				);
	
	_timer_module_info.countdown( DLR_MQTT_MODULE_INFO_TIME );
	return( 0 );
}

	