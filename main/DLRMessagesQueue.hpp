#ifndef DLRMessagesQueue_HPP
#define DLRMessagesQueue_HPP

#include <stdint.h>
#include <vector>
#include <queue>
#include <functional>

class DLRMessagePriorityComparison
{
	bool reverse;
	public:
		DLRMessagePriorityComparison( const bool& revparam = false )
		{
			reverse=revparam;
		}
		bool operator() ( DLRMessage& lhs , DLRMessage& rhs )
		{
			if ( reverse ) return (
										LOG_PRI( lhs.SysLog_Code ) >
										LOG_PRI( rhs.SysLog_Code )
									);
			else return (
								LOG_PRI( lhs.SysLog_Code ) <
								LOG_PRI( rhs.SysLog_Code )
							);
		}
};

typedef std::priority_queue<DLRMessage,std::vector<DLRMessage>,DLRMessagePriorityComparison> DLRMessagesQueue;
DLRMessagesQueue MessagesQueue;

error_t _sendLog( ObjectID_t ObjectID , uint16_t pri , const char *message )
{
	return( 0 );
}

error_t _sendLog( ObjectID_t ObjectID , uint16_t pri, const __FlashStringHelper *message )
{
	return( 0 );
}

error_t log( ObjectID_t ObjectID ,uint16_t pri, const __FlashStringHelper *message)
{
	return( _sendLog( ObjectID , pri , message ) );
}

error_t log( ObjectID_t ObjectID , uint16_t pri, const String &message)
{
	return( _sendLog( ObjectID , pri , message.c_str() ) );
}

error_t log( ObjectID_t ObjectID ,uint16_t pri, const char *message)
{
	return( _sendLog( ObjectID , pri, message) );
}


error_t vlogf( ObjectID_t ObjectID ,uint16_t pri, const char *fmt, va_list args) {
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

error_t vlogf_P( ObjectID_t ObjectID ,uint16_t pri, PGM_P fmt_P, va_list args) {
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


error_t logf( ObjectID_t ObjectID ,uint16_t pri, const char *fmt, ...) {
  va_list args;
  error_t result;

  va_start(args, fmt);
  result = vlogf(  ObjectID , pri, fmt, args);
  va_end(args);
  return result;
}


error_t logf_P( ObjectID_t ObjectID ,uint16_t pri, PGM_P fmt_P, ...) {
  va_list args;
  error_t result;

  va_start(args, fmt_P);
  result = vlogf_P(  ObjectID , pri , fmt_P, args);
  va_end(args);
  return result;
}



class DLRMessagesQueueManager: public DLRObject
{
	public:
		DLRMessagesQueueManager()
		{}
		~DLRMessagesQueueManager()
		{}
} DLRMessagesQueueManager;


#endif //DLRMessagesQueue_HPP