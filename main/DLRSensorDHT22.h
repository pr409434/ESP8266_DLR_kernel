#ifndef DLRSensorDHT22_H
#define DLRSensorDHT22_H

#include "DLRObject.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


class DLRSensorDHT22: public DLRObject
{
	public:
		DLRSensorDHT22();
		~DLRSensorDHT22();
		error_t loop();
		error_t setup();
	
		error_t module_info();
		
	private:
		Countdown _timer_module_info;
		uint32_t module_info_counter;
		
		DHT_Unified* sensor;

		sensor_t temperature_sensor_info;
		sensors_event_t temperature_event;

		sensor_t humidity_sensor_info;
		sensors_event_t humidity_event;		

};

#endif //DLRSensorDHT22_H