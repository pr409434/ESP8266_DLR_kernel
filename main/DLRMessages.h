#ifndef DLRMessages_H
#define DLRMessages_H

#include "Configuration.h"

extern time_t system_boot_time;

class DLRMessage
{
	public:
		uint32_t    Number = 0;
		time_t      timestamp = 0;
		uint32_t	microSecond = 0;
		
		ObjectID_t  from_ObjectID;
		uint16_t    SysLog_Code;

		char        *payload   = nullptr;

		DLRMessage( ObjectID_t ObjectID , uint16_t pri , const char *message );
		DLRMessage();
		~DLRMessage();
};

class DLRMessagePriorityComparison
{
	bool reverse;
	public:
		DLRMessagePriorityComparison( const bool& revparam );
		bool operator() ( DLRMessage * lhs , DLRMessage * rhs );
};

typedef std::priority_queue<DLRMessage *,std::vector<DLRMessage *>,DLRMessagePriorityComparison> DLRMessagesQueue;
extern DLRMessagesQueue MessagesQueue;


#endif //DLRMessage_H