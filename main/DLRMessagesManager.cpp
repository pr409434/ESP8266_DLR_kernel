
#include "DLRMessagesManager.h"

DLRMessagesQueueManager::DLRMessagesQueueManager()
{}	
DLRMessagesQueueManager::~DLRMessagesQueueManager()
{}
error_t DLRMessagesQueueManager::loop()
{
	if ( current_message_size <= 0 )
	{
		if( ! MessagesQueue.empty() ) //.size() > 0 )
		{
			if( current_message != nullptr )
			{
				delete current_message;
			}
			
			size_t payload_size = strlen( MessagesQueue.top()->payload );
			current_message = new char[payload_size];
			strncpy ( current_message , MessagesQueue.top()->payload , payload_size );
			current_message_idx  = 0;
			current_message_size = payload_size;

			delete MessagesQueue.top();
			MessagesQueue.pop();
		}
	}
	send_current_message();
	return( 0 );
}

error_t DLRMessagesQueueManager::send_current_message()
{
	if ( Serial )
	{
		int serial_packet_size = Serial.availableForWrite();
		if ( serial_packet_size > 0x0f )
		{
			if ( current_message_size > 0 )
			{
				serial_packet_size -= 0x07;
				char * current_message_ptr = current_message + current_message_idx;
				int16_t serial_packet_send_size = std::min( serial_packet_size , current_message_size - current_message_idx );
				if( serial_packet_send_size > 0 )
				{
					Serial.write( current_message_ptr , serial_packet_send_size );
					current_message_idx += serial_packet_send_size;
				}
				if( current_message_idx >= current_message_size )
				{
					if( current_message != nullptr )
					{
						delete current_message;
					}
					current_message      = nullptr;
					current_message_idx  = 0;
					current_message_size = 0;
				}
			}
		}
	}
	return( 0 );
}

DLRMessagesQueueManager MessagesQueueManager;