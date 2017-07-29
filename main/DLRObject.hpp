#ifndef DLRObject_HPP
#define DLRObject_HPP

#include <stdint.h>
#include <vector>
#include <functional>


typedef uint8_t error_t;
typedef size_t  ObjectID_t;


class DLRMessage;
class DLRObject;

std::vector< DLRObject *  > DLRObjectStack;

extern error_t log( ObjectID_t ObjectID , uint16_t pri , const __FlashStringHelper *message );
extern error_t log( ObjectID_t ObjectID , uint16_t pri , const String &message );
extern error_t log( ObjectID_t ObjectID , uint16_t pri , const char *message );

extern error_t vlogf(   ObjectID_t ObjectID , uint16_t pri, const char *fmt, va_list args) __attribute__((format(printf, 3, 0)));
extern error_t vlogf_P( ObjectID_t ObjectID , uint16_t pri, PGM_P fmt_P, va_list args) __attribute__((format(printf, 3, 0)));

extern error_t logf( ObjectID_t ObjectID , uint16_t pri, const char *fmt, ...) __attribute__((format(printf, 3, 4)));

extern error_t logf_P( ObjectID_t ObjectID , uint16_t pri, PGM_P fmt_P, ...) __attribute__((format(printf, 3, 4)));


class DLRObject
{
	public:
		DLRObject()
		{
			ObjectID = DLRObjectStack.size();
			DLRObjectStack.push_back( this );
		}
		~DLRObject()
		{
			DLRObjectStack[ObjectID] = nullptr;
		}
		virtual error_t setup()
		{
			return( 0 );
		}
		virtual error_t loop()
		{
			return( 0 );
		}
		virtual error_t status()
		{
			return( 0 );
		}
		virtual error_t message_handler( DLRMessage *message )
		{
			return( 0 );
		}
	private:
		ObjectID_t ObjectID;
};




#endif //DLRObject_HPP