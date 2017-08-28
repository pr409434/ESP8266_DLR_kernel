#ifndef DLR_SENSORS_EVENT_H
#define DLR_SENSORS_EVENT_H

#include "Configuration.h"
//#include "DLRObject.h"

/*
	Sensor types
*/
typedef enum DLR_sensors_type_t
{
	DLR_SENSOR_TYPE_UNKNOW 		= 0	,
	DLR_SENSOR_TYPE_STATE			,
	DLR_SENSOR_TYPE_VOLTAGE			,
	DLR_SENSOR_TYPE_CURRENT			,
	DLR_SENSOR_TYPE_LIGHT			,
	DLR_SENSOR_TYPE_PRESSURE		,
	DLR_SENSOR_TYPE_TEMPERATURE		,
	DLR_SENSOR_TYPE_HUMIDITY		,
	DLR_SENSOR_TYPE_RPM				,
	DLR_SENSOR_TYPE_PPM				,
	
	DLR_SENSOR_TYPE_SYSINFO_UPTIME				,
	DLR_SENSOR_TYPE_SYSINFO_LOOPSECOND			,
	DLR_SENSOR_TYPE_SYSINFO_FREEHEAP			,
	DLR_SENSOR_TYPE_SYSINFO_FREESKETCHSPACE		,
	DLR_SENSOR_TYPE_SYSINFO_CPUFREQMHZ			,

	DLR_SENSOR_TYPE_WIFI_RSSI							,
	DLR_SENSOR_TYPE_WIFI_CURRENT_WIFI_DISCONNECT_COUNT	,
	
	DLR_SENSOR_TYPE_EOF
} DLR_sensors_type_t;

/*
	Sensor event 
*/
typedef struct
{
	uint8_t		priorities;
    
    time_t		timestamp;
    time_t		timeperiod;
	
	DLR_sensors_type_t	type;
    union
    {
		bool			state;	
		uint32_t		counter;
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
		float			LoopSecond;
		uint32_t		FreeHeap;
		uint32_t		FreeSketchSpace;
		uint32_t		CpuFreqMHz;

		float			RSSI;

    };
} DLR_sensors_event_t;


/*  */
bool operator == (const DLR_sensors_event_t& lhs, const DLR_sensors_event_t& rhs);
bool operator != (const DLR_sensors_event_t& lhs, const DLR_sensors_event_t& rhs);
bool operator >  (const DLR_sensors_event_t& lhs, const DLR_sensors_event_t& rhs);
bool operator >= (const DLR_sensors_event_t& lhs, const DLR_sensors_event_t& rhs);
bool operator <  (const DLR_sensors_event_t& lhs, const DLR_sensors_event_t& rhs);
bool operator <= (const DLR_sensors_event_t& lhs, const DLR_sensors_event_t& rhs);

struct MapSensorsEventsComp
{
	bool operator() ( const String& lhs, const String& rhs ) const;
};

typedef std::map<String , std::map<String , DLR_sensors_event_t *, MapSensorsEventsComp> , MapSensorsEventsComp > DLRSensorsEventsDictionary;
extern DLRSensorsEventsDictionary CurrentSensorsEvents;

//typedef std::list<DLR_sensors_event_t> DLRSensorsEventsQueue;

/*
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
*/


#endif // DLR_SENSORS_EVENT_H