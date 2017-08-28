#include "DLR_Sensors_Event.h"

DLRSensorsEventsDictionary CurrentSensorsEvents;

bool operator==(const DLR_sensors_event_t& lhs, const DLR_sensors_event_t& rhs)
{
	return (
				   ( LOG_PRI( lhs.priorities ) ==  LOG_PRI( rhs.priorities ) )
				&& ( lhs.timestamp == rhs.timestamp )
	);
}
bool operator!=(const DLR_sensors_event_t& lhs, const DLR_sensors_event_t& rhs)
{
	return (
				   ( LOG_PRI( lhs.priorities ) !=  LOG_PRI( rhs.priorities ) )
				|| ( lhs.timestamp != rhs.timestamp )
	);
}

bool operator>(const DLR_sensors_event_t& lhs, const DLR_sensors_event_t& rhs)
{

		if( LOG_PRI( lhs.priorities ) <  LOG_PRI( rhs.priorities ) )
		{
			return( true );
		}
		else if( LOG_PRI( lhs.priorities ) == LOG_PRI( rhs.priorities ) )
		{
			return( lhs.timestamp < rhs.timestamp );
		}
		return( false );
}
bool operator>=(const DLR_sensors_event_t& lhs, const DLR_sensors_event_t& rhs)
{

		if( LOG_PRI( lhs.priorities ) <  LOG_PRI( rhs.priorities ) )
		{
			return( true );
		}
		else if( LOG_PRI( lhs.priorities ) == LOG_PRI( rhs.priorities ) )
		{
			return( lhs.timestamp < rhs.timestamp );
		}
		return (
					   ( LOG_PRI( lhs.priorities ) ==  LOG_PRI( rhs.priorities ) )
					&& ( lhs.timestamp == rhs.timestamp )
		);
}

bool operator<(const DLR_sensors_event_t& lhs, const DLR_sensors_event_t& rhs)
{

		if( LOG_PRI( lhs.priorities ) >  LOG_PRI( rhs.priorities ) )
		{
			return( true );
		}
		else if( LOG_PRI( lhs.priorities ) == LOG_PRI( rhs.priorities ) )
		{
			return( lhs.timestamp > rhs.timestamp );
		}
		return( false );
}
bool operator<=(const DLR_sensors_event_t& lhs, const DLR_sensors_event_t& rhs)
{

		if( LOG_PRI( lhs.priorities ) >  LOG_PRI( rhs.priorities ) )
		{
			return( true );
		}
		else if( LOG_PRI( lhs.priorities ) == LOG_PRI( rhs.priorities ) )
		{
			return( lhs.timestamp > rhs.timestamp );
		}
		return (
					   ( LOG_PRI( lhs.priorities ) ==  LOG_PRI( rhs.priorities ) )
					&& ( lhs.timestamp == rhs.timestamp )
		);
}

bool MapSensorsEventsComp::operator() ( const String& lhs, const String& rhs ) const
{
	return( lhs < rhs );
}



/*
DLREventsManager::DLREventsManager()
{
	if( name == nullptr )
	{
		name = (char *) "EventsManager";
	}
	priority = 0;
}
DLREventsManager::~DLREventsManager()
{}
error_t DLREventsManager::status()
{
	return( 0 );
}
error_t DLREventsManager::setup()
{
	module_info();
	return( 0 );
}
error_t DLREventsManager::loop()
{
	return( 0 );
}
error_t DLREventsManager::module_info()
{
	_timer_module_info.countdown_ms( DLR_SENSORS_EVENT_MODULE_INFO_TIME );
	return( 0 );
}
*/
//		Countdown _timer_module_info;

