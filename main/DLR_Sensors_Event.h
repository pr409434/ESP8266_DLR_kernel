#ifndef DLR_SENSORS_EVENT_H
#define DLR_SENSORS_EVENT_H

#include "Configuration.h"
#include "DLRObject.h"

/*
	Sensor types
*/
typedef enum
{
	SENSOR_TYPE_UNKNOW 		= 0	,
	SENSOR_TYPE_VOLTAGE			,
	SENSOR_TYPE_CURRENT			,
	SENSOR_TYPE_LIGHT			,
	SENSOR_TYPE_PRESSURE		,
	SENSOR_TYPE_TEMPERATURE		,
	SENSOR_TYPE_HUMIDITY		,
	SENSOR_TYPE_RPM				,
	SENSOR_TYPE_PPM				,
	
	SENSOR_TYPE_SYSINFO_UPTIME				,
	SENSOR_TYPE_SYSINFO_LOOPSECOND			,
	SENSOR_TYPE_SYSINFO_FREEHEAP			,
	SENSOR_TYPE_SYSINFO_FREESKETCHSPACE		,
	SENSOR_TYPE_SYSINFO_CPUFREQMHZ			,

	SENSOR_TYPE_WIFI_RSSI		,
	
	SENSOR_TYPE_EOF
} sensors_type_t;

/*
	Sensor event 
*/
typedef struct
{
	uint8_t		priorities;
    uint16_t	type;
    uint32_t	timestamp;
    uint32_t	timeperiod;
    union
    {
        float           voltage;
        float           current;
        float           light;
        float           pressure;
        float           temperature;
        float           humidity;
		uint32_t		rpm;
		uint32_t		ppm;

		time_t			uptime;
		time_t			system_boot_time;
		uint32_t		LoopSecond;
		uint32_t		FreeHeap;
		uint32_t		FreeSketchSpace;
		uint32_t		CpuFreqMHz;

		float			RSSI;

    };
} sensors_event_t;


/*  */
bool operator == (const sensors_event_t& lhs, const sensors_event_t& rhs);
bool operator != (const sensors_event_t& lhs, const sensors_event_t& rhs);
bool operator >  (const sensors_event_t& lhs, const sensors_event_t& rhs);
bool operator >= (const sensors_event_t& lhs, const sensors_event_t& rhs);
bool operator <  (const sensors_event_t& lhs, const sensors_event_t& rhs);
bool operator <= (const sensors_event_t& lhs, const sensors_event_t& rhs);

typedef std::list<sensors_event_t> DLRSensorsEventsQueue;


class DLREventsManager: public DLRObject
{
	public:
		DLREventsManager();
		~DLREventsManager();
		error_t status();
		error_t setup();
		error_t loop();
		error_t module_info();

	private:
		Countdown _timer_module_info;
		DLRSensorsEventsQueue* SensorsEventsQueue;

};
extern DLREventsManager *EventsManager;



#endif // DLR_SENSORS_EVENT_H