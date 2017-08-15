#include "local/id_local.h"
/***********************************************
    Content: id_local.h
************************************************
const char* ssid        = "................";
const char* password    = "................";
const char* mqtt_server = "................";
***********************************************/


/*
wifi_station_set_hostname("xxxxx");
wifi_station_get_hostname();
*/
#include "Configuration.h"
#include "DLRObject.h"
#include "DLRMessages.h"
#include "SystemWatch.h"
#include "DLRObjectManager.h"
#include "DLRMessagesManager.h"
#include "DLRWifiManager.h"
#include "DLRMQTT.h"
#include "DLRSensor.h"
#include "DLRSensorDHT22.h"

#include <ArduinoOTA.h>

time_t   system_boot_time         = 0;
uint64_t MicroTimeStampTimeOffset = 0;
uint64_t LastMicroTimeStamp       = micros();


bool POWER_RELAY_VALUE = HIGH;


void ICACHE_FLASH_ATTR setup()
{

#ifdef POWER_RELAY_PIN
	pinMode( POWER_RELAY_PIN , OUTPUT );
	digitalWrite( POWER_RELAY_PIN , POWER_RELAY_VALUE );
#endif //POWER_RELAY_PIN
	
	Serial.begin( 115200 );
	delay(1000);
	Serial.println("\n\n/*********************************************/");
	Serial.println(ESP.getSketchMD5());
	Serial.println("/*********************************************/");			
	Serial.println(ESP.getResetInfo());
	Serial.println(ESP.getResetReason());
	Serial.println("/*********************************************/");

	delay(1000);
	ObjectsQueue.push_back( new DLRSystemWatch( &ObjectsQueue ) );
	delay(1000);
	ObjectsQueue.push_back( new DLRMessagesQueueManager() );
	delay(1000);
	ObjectsQueue.push_back( new DLRObjectManager() );
	delay(1000);
	ObjectsQueue.push_back( new DLRWifiManager() );
	delay(1000);
	ObjectsQueue.push_back( new DLRMQTT() );
	delay(1000);
#ifdef POWER_RELAY_PIN
	ObjectsQueue.push_back( new DLRSensor() );
#else //POWER_RELAY_PIN
	ObjectsQueue.push_back( new DLRSensorDHT22() );
#endif //POWER_RELAY_PIN
	delay(1000);
	ObjectManager->main_setup();
	addLog( 0 , LOG_DEBUG , "Exit setup() -> ObjectQueue.size(): %ld\n" , ObjectsQueue.size() );
	
	ArduinoOTA.setPort(8266);
	ArduinoOTA.setPassword("admin");
	ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
	ArduinoOTA.begin();
}


void ICACHE_FLASH_ATTR loop()
{
	ObjectManager->main_loop();
	ArduinoOTA.handle();
}

