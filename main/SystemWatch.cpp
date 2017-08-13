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
	uint32_t delay_ms = 10 * 1000;
	time_t timestamp = time( NULL );
	int32_t elapsed_ms = delay_ms - _timer_module_info.left_ms();
	float LoopSecond = (1000.0f*((float)( _loop_count - last_loop_count ))) / elapsed_ms ;
	last_loop_count  = _loop_count;
	
	addLog( ObjectID , LOG_NOTICE ,
			"{\"timestamp\":%ld,\"uptime\":%ld,\"LoopSecond\":%0.2f,\"FreeHeap\":%ld,\"FreeSketchSpace\":%ld,\"CpuFreqMHz\":%d}\n"
				, timestamp
				, timestamp - system_boot_time
				, LoopSecond
				, ESP.getFreeHeap()
				, ESP.getFreeSketchSpace()
				, ESP.getCpuFreqMHz()
				);
	mqtt_publish( LOG_NOTICE , "sysinfo" ,
			"{\"timestamp\":%ld,\"uptime\":%ld,\"LoopSecond\":%0.2f,\"FreeHeap\":%ld,\"FreeSketchSpace\":%ld,\"CpuFreqMHz\":%d}"
				, timestamp
				, timestamp - system_boot_time
				, LoopSecond
				, ESP.getFreeHeap()
				, ESP.getFreeSketchSpace()
				, ESP.getCpuFreqMHz()
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


		

