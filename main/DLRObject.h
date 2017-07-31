#ifndef DLRObject_H
#define DLRObject_H

#include "Configuration.h"


class DLRObject
{
	public:
		uint8_t     priority = 0xff;
		ObjectID_t  ObjectID;
		char       *name     = nullptr;
		
		DLRObject();
		~DLRObject();
		virtual error_t setup();
		virtual error_t loop();
		virtual error_t status();
		//virtual error_t message_handler( DLRMessage *message );
};

class DLRObjectComparison
{
	bool reverse;
	public:
		DLRObjectComparison( const bool& revparam );
		bool operator() ( DLRObject * , DLRObject * );
};

//typedef std::priority_queue<DLRObject *,std::vector<DLRObject *>,DLRObjectComparison> DLRObjectsQueue;
typedef std::vector<DLRObject *> DLRObjectsQueue;
extern DLRObjectsQueue ObjectsQueue;


#endif //DLRObject_H