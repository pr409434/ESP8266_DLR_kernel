#include "SystemWatch.h"

ICACHE_FLASH_ATTR DLRSystemWatch::DLRSystemWatch()
{
	if( name == nullptr )
	{
		name = (char *) "SystemWatch";
	}
	priority = 0;
	_WatchingObjectsQueue = nullptr;
	_last_microtimestamp  = microtimes();
	
	current_sysinfo_uptime.priorities          = LOG_ObjectID_PRI( ObjectID , LOG_INFO );
	current_sysinfo_loopsecond.priorities      = LOG_ObjectID_PRI( ObjectID , LOG_INFO );
	current_sysinfo_freeheap.priorities        = LOG_ObjectID_PRI( ObjectID , LOG_INFO );
	current_sysinfo_freesketchspace.priorities = LOG_ObjectID_PRI( ObjectID , LOG_INFO );
	current_sysinfo_cpufreqmhz.priorities      = LOG_ObjectID_PRI( ObjectID , LOG_INFO );

	current_sysinfo_uptime.type          = DLR_SENSOR_TYPE_SYSINFO_UPTIME;
	current_sysinfo_loopsecond.type      = DLR_SENSOR_TYPE_SYSINFO_LOOPSECOND;
	current_sysinfo_freeheap.type        = DLR_SENSOR_TYPE_SYSINFO_FREEHEAP;
	current_sysinfo_freesketchspace.type = DLR_SENSOR_TYPE_SYSINFO_FREESKETCHSPACE;
	current_sysinfo_cpufreqmhz.type      = DLR_SENSOR_TYPE_SYSINFO_CPUFREQMHZ;

	time_t timestamp   = time(NULL);	
	current_sysinfo_uptime.timestamp          = timestamp;
	current_sysinfo_loopsecond.timestamp      = timestamp;
	current_sysinfo_freeheap.timestamp        = timestamp;
	current_sysinfo_freesketchspace.timestamp = timestamp;
	current_sysinfo_cpufreqmhz.timestamp      = timestamp;

	current_sysinfo_uptime.uptime                   = timestamp - system_boot_time;
	current_sysinfo_loopsecond.LoopSecond           = 0;
	current_sysinfo_freeheap.FreeHeap               = ESP.getFreeHeap();
	current_sysinfo_freesketchspace.FreeSketchSpace = ESP.getFreeSketchSpace();
	current_sysinfo_cpufreqmhz.CpuFreqMHz           = ESP.getCpuFreqMHz();

	CurrentSensorsEvents["sysinfo"]["uptime"]          = &current_sysinfo_uptime;
	CurrentSensorsEvents["sysinfo"]["LoopSecond"]      = &current_sysinfo_loopsecond;
	CurrentSensorsEvents["sysinfo"]["FreeHeap"]        = &current_sysinfo_freeheap;
	CurrentSensorsEvents["sysinfo"]["FreeSketchSpace"] = &current_sysinfo_freesketchspace;
	CurrentSensorsEvents["sysinfo"]["CpuFreqMHz"]      = &current_sysinfo_cpufreqmhz;


	
}

ICACHE_FLASH_ATTR DLRSystemWatch::DLRSystemWatch( const DLRObjectsQueue* param_ptr_ObjectsQueue )
{
	if( name == nullptr )
	{
		name = (char *)  "SystemWatch";
	}
	priority = 0;
	_WatchingObjectsQueue = param_ptr_ObjectsQueue;
	_last_microtimestamp  = microtimes();
	addLog( ObjectID , LOG_ALERT ,
		"{\"ChipId\":\"%0xld\",\"ResetReason\":%s,\"ResetInfo\":%s,\"SketchMD5\":%s,\"SketchSize\":%ld\"FreeSketchSpace\":%ld\"getFreeHeap\":%ld}"
				, ESP.getChipId()
				, ESP.getResetReason().c_str()
				, ESP.getResetInfo().c_str()
				, ESP.getSketchMD5().c_str()
				, ESP.getSketchSize()
				, ESP.getFreeSketchSpace()
				, ESP.getFreeHeap()
				);
}
ICACHE_FLASH_ATTR DLRSystemWatch::~DLRSystemWatch()
{}

