#ifndef DLRMessagesManager_HPP
#define DLRMessagesManager_HPP



/*
MessagesQueue.push( new DLRMessage( ObjectID , pri , message ) );
*/


error_t addLog( ObjectID_t ObjectID , uint16_t pri , const String fmt , ... )
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
	MessagesQueue.push( new DLRMessage( ObjectID , pri , str.c_str() ) );
	//delete str;
	return( 0 );
}

class DLRMessagesQueueManager: public DLRObject
{
	char   *current_message     = nullptr;
	int     current_message_idx  = 0;
	int     current_message_size = 0;
	public:
		DLRMessagesQueueManager()
		{}
		
		~DLRMessagesQueueManager()
		{}
		
		error_t loop()
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
		
		error_t send_current_message()
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
		
};


#endif //DLRMessagesManager_HPP