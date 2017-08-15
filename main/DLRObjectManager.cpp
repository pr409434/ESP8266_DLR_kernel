
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

error_t ICACHE_FLASH_ATTR DLRObjectManager::setup()
{
	addLog( 0 , LOG_DEBUG , "Setup: DLRObjectManager\n" );
	module_info();
	return( 0 );
}

error_t ICACHE_FLASH_ATTR DLRObjectManager::loop()
{
	if( _timer_module_info.expired() )
	{
		module_info();
	}
	return( 0 );
};

error_t ICACHE_FLASH_ATTR DLRObjectManager::module_info()
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
	_timer_module_info.countdown( 60 );
	return( 0 );
}

DLRObjectManager *ObjectManager;

