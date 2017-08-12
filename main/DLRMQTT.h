#ifndef DLRMQTT_H
#define DLRMQTT_H

#include "DLRObject.h"

class DLRMQTT: public DLRObject
{
	public:
		DLRMQTT();
		//DLRMQTT( const char* param_server , uint16_t param_port , const char* param_username , const char* param_password );
		
		~DLRMQTT();
		error_t loop();
		error_t setup();
		
		error_t reconnect();
		error_t sysinfo();
		
		
		//void callback( char* topic , byte* payload , unsigned int length );
		
	private:
		String   server;
		uint16_t port = 1883;
		String   username;
		String   password;
		
		Countdown     _timer_sysinfo;
	
		Countdown     _timer_reconnect;
		WiFiClient*   _tcp_Client; //WiFiClientSecure _tcp_Client;
		PubSubClient* _mqtt_client;
		
};

#endif //DLRMQTT_H