
#include "DLRMessages.h"

static uint32_t  DLRMessageCounter = 0;
static time_t    system_boot_time  = 0;
static time_t    TimerTimeOffset   = 0;
static uint32_t _DLRMessageLastMicroTimeStamp  = 0;

DLRMessage::DLRMessage( ObjectID_t ObjectID , uint16_t pri , const char *message )
{
	DLRMessageCounter +=1;
	Number = DLRMessageCounter;
	uint32_t _TimeMicroStamp = millis();
	if( _DLRMessageLastMicroTimeStamp > _TimeMicroStamp )
	{
		TimerTimeOffset += 1;
	}
	_DLRMessageLastMicroTimeStamp = _TimeMicroStamp;
	microSecond = micros() % 1000000;
	timestamp   = ( millis() / 1000 ) + TimerTimeOffset;
	
	from_ObjectID = ObjectID;
	
	SysLog_Code   = LOG_ObjectID_PRI( ObjectID , pri );
	
	size_t message_size = strlen( message );
	payload = new char[ message_size +1 ];
	strncpy( payload , message , message_size );
	payload[message_size] = 0;
}
DLRMessage::DLRMessage()
{
	DLRMessageCounter +=1;
	Number = DLRMessageCounter;
	//payload = new char[80];
}
DLRMessage::~DLRMessage()
{
	if( payload != nullptr )
	{
		delete payload;
	}
}


DLRMessagePriorityComparison::DLRMessagePriorityComparison( const bool& revparam = false )
{
	reverse=revparam;
}
bool DLRMessagePriorityComparison::operator() ( DLRMessage * lhs , DLRMessage * rhs )
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

DLRMessagesQueue MessagesQueue;

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
