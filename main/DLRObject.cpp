
#include "DLRObject.h"


ObjectID_t ObjectID_count = 0;


ICACHE_FLASH_ATTR DLRObject::DLRObject()
{
	ObjectID = ObjectID_count;
	ObjectID_count += 1;
}
ICACHE_FLASH_ATTR DLRObject::~DLRObject()
{
	//  DLRObjectStack[ObjectID] = nullptr;
	//	DLRObjectStack[ObjectID] = nullptr;
	if( name != nullptr )
	{
		delete name;
	}
}
error_t ICACHE_FLASH_ATTR DLRObject::setup()
{
	addLog( 0 , LOG_DEBUG , "Setup: prototype\n" );
	return( 0 );
}
error_t ICACHE_FLASH_ATTR DLRObject::loop()
{
	return( 0 );
}
error_t ICACHE_FLASH_ATTR DLRObject::status()
{
	return( 0 );
}
/*
virtual error_t DLRObject::message_handler( DLRMessage *message )
{
	return( 0 );
}
*/

ICACHE_FLASH_ATTR DLRObjectComparison::DLRObjectComparison( const bool& revparam = false )
{
	reverse = revparam;
}
bool ICACHE_FLASH_ATTR DLRObjectComparison::operator() ( DLRObject * lhs , DLRObject * rhs )
{
	if ( reverse )
	{
		if( lhs->priority  >  rhs->priority  )
		{
			return( true );
		}
		else if( lhs->priority == rhs->priority )
		{
			return( lhs->ObjectID > rhs->ObjectID );
		}
		return( false );
	}
	else
	{
		if( lhs->priority <  rhs->priority )
		{
			return( true );
		}
		else if( lhs->priority == rhs->priority )
		{
			return( lhs->ObjectID < rhs->ObjectID );
		}
		return( false );
	}
}


DLRObjectsQueue ObjectsQueue;

