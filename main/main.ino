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



time_t   system_boot_time         = 0;
uint64_t MicroTimeStampTimeOffset = 0;
uint64_t LastMicroTimeStamp       = micros();

void ICACHE_FLASH_ATTR setup()
{
	Serial.begin( 115200 );
	Serial.println("\n\n/*********************************************/");
	Serial.println(ESP.getSketchMD5());
	Serial.println("/*********************************************/");			
	Serial.println(ESP.getResetInfo());
	Serial.println(ESP.getResetReason());
	Serial.println("/*********************************************/");

	ObjectsQueue.push_back( new DLRSystemWatch( &ObjectsQueue ) );
	ObjectsQueue.push_back( new DLRMessagesQueueManager() );
	ObjectsQueue.push_back( new DLRObjectManager() );
	ObjectsQueue.push_back( new DLRWifiManager() );
	ObjectsQueue.push_back( new DLRMQTT() );

	ObjectManager->main_setup();
	addLog( 0 , LOG_DEBUG , "Exit setup() -> ObjectQueue.size(): %ld\n" , ObjectsQueue.size() );
}


void ICACHE_FLASH_ATTR loop()
{
	ObjectManager->main_loop();
}

