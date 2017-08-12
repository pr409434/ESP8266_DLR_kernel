#ifndef DLRWifiManager_H
#define DLRWifiManager_H

#include "DLRObject.h"

class DLRWifiManager: public DLRObject
{
	public:
		DLRWifiManager();
		~DLRWifiManager();
		error_t loop();
		error_t setup();
		
		error_t reconnect_STA();
		
		error_t module_info();
		
	private:
		Countdown _timer_reconnect;
		Countdown _timer_module_info;
};

#endif //DLRWifiManager_H