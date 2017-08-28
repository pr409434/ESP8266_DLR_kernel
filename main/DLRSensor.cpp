#include "DLRSensor.h"


//#define POWER_RELAY_PIN   D1 => configure
#define ONE_WIRE_BUS_PIN  D4
#define FAN_INTERRUPT_PIN D3

#define RPM_FACTOR 0.5 * 60.0 * 1000000.0

static uint32_t FanInterruptCounter;
static void FanInterruptHandles()
{
	FanInterruptCounter++;
}
bool POWER_RELAY_VALUE = HIGH;

ICACHE_FLASH_ATTR DLRSensor::DLRSensor()
{
	if( name == nullptr )
	{
		name = (char *) "Sensor";
	}
	priority = 0;
	module_info_counter = 0;
	pinMode( POWER_RELAY_PIN , OUTPUT );
	digitalWrite( POWER_RELAY_PIN , POWER_RELAY_VALUE);
	
	current_power_state.priorities  = LOG_ObjectID_PRI( ObjectID , LOG_NOTICE );
	current_power_state.type        = DLR_SENSOR_TYPE_STATE;
	current_power_state.timestamp   = time(NULL);
	current_power_state.state       = POWER_RELAY_VALUE;
	CurrentSensorsEvents["power"]["state"] = &current_power_state;
	
	temperature = NAN;
	oneWire = new OneWire( ONE_WIRE_BUS_PIN );
	sensor  = new DS18B20( oneWire );
	current_heatsink_temperature.priorities  = LOG_ObjectID_PRI( ObjectID , LOG_NOTICE );
	current_heatsink_temperature.type        = DLR_SENSOR_TYPE_TEMPERATURE;
	current_heatsink_temperature.timestamp   = time(NULL);
	current_heatsink_temperature.temperature = NAN;
	CurrentSensorsEvents["heatsink"]["temperature"] = &current_heatsink_temperature;
	
	
	rpm = NAN;
	FanInterruptCounter = 0;
	attachInterrupt( FAN_INTERRUPT_PIN , FanInterruptHandles , FALLING );
	Last_FanCount_microtimestamp = microtimes();
	
	current_fan_rpm.priorities  = LOG_ObjectID_PRI( ObjectID , LOG_NOTICE );
	current_fan_rpm.type        = DLR_SENSOR_TYPE_RPM;
	current_fan_rpm.timestamp   = time(NULL);
	current_fan_rpm.rpm         = 0;
	CurrentSensorsEvents["fan"]["rpm"] = &current_fan_rpm;

}

ICACHE_FLASH_ATTR DLRSensor::~DLRSensor()
{
	
}
error_t ICACHE_FLASH_ATTR DLRSensor::loop()
{
	digitalWrite( POWER_RELAY_PIN , POWER_RELAY_VALUE );
	if( _timer_module_info.expired() )
	{
		module_info();
	}
	return( 0 );
}
error_t ICACHE_FLASH_ATTR DLRSensor::setup()
{
	sensor->begin();
	sensor->setResolution(12);
	sensor->requestTemperatures();

	module_info(); //_timer_module_info.countdown( 23 );
	return( 0 );
}
	
error_t ICACHE_FLASH_ATTR DLRSensor::module_info()
{
	uint32_t delay_ms     = DLR_SENSORS_EVENT_MODULE_INFO_TIME_ms;
	int32_t  elapsed_ms   = delay_ms - _timer_module_info.left_ms();
	time_t   timestamp    = time( NULL );

	float FanTimeElapsed         = microtimes() - Last_FanCount_microtimestamp;
	Last_FanCount_microtimestamp = microtimes();
	float FanCounter     = FanInterruptCounter;
	FanInterruptCounter = 0;
	
	rpm = ( RPM_FACTOR * FanCounter ) / FanTimeElapsed;
	current_fan_rpm.timestamp   = timestamp;
	current_fan_rpm.rpm         = roundf(rpm);
	
	if ( sensor->isConversionComplete() )
	{
		temperature = sensor->getTempC();
		current_heatsink_temperature.timestamp   = timestamp;
		current_heatsink_temperature.temperature = temperature;
	}
	
	
	/*
	if( std::isnan(temperature ) )
	{
		addLog( ObjectID , LOG_CRIT , "Module: %s Error reading temperature!\n" , name );
	} else
	{
	}
	temperature = NAN;
	*/
	
	sensor->setResolution(12);
	sensor->requestTemperatures();

	digitalWrite( POWER_RELAY_PIN , POWER_RELAY_VALUE );
	current_power_state.timestamp   = timestamp;
	current_power_state.state       = POWER_RELAY_VALUE;

		mqtt_publish( LOG_NOTICE , name ,
		"{\"timestamp\":%ld,\"power\":%d,\"temperature\":%0.2f,\"rpm\":%ld}"
				, timestamp
				, ( current_power_state.state ? 0 : 1 )
				, current_heatsink_temperature.temperature
				, current_fan_rpm.rpm
			);

	
	module_info_counter +=1;
	_timer_module_info.countdown_ms( delay_ms );
	return( 0 );
}

