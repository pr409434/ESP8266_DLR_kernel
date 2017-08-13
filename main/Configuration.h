#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/*****************************************************************************
                    INCLUDE
*****************************************************************************/
#include <Arduino.h>

//#include "certificates.h"

#include <ESP8266WiFi.h>

// modify PubSubClient.h => #define MQTT_MAX_PACKET_SIZE 0xff
#include <PubSubClient.h>

#include "Countdown.h"
#include <time.h>
#include <stdint.h>
#include <stdarg.h>
#include <inttypes.h>
#include <math.h>
#include <WString.h>
#include <cstring>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <functional>

/*****************************************************************************
                    DEFINE
*****************************************************************************/
#define SYSLOG_NILVALUE "-"
// Syslog protocol format
#define SYSLOG_PROTO_IETF 0  // RFC 5424
#define SYSLOG_PROTO_BSD 1   // RFC 3164

/*
 * priorities/facilities are encoded into a single 32-bit quantity, where the
 * bottom 3 bits are the priority (0-7) and the top 28 bits are the facility
 * (0-big number).  Both the priorities and the facilities map roughly
 * one-to-one to strings in the syslogd(8) source code.  This mapping is
 * included in this file.
 *
 * priorities (these are ordered)
 */
#define LOG_EMERG 0 /* system is unusable */
#define LOG_ALERT 1 /* action must be taken immediately */
#define LOG_CRIT  2 /* critical conditions */
#define LOG_ERR   3 /* error conditions */
#define LOG_WARNING 4 /* warning conditions */
#define LOG_NOTICE  5 /* normal but significant condition */
#define LOG_INFO  6 /* informational */
#define LOG_DEBUG 7 /* debug-level messages */

#define LOG_PRIMASK 0x07  /* mask to extract priority part (internal) */
        /* extract priority */
#define LOG_PRI(p)  ((p) & LOG_PRIMASK)
#define LOG_MAKEPRI(fac, pri) (((fac) << 3) | (pri))

/* facility codes */
#define LOG_KERN  (0<<3)  /* kernel messages */
#define LOG_USER  (1<<3)  /* random user-level messages */
#define LOG_MAIL  (2<<3)  /* mail system */
#define LOG_DAEMON  (3<<3)  /* system daemons */
#define LOG_AUTH  (4<<3)  /* security/authorization messages */
#define LOG_SYSLOG  (5<<3)  /* messages generated internally by syslogd */
#define LOG_LPR   (6<<3)  /* line printer subsystem */
#define LOG_NEWS  (7<<3)  /* network news subsystem */
#define LOG_UUCP  (8<<3)  /* UUCP subsystem */
#define LOG_CRON  (9<<3)  /* clock daemon */
#define LOG_AUTHPRIV  (10<<3) /* security/authorization messages (private) */
#define LOG_FTP   (11<<3) /* ftp daemon */

/* other codes through 15 reserved for system use */
#define LOG_LOCAL0  (16<<3) /* reserved for local use */
#define LOG_LOCAL1  (17<<3) /* reserved for local use */
#define LOG_LOCAL2  (18<<3) /* reserved for local use */
#define LOG_LOCAL3  (19<<3) /* reserved for local use */
#define LOG_LOCAL4  (20<<3) /* reserved for local use */
#define LOG_LOCAL5  (21<<3) /* reserved for local use */
#define LOG_LOCAL6  (22<<3) /* reserved for local use */
#define LOG_LOCAL7  (23<<3) /* reserved for local use */

#define LOG_NFACILITIES 24  /* current number of facilities */
#define LOG_FACMASK 0x03f8  /* mask to extract facility part */
                            /* facility of pri */
#define LOG_FAC(p)  (((p) & LOG_FACMASK) >> 3)

#define LOG_MASK(pri)  (1 << (pri))	/* mask for one priority */
#define LOG_UPTO(pri)  ((1 << ((pri)+1)) - 1)	/* all priorities through pri */

#define LOG_ObjectID_PRI( fac , pri ) ((( fac + 31 ) << 3) | (pri))


/*****************************************************************************
                    TYPEDEF
*****************************************************************************/
typedef uint8_t  error_t;
typedef uint16_t ObjectID_t;


/*****************************************************************************/
extern const char* ssid;
extern const char* password;
/*****************************************************************************/
extern const char* mqtt_server;
extern const char* mqtt_username;
extern const char* mqtt_password;
/*****************************************************************************/
extern time_t    system_boot_time;
extern uint64_t  MicroTimeStampTimeOffset;
extern uint64_t  LastMicroTimeStamp;

extern uint64_t microtimes();
/*****************************************************************************/
extern String String_Format( const String fmt , ... );
extern error_t addLog( ObjectID_t ObjectID , uint16_t pri , const String fmt , ... );
extern error_t mqtt_publish( uint16_t pri , const char* topic , const String fmt , ... );

#endif // CONFIGURATION_H