error_t ICACHE_FLASH_ATTR DLRSystemWatch::setup()
{
	module_info();
	return( 0 );
}

error_t ICACHE_FLASH_ATTR DLRSystemWatch::loop()
{
	_loop_count += 1;
	uint64_t elapsed_microtimes = microtimes() - _last_microtimestamp;
	_last_microtimestamp = microtimes();
	if( _timer_module_info.expired() )
	{
		module_info();
	}
	return( 0 );
}

error_t ICACHE_FLASH_ATTR DLRSystemWatch::module_info()
{
	uint32_t delay_ms  = DLR_SYSTEMWATCH_MODULE_INFO_TIME_MS;
	time_t timestamp   = time( NULL );
	int32_t elapsed_ms = delay_ms - _timer_module_info.left_ms();
	float LoopSecond   = (1000.0f*((float)( _loop_count - last_loop_count ))) / elapsed_ms ;
	last_loop_count    = _loop_count;

	current_sysinfo_uptime.timestamp          = timestamp;
	current_sysinfo_loopsecond.timestamp      = timestamp;
	current_sysinfo_freeheap.timestamp        = timestamp;
	current_sysinfo_freesketchspace.timestamp = timestamp;
	current_sysinfo_cpufreqmhz.timestamp      = timestamp;

	current_sysinfo_uptime.uptime                   = timestamp - system_boot_time;
	current_sysinfo_loopsecond.LoopSecond           = LoopSecond;
	current_sysinfo_freeheap.FreeHeap               = ESP.getFreeHeap();
	current_sysinfo_freesketchspace.FreeSketchSpace = ESP.getFreeSketchSpace();
	current_sysinfo_cpufreqmhz.CpuFreqMHz           = ESP.getCpuFreqMHz();

	
	addLog( ObjectID , LOG_NOTICE ,
			"{\"timestamp\":%ld,\"uptime\":%ld,\"LoopSecond\":%0.2f,\"FreeHeap\":%ld,\"FreeSketchSpace\":%ld,\"CpuFreqMHz\":%ld}\n"
				, timestamp
				, current_sysinfo_uptime.uptime
				, current_sysinfo_loopsecond.LoopSecond
				, current_sysinfo_freeheap.FreeHeap
				, current_sysinfo_freesketchspace.FreeSketchSpace
				, current_sysinfo_cpufreqmhz.CpuFreqMHz
				);
	mqtt_publish( LOG_INFO , "sysinfo" ,
		"{\"timestamp\":%ld,\"uptime\":%ld,\"LoopSecond\":%0.2f,\"FreeHeap\":%ld,\"FreeSketchSpace\":%ld,\"CpuFreqMHz\":%ld}"
			, timestamp
			, current_sysinfo_uptime.uptime
			, current_sysinfo_loopsecond.LoopSecond
			, current_sysinfo_freeheap.FreeHeap
			, current_sysinfo_freesketchspace.FreeSketchSpace
			, current_sysinfo_cpufreqmhz.CpuFreqMHz
			);
	_timer_module_info.countdown_ms( delay_ms );
	return( 0 );
}



/*
error_t DLRMQTT::sysinfo()
{
	_mqtt_client->publish( 
			String_Format( "/%s/sysinfo" , WiFi.hostname().c_str() ).c_str() ,
			String_Format( "{\n" \
				"getChipId:          %ld\n" \
				",getSdkVersion:      %s\n" \
				",getCoreVersion:     %s\n" \
				",getFreeHeap:        %ld\n" \
				",getCpuFreqMHz:      %ld\n" \
				",getSketchSize:      %ld\n" \
				",getFreeSketchSpace: %ld\n" \
				",getCycleCount:      %ld\n" \
				"\n}"
				, ESP.getChipId()
				, ESP.getSdkVersion()
				, ESP.getCoreVersion().c_str()
				, ESP.getFreeHeap()
				, ESP.getCpuFreqMHz()
				, ESP.getSketchSize()
				, ESP.getFreeSketchSpace()
				, ESP.getFreeSketchSpace()
				, ESP.getCycleCount()
			).c_str()
		);
}
*/


		

