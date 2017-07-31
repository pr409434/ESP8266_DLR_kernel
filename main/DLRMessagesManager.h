#ifndef DLRMessagesManager_H
#define DLRMessagesManager_H

#include "DLRMessages.h"
#include "DLRObject.h"

class DLRMessagesQueueManager: public DLRObject
{
	char   *current_message     = nullptr;
	int     current_message_idx  = 0;
	int     current_message_size = 0;
	public:
		DLRMessagesQueueManager();
		~DLRMessagesQueueManager();
		error_t loop();
		error_t send_current_message();
};

extern DLRMessagesQueueManager *MessagesQueueManager;

#endif //DLRMessagesManager_H