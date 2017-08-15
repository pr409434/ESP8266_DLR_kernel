#ifndef DLRObjectManager_HPP
#define DLRObjectManager_HPP

#include "DLRObject.h"

class DLRObjectManager: public DLRObject
{
	public:
		DLRObjectManager();
		~DLRObjectManager();
		error_t status();
		error_t setup();
		error_t loop();
		error_t module_info();
		
		error_t main_setup();
		error_t main_loop();

	private:
		Countdown _timer_module_info;

};
extern DLRObjectManager *ObjectManager;

#endif //DLRObjectManager_HPP