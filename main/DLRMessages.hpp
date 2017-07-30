#ifndef DLRMessages_HPP
#define DLRMessages_HPP

static uint32_t DLRMessageCounter = 0;

class DLRMessage
{
	public:
		uint32_t    Number = 0;
		time_t      timestamp = 0;
		uint32_t	microSecond = 0;
		
		ObjectID_t  from_ObjectID;
		uint16_t    SysLog_Code;

		char        *payload   = nullptr;

		DLRMessage( ObjectID_t ObjectID , uint16_t pri , const char *message )
		{
			DLRMessageCounter +=1;
			Number = DLRMessageCounter;
			
			microSecond = micros() % 1000000;
			timestamp   = ( millis() / 1000 ) + TimerTimeOffset;
			
			from_ObjectID = ObjectID;
			
			SysLog_Code   = LOG_ObjectID_PRI( ObjectID , pri );
			
			size_t message_size = strlen( message );
			payload = new char[ message_size +1 ];
			strncpy( payload , message , message_size );
			payload[message_size] = 0;
		}
		DLRMessage()
		{
			DLRMessageCounter +=1;
			Number = DLRMessageCounter;
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
			if ( reverse )
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
			else
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
		}
};








#endif //DLRMessage_HPP