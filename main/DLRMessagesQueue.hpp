#ifndef DLRMessagesQueue_HPP
#define DLRMessagesQueue_HPP

#include <stdint.h>
#include <algorithm>
#include <functional>
#include <vector>
#include <queue>

/*
class DLRMessagePriorityComparison
{
	bool reverse;
	public:
		DLRMessagePriorityComparison( const bool& revparam = false )
		{
			reverse=revparam;
		}
		bool operator() ( DLRMessage * lhs , DLRMessage * rhs )
		{
			if ( reverse ) return (
										LOG_PRI( lhs->SysLog_Code ) >
										LOG_PRI( rhs->SysLog_Code )
									);
			else return (
								LOG_PRI( lhs->SysLog_Code ) <
								LOG_PRI( rhs->SysLog_Code )
							);
		}
};

typedef std::priority_queue<DLRMessage *,std::vector<DLRMessage *>,DLRMessagePriorityComparison> DLRMessagesQueue;
DLRMessagesQueue MessagesQueue;



error_t _sendLog( ObjectID_t ObjectID , uint16_t pri , const char *message )
{
	//strlen(message);
	MessagesQueue.push( new DLRMessage( ObjectID , pri , message ) );
	return( 0 );
}

error_t _sendLog( ObjectID_t ObjectID , uint16_t pri, const __FlashStringHelper *message )
{
	//strlen(message);
	return( 0 );
}

error_t addLogaddLog( ObjectID_t ObjectID ,uint16_t pri, const __FlashStringHelper *message)
{
	return( _sendLog( ObjectID , pri , message ) );
}

error_t addLog( ObjectID_t ObjectID , uint16_t pri, const String &message)
{
	return( _sendLog( ObjectID , pri , message.c_str() ) );
}

error_t addLog( ObjectID_t ObjectID ,uint16_t pri, const char *message)
{
	return( _sendLog( ObjectID , pri, message) );
}


error_t vaddLogf( ObjectID_t ObjectID ,uint16_t pri, const char *fmt, va_list args) {
  char *message;
  size_t initialLen;
  size_t len;
  error_t result;

  initialLen = strlen(fmt);

  message = new char[initialLen + 1];

  len = vsnprintf(message, initialLen, fmt, args);
  if (len > initialLen) {
    delete[] message;
    message = new char[len + 1];

    vsnprintf(message, len, fmt, args);
  }

  result = _sendLog( ObjectID , pri , message );

  delete[] message;
  return result;
}

error_t vaddLogf_P( ObjectID_t ObjectID ,uint16_t pri, PGM_P fmt_P, va_list args) {
  char *message;
  size_t initialLen;
  size_t len;
  error_t result;

  initialLen = strlen_P(fmt_P);

  message = new char[initialLen + 1];

  len = vsnprintf_P(message, initialLen, fmt_P, args);
  if (len > initialLen) {
    delete[] message;
    message = new char[len + 1];

    vsnprintf(message, len, fmt_P, args);
  }

  result = _sendLog( ObjectID , pri, message);

  delete[] message;
  return result;
}


error_t addLogf( ObjectID_t ObjectID ,uint16_t pri, const char *fmt, ...) {
  va_list args;
  error_t result;

  va_start(args, fmt);
  result = vaddLogf(  ObjectID , pri, fmt, args);
  va_end(args);
  return result;
}


error_t addLogf_P( ObjectID_t ObjectID ,uint16_t pri, PGM_P fmt_P, ...)
{
  va_list args;
  error_t result;

  va_start(args, fmt_P);
  result = vaddLogf_P(  ObjectID , pri , fmt_P, args);
  va_end(args);
  return result;
}
*/

/*
		DLRMessage& operator=( const DLRMessage& other ) // copy assignment
		{
			if ( this != &other )            // sanity self-assignment check expected
			{
				timestamp      = other.timestamp;
				microSecond    = other.microSecond;
				from_ObjectID  = other.from_ObjectID;
				SysLog_Code    = other.SysLog_Code;
				F_payload      = other.F_payload;
				if( payload != nullptr )
				{
						delete payload;
						size_t payload_size = strlen(other.payload);
						payload = new char[payload_size];
						strncpy ( payload , other.payload , payload_size );
				}
			}
			return *this;
		}
*/

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
		
} MessagesQueueManager;


#endif //DLRMessagesQueue_HPP