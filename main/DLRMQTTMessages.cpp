
#include "DLRMQTTMessages.h"

static uint32_t  DLRMQTTMessageCounter = 0;

ICACHE_FLASH_ATTR DLRMQTTMessage::DLRMQTTMessage( uint16_t pri , const char* param_topic , uint8_t* param_payload , size_t param_size )
{
	DLRMQTTMessageCounter +=1;
	Number         = DLRMQTTMessageCounter;
	microtimestamp = microtimes();
	SysLog_Code    = LOG_ObjectID_PRI( 0 , pri );
	topic          = String_Format( "/%s/%s" , WiFi.hostname().c_str() , param_topic );
	
	type   = MQTT_BINARY;
	lenght = param_size;
	std::memcpy( payload.binary , param_payload , lenght );
}



ICACHE_FLASH_ATTR DLRMQTTMessage::DLRMQTTMessage( uint16_t pri , const char* param_topic , const char* param_payload )
{
	DLRMQTTMessageCounter +=1;
	Number         = DLRMQTTMessageCounter;
	microtimestamp = microtimes();
	SysLog_Code    = LOG_ObjectID_PRI( 0 , pri );
	topic          = String_Format( "/%s/%s" , WiFi.hostname().c_str() , param_topic );
	
	type   = MQTT_TEXT;
	lenght = strlen( param_payload ) + 1;
	payload.text = new char[ lenght ];
	std::memcpy( payload.text , param_payload , lenght - 1 );
	payload.text[lenght] = 0;
	
}

ICACHE_FLASH_ATTR DLRMQTTMessage::~DLRMQTTMessage()
{
	if( lenght > 0 )
	{
		switch( type )
		{
			case MQTT_TEXT:
					delete payload.text;
				break;
			case MQTT_BINARY:
					delete payload.binary;
				break;
			default:
					delete payload.text;
					delete payload.binary;
				break;
		}
	}
}


ICACHE_FLASH_ATTR DLRMQTTMessagePriorityComparison::DLRMQTTMessagePriorityComparison( const bool& revparam = false )
{
	reverse=revparam;
}
bool ICACHE_FLASH_ATTR DLRMQTTMessagePriorityComparison::operator() ( DLRMQTTMessage * lhs , DLRMQTTMessage * rhs )
{
	if ( reverse )
	{
		if( LOG_PRI( lhs->SysLog_Code ) <  LOG_PRI( rhs->SysLog_Code ) )
		{
			return( true );
		}
		else if( LOG_PRI( lhs->SysLog_Code ) == LOG_PRI( rhs->SysLog_Code ) )
		{
			return( lhs->Number < rhs->Number );
		}
		return( false );
	}
	else
	{
		if( LOG_PRI( lhs->SysLog_Code ) >  LOG_PRI( rhs->SysLog_Code ) )
		{
			return( true );
		}
		else if( LOG_PRI( lhs->SysLog_Code ) == LOG_PRI( rhs->SysLog_Code ) )
		{
			return( lhs->Number > rhs->Number );
		}
		return( false );
	}
}

DLRMQTTMessagesQueue MQTTMessagesQueue;

/*
MessagesQueue.push( new DLRMQTTMessage( ObjectID , pri , message ) );
*/

error_t ICACHE_FLASH_ATTR mqtt_publish( uint16_t pri , const char* topic , const String fmt , ... )
{
	int size = ( (int) fmt.length() ) + 32; //* 2 + 50;   // Use a rubric appropriate for your code
	String str;
	va_list ap;
	while (1) // Maximum two passes on a POSIX system...
	{     
		if( ! str.reserve( size ) )
		{
			break;
		}
		va_start( ap , fmt );
		// Notice that only when this returned value is non-negative and less than n, the string has been completely written.
		int n = vsnprintf( (char *) str.begin() , size , fmt.c_str() , ap );
		va_end( ap );
		if ( n > -1 && n < size )
		{
			// Everything worked
			str.reserve( n );
			//return str;
			break;
		}
		if ( n > -1 )
		{
			// Needed size returned
			size = n + 1;   // For null char
		}
		else
		{
			// Guess at a larger size (OS specific) => attempt to crash...
			size *= 2;
		}
	}
	MQTTMessagesQueue.push( new DLRMQTTMessage( pri , topic , str.c_str() ) );

	return( 0 );
}
