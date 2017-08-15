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
	
	
	temperature = NAN;
	oneWire = new OneWire( ONE_WIRE_BUS_PIN );
	sensor  = new DS18B20( oneWire );
	
	rpm = NAN;
	FanInterruptCounter = 0;
	attachInterrupt( FAN_INTERRUPT_PIN , FanInterruptHandles , FALLING );
	Last_FanCount_microtimestamp = microtimes();
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
	uint32_t delay_ms     = 1 * 1000;
	int32_t  elapsed_ms   = delay_ms - _timer_module_info.left_ms();
	time_t   timestamp    = time( NULL );

	float FanTimeElapsed         = microtimes() - Last_FanCount_microtimestamp;
	Last_FanCount_microtimestamp = microtimes();
	float FanCounter     = FanInterruptCounter;
	FanInterruptCounter = 0;
	
	rpm = ( RPM_FACTOR * FanCounter ) / FanTimeElapsed;
	
	if ( sensor->isConversionComplete() )
	{
		temperature = sensor->getTempC();
	}
	mqtt_publish( LOG_NOTICE , name ,
		"{\"timestamp\":%ld,\"power\":%d,\"temperature\":%0.2f,\"rpm\":%0.8f}"
				, timestamp
				, ( POWER_RELAY_VALUE ? 0 : 1 )
				, temperature
				, rpm
			);
	
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

	module_info_counter +=1;
	_timer_module_info.countdown_ms( delay_ms );
	return( 0 );
}

