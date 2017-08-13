#include "DLRSensorDHT22.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


/*****************************************************************************
Type of sensor in use:
#define DHTTYPE           DHT11     // DHT 11 
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
#define DHTTYPE           DHT21     // DHT 21 (AM2301)
*****************************************************************************/
#define DHTPIN            D4        // GPIO2 Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT22     // DHT 22 (AM2302)




ICACHE_FLASH_ATTR DLRSensorDHT22::DLRSensorDHT22()
{
	if( name == nullptr )
	{
		name = (char *) "SensorDHT22";
	}
	priority = 0;
	module_info_counter = 0;
	sensor = new DHT_Unified( DHTPIN, DHTTYPE );
}
ICACHE_FLASH_ATTR DLRSensorDHT22::~DLRSensorDHT22()
{
	
}
error_t ICACHE_FLASH_ATTR DLRSensorDHT22::loop()
{
	if( _timer_module_info.expired() )
	{
		module_info();
	}
	return( 0 );
}
error_t ICACHE_FLASH_ATTR DLRSensorDHT22::setup()
{
	sensor->begin();
	sensor->temperature().getSensor( &temperature_sensor_info );
	sensor->humidity().getSensor( &humidity_sensor_info );
	_timer_module_info.countdown( 23 );

	return( 0 );
}
	
error_t ICACHE_FLASH_ATTR DLRSensorDHT22::module_info()
{
	uint32_t delay_ms   = 1 * 1000;
	int32_t  elapsed_ms = delay_ms - _timer_module_info.left_ms();
	time_t timestamp    = time( NULL );
	
	
	switch( module_info_counter % 3 )
	{
		case 0:
			{
				sensor->temperature().getEvent( &temperature_event );
				if ( std::isnan( temperature_event.temperature ) )
				{
					addLog( ObjectID , LOG_CRIT , "Module: %s Error reading temperature!\n" , name );
				} else
				{
					addLog( ObjectID , LOG_NOTICE ,
						"{\"timestamp\":%ld,\"timestamp_ms\":%ld,\"temperature\":%0.1f,\"units\":\"%s\"}\n"
								, timestamp
								, temperature_event.timestamp % 1000
								, temperature_event.temperature
								, "°C"
							);
				}
			}
			break;
		case 1:
			{
				sensor->humidity().getEvent( &humidity_event );
				if ( std::isnan( humidity_event.relative_humidity ) )
				{
					addLog( ObjectID , LOG_CRIT , "Module: %s Error reading humidity!\n" , name );
				} else
				{
					addLog( ObjectID , LOG_NOTICE ,
						"{\"timestamp\":%ld,\"timestamp_ms\":%ld,\"humidity\":%0.1f,\"units\":\"%s\"}\n"
								, timestamp
								, humidity_event.timestamp % 1000
								, humidity_event.relative_humidity
								, "%"
							);
				}
			}
			break;
		case 2:
			{
				mqtt_publish( LOG_NOTICE , "DHT22" ,
							"{\"timestamp\":%ld,\"temperature\":%0.1f,\"humidity\":%0.1f}"
									, timestamp
									, temperature_event.temperature
									, humidity_event.relative_humidity
									);
				
			}
			break;
		default:
			break;
	}

	module_info_counter +=1;
	_timer_module_info.countdown_ms( delay_ms );
	return( 0 );
}