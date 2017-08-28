#ifndef DLRSystemWatch_H
#define DLRSystemWatch_H

#include "Configuration.h"
#include "DLR_Sensors_Event.h"
#include "DLRObject.h"

class DLRSystemWatch: public DLRObject
{
	public:
		DLRSystemWatch();
		DLRSystemWatch( const DLRObjectsQueue* param_ptr_ObjectsQueue );
		~DLRSystemWatch();
		error_t setup();
		error_t loop();
		error_t module_info();
		
		DLR_sensors_event_t current_sysinfo_uptime;
		DLR_sensors_event_t current_sysinfo_loopsecond;
		DLR_sensors_event_t current_sysinfo_freeheap;
		DLR_sensors_event_t current_sysinfo_freesketchspace;
		DLR_sensors_event_t current_sysinfo_cpufreqmhz;
		
	private:
		const DLRObjectsQueue* _WatchingObjectsQueue;
		uint64_t _last_microtimestamp = 0;
		uint64_t _loop_count = 0;
		uint64_t last_loop_count = 0;
		Countdown _timer_module_info;
		String _bootinfo;
};


#endif //DLRSystemWatch_H