#ifndef DLRObjectManager_HPP
#define DLRObjectManager_HPP

#include "DLRObject.h"

class DLRObjectManager: public DLRObject
{
	public:
		DLRObjectManager();
		~DLRObjectManager();
		error_t status();
		uint16_t size();
		error_t main_setup();
		error_t main_loop();
		
		error_t ObjectList();

		error_t setup();
		error_t loop();

};
extern DLRObjectManager *ObjectManager;

#endif //DLRObjectManager_HPP