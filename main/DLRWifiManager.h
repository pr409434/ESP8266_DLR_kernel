#ifndef DLRWifiManager_H
#define DLRWifiManager_H

#include "Configuration.h"
#include "DLR_Sensors_Event.h"
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
		
		DLR_sensors_event_t current_wifi_rssi;
		DLR_sensors_event_t current_wifi_disconnect_count;
		
	private:
		Countdown _timer_reconnect;
		Countdown _timer_module_info;
};

#endif //DLRWifiManager_H