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
		
};

#endif //DLRWifiManager_H