
#include "DLRObjectManager.h"


ICACHE_FLASH_ATTR DLRObjectManager::DLRObjectManager()
{
	if( name == nullptr )
	{
		name = (char *) "ObjectManager";
	}
	priority = 0;
	ObjectManager = this;
}
ICACHE_FLASH_ATTR DLRObjectManager::~DLRObjectManager()
{}
error_t ICACHE_FLASH_ATTR DLRObjectManager::status()
{
	return( 0 );
}
uint16_t ICACHE_FLASH_ATTR size()
{
	return( ObjectsQueue.size() );
}
error_t ICACHE_FLASH_ATTR DLRObjectManager::main_setup()
{
	for ( std::vector<DLRObject *>::iterator it=ObjectsQueue.begin(); it!=ObjectsQueue.end(); ++it )
	{
		yield();
		(*it)->setup();
	}
	return( 0 );
}
error_t ICACHE_FLASH_ATTR DLRObjectManager::main_loop()
{
	for ( std::vector<DLRObject *>::iterator it=ObjectsQueue.begin(); it!=ObjectsQueue.end(); ++it )
	{
		yield();

		uint64_t microtimestamp = microtimes();

		(*it)->loop();

		uint64_t elapsed_microtimes = microtimes() - microtimestamp;
	}
	return( 0 );
}
error_t ICACHE_FLASH_ATTR DLRObjectManager::ObjectList()
{
	String list( name );
	list += ".ObjectList:\n";
	for ( std::vector<DLRObject *>::iterator it=ObjectsQueue.begin(); it!=ObjectsQueue.end(); ++it )
	{
		list += "\t";
		list += (*it)->ObjectID;
		list += "\t";
		list += (*it)->name;
		list += "\n";
	}
	addLog( ObjectID , LOG_INFO , list.c_str() );
}
error_t ICACHE_FLASH_ATTR DLRObjectManager::setup()
{
	addLog( 0 , LOG_DEBUG , "Setup: DLRObjectManager\n" );
	return( 0 );
}

time_t _last_timestamp = 0;
error_t ICACHE_FLASH_ATTR DLRObjectManager::loop()
{
	time_t _timestamp = millis();
	if( _last_timestamp > _timestamp )
	{
		_last_timestamp = _timestamp;
	}
	if( ( _timestamp - _last_timestamp ) > 60000 )
	{
		_last_timestamp = _timestamp;
		ObjectList();
	}
	return( 0 );
};


DLRObjectManager *ObjectManager;

