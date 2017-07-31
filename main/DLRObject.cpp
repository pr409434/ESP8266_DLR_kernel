
#include "DLRObject.h"


ObjectID_t ObjectID_count = 0;


DLRObject::DLRObject()
{
	ObjectID = ObjectID_count;
	ObjectID_count += 1;
}
DLRObject::~DLRObject()
{
	//  DLRObjectStack[ObjectID] = nullptr;
	//	DLRObjectStack[ObjectID] = nullptr;
	if( name != nullptr )
	{
		delete name;
	}
}
error_t DLRObject::setup()
{
	addLog( 0 , LOG_DEBUG , "Setup: prototype\n" );
	return( 0 );
}
error_t DLRObject::loop()
{
	return( 0 );
}
error_t DLRObject::status()
{
	return( 0 );
}
/*
virtual error_t DLRObject::message_handler( DLRMessage *message )
{
	return( 0 );
}
*/

DLRObjectComparison::DLRObjectComparison( const bool& revparam = false )
{
	reverse = revparam;
}
bool DLRObjectComparison::operator() ( DLRObject * lhs , DLRObject * rhs )
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

