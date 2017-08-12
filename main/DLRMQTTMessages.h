#ifndef DLRMQTTMessages_H
#define DLRMQTTMessages_H

#include "Configuration.h"

typedef enum MQTT_payload_type
{
	MQTT_Unknow = 0,
    MQTT_TEXT   = 1,
    MQTT_BINARY = 2
} MQTT_payload_type;

typedef union MQTT_payload
{
	uint8_t* binary;
	char*    text;
} MQTT_payload;


class DLRMQTTMessage
{
	public:
		uint32_t    Number = 0;
		uint64_t	microtimestamp = 0;
		
		uint16_t    SysLog_Code;

		String topic;
		MQTT_payload_type     type    = MQTT_Unknow;
		MQTT_payload          payload;
		size_t                lenght  = 0;

		DLRMQTTMessage( uint16_t pri , const char *param_topic , const char*    param_payload );
		DLRMQTTMessage( uint16_t pri , const char *param_topic , uint8_t* param_payload , size_t param_size );
		~DLRMQTTMessage();
};

class DLRMQTTMessagePriorityComparison
{
	bool reverse;
	public:
		DLRMQTTMessagePriorityComparison( const bool& revparam );
		bool operator() ( DLRMQTTMessage * lhs , DLRMQTTMessage * rhs );
};

typedef std::priority_queue<DLRMQTTMessage *,std::vector<DLRMQTTMessage *>,DLRMQTTMessagePriorityComparison> DLRMQTTMessagesQueue;
extern DLRMQTTMessagesQueue MQTTMessagesQueue;


#endif //DLRMQTTMessage_H