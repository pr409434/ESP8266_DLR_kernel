#ifndef DLRSensor_H
#define DLRSensor_H

#include "DLRObject.h"
#include <OneWire.h>
#include <DS18B20.h>

class DLRSensor: public DLRObject
{
	public:
		DLRSensor();
		~DLRSensor();
		error_t loop();
		error_t setup();
	
		error_t module_info();
		
	private:
		Countdown _timer_module_info;
		uint32_t  module_info_counter;
		
		OneWire* oneWire;
		DS18B20* sensor;
		
		float temperature;

		uint64_t Last_FanCount_microtimestamp;
		float	 rpm;
		
		
};

#endif //DLRSensor_H