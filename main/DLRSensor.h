#ifndef DLRSensor_H
#define DLRSensor_H

#include "DLR_Sensors_Event.h"
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
		
		DLR_sensors_event_t current_power_state;
		DLR_sensors_event_t current_heatsink_temperature;
		DLR_sensors_event_t current_fan_rpm;

		
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