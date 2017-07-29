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
		bool operator() ( DLRMessage *lhs , DLRMessage *rhs ) const
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

typedef std::priority_queue<DLRMessage,std::vector<DLRMessage>,DLRMessagePriorityComparison> DLRMessagesQueue;

class DLRMessagesQueueManager: public DLRObject
{
	public:
		DLRMessagesQueueManager()
		{}
		~DLRMessagesQueueManager()
		{}
} DLRMessagesQueueManager;


#endif //DLRMessagesQueue_HPP