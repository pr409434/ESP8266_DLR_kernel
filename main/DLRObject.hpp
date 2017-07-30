#ifndef DLRObject_HPP
#define DLRObject_HPP

#include <stdint.h>
#include <stdarg.h>
#include <inttypes.h>
#include <WString.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>



#define SYSLOG_NILVALUE "-"

// Syslog protocol format
#define SYSLOG_PROTO_IETF 0  // RFC 5424
#define SYSLOG_PROTO_BSD 1   // RFC 3164

/*
 * priorities/facilities are encoded into a single 32-bit quantity, where the
 * bottom 3 bits are the priority (0-7) and the top 28 bits are the facility
 * (0-big number).  Both the priorities and the facilities map roughly
 * one-to-one to strings in the syslogd(8) source code.  This mapping is
 * included in this file.
 *
 * priorities (these are ordered)
 */
#define LOG_EMERG 0 /* system is unusable */
#define LOG_ALERT 1 /* action must be taken immediately */
#define LOG_CRIT  2 /* critical conditions */
#define LOG_ERR   3 /* error conditions */
#define LOG_WARNING 4 /* warning conditions */
#define LOG_NOTICE  5 /* normal but significant condition */
#define LOG_INFO  6 /* informational */
#define LOG_DEBUG 7 /* debug-level messages */

#define LOG_PRIMASK 0x07  /* mask to extract priority part (internal) */
        /* extract priority */
#define LOG_PRI(p)  ((p) & LOG_PRIMASK)
#define LOG_MAKEPRI(fac, pri) (((fac) << 3) | (pri))

/* facility codes */
#define LOG_KERN  (0<<3)  /* kernel messages */
#define LOG_USER  (1<<3)  /* random user-level messages */
#define LOG_MAIL  (2<<3)  /* mail system */
#define LOG_DAEMON  (3<<3)  /* system daemons */
#define LOG_AUTH  (4<<3)  /* security/authorization messages */
#define LOG_SYSLOG  (5<<3)  /* messages generated internally by syslogd */
#define LOG_LPR   (6<<3)  /* line printer subsystem */
#define LOG_NEWS  (7<<3)  /* network news subsystem */
#define LOG_UUCP  (8<<3)  /* UUCP subsystem */
#define LOG_CRON  (9<<3)  /* clock daemon */
#define LOG_AUTHPRIV  (10<<3) /* security/authorization messages (private) */
#define LOG_FTP   (11<<3) /* ftp daemon */

/* other codes through 15 reserved for system use */
#define LOG_LOCAL0  (16<<3) /* reserved for local use */
#define LOG_LOCAL1  (17<<3) /* reserved for local use */
#define LOG_LOCAL2  (18<<3) /* reserved for local use */
#define LOG_LOCAL3  (19<<3) /* reserved for local use */
#define LOG_LOCAL4  (20<<3) /* reserved for local use */
#define LOG_LOCAL5  (21<<3) /* reserved for local use */
#define LOG_LOCAL6  (22<<3) /* reserved for local use */
#define LOG_LOCAL7  (23<<3) /* reserved for local use */

#define LOG_NFACILITIES 24  /* current number of facilities */
#define LOG_FACMASK 0x03f8  /* mask to extract facility part */
                            /* facility of pri */
#define LOG_FAC(p)  (((p) & LOG_FACMASK) >> 3)

#define LOG_MASK(pri)  (1 << (pri))	/* mask for one priority */
#define LOG_UPTO(pri)  ((1 << ((pri)+1)) - 1)	/* all priorities through pri */

#define LOG_ObjectID_PRI( fac , pri ) ((( fac + 31 ) << 3) | (pri))


typedef uint8_t error_t;
typedef size_t  ObjectID_t;


#include <stdarg.h>  // For va_start, etc.

std::string string_format(const std::string fmt, ...) {
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::string str;
    va_list ap;
    while (1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
}

//std::string string_format(const std::string fmt, ...)
String String_Format( const String fmt , ... )
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
			return str;
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
	return str;
}


class DLRObject;

std::vector< DLRObject *  > DLRObjectStack;

class DLRMessage
{
	public:
		time_t      timestamp;
		uint32_t	microSecond;
		
		ObjectID_t  from_ObjectID;
		uint16_t    SysLog_Code;

		char                      *payload   = nullptr;

		DLRMessage( ObjectID_t ObjectID , uint16_t pri , const String format , ... )
		{
			microSecond = micros() % 1000000;
			timestamp   = ( millis() / 1000 ) + TimerTimeOffset;
			
			from_ObjectID = ObjectID;
			
			SysLog_Code   = LOG_ObjectID_PRI( ObjectID , pri );

			char buffer[0xff];
			va_list args;
			va_start( args , format );
			int message_size = vsnprintf( buffer , 0xff , format.c_str() , args );
			va_end (args);
			
			if( message_size > 0 )
			{
				//  Copy buffer buffer
				payload = new char[ message_size +1 ];
				strncpy ( payload , buffer , message_size );
				payload[message_size] = 0;
			}
		}
		DLRMessage()
		{
			//payload = new char[80];
		}
		~DLRMessage()
		{
			if( payload != nullptr )
			{
				delete payload;
			}
		}
		error_t setup()
		{
			return( 0 );
		}
		error_t loop()
		{
			return( 0 );
		}
};


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

/*
MessagesQueue.push( new DLRMessage( ObjectID , pri , message ) );
*/

error_t addLog( ObjectID_t ObjectID , uint16_t pri ,  const String format , ...  )
{
	va_list args;
	va_start( args , format );
	MessagesQueue.push( new DLRMessage( ObjectID , pri , format , args ) );
	va_end (args);
	return( 0 );
}

/*
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
extern error_t addLog( ObjectID_t ObjectID , uint16_t pri , const __FlashStringHelper *message );
extern error_t addLog( ObjectID_t ObjectID , uint16_t pri , const String &message );
extern error_t addLog( ObjectID_t ObjectID , uint16_t pri , const char *message );

extern error_t vaddLogf(   ObjectID_t ObjectID , uint16_t pri, const char *fmt, va_list args) __attribute__((format(printf, 3, 0)));
extern error_t vaddLogf_P( ObjectID_t ObjectID , uint16_t pri, PGM_P fmt_P, va_list args) __attribute__((format(printf, 3, 0)));

extern error_t addLogf( ObjectID_t ObjectID , uint16_t pri, const char *fmt, ...) __attribute__((format(printf, 3, 4)));

extern error_t addLogf_P( ObjectID_t ObjectID , uint16_t pri, PGM_P fmt_P, ...) __attribute__((format(printf, 3, 4)));
*/

class DLRObject
{
	public:
		DLRObject()
		{
			ObjectID = DLRObjectStack.size();
			DLRObjectStack.push_back( this );
		}
		~DLRObject()
		{
			DLRObjectStack[ObjectID] = nullptr;
		}
		virtual error_t setup()
		{
			return( 0 );
		}
		virtual error_t loop()
		{
			return( 0 );
		}
		virtual error_t status()
		{
			return( 0 );
		}
		virtual error_t message_handler( DLRMessage *message )
		{
			return( 0 );
		}
	private:
		ObjectID_t ObjectID;
};




#endif //DLRObject_HPP