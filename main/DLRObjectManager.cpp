
#include "DLRObjectManager.h"


DLRObjectManager::DLRObjectManager()
{
	if( name == nullptr )
	{
		name = (char *) "ObjectManager";
	}
	priority = 0;
}
DLRObjectManager::~DLRObjectManager()
{}
error_t DLRObjectManager::status()
{
	return( 0 );
}
uint16_t size()
{
	return( ObjectsQueue.size() );
}
error_t DLRObjectManager::main_setup()
{
	for ( std::vector<DLRObject *>::iterator it=ObjectsQueue.begin(); it!=ObjectsQueue.end(); ++it )
	{
		yield();
		(*it)->setup();
	}
	return( 0 );
}
error_t DLRObjectManager::main_loop()
{
	for ( std::vector<DLRObject *>::iterator it=ObjectsQueue.begin(); it!=ObjectsQueue.end(); ++it )
	{
		yield();
		(*it)->loop();
	}
	return( 0 );
}
error_t DLRObjectManager::ObjectList()
{
	String list( name );
	list += ".ObjectList:\n";
	for ( std::vector<DLRObject *>::iterator it=ObjectsQueue.begin(); it!=ObjectsQueue.end(); ++it )
	{
		list += "\t";
		list += (*it)->name;
		list += "\n";
	}
	//addLog( ObjectID , LOG_INFO , list.c_str() );
}

DLRObjectManager ObjectManager;