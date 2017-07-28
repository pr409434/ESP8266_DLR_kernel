#ifndef DLRObject_HPP
#define DLRObject_HPP

#include <stdint.h>
#include <list>
#include <functional>


typedef uint8_t error_t;

class DLRObject;

std::list< DLRObject *  > DLRObjectStack;

class DLRObject
{
	public:
		DLRObject()
		{
			StackID = DLRObjectStack.size();
			DLRObjectStack.push_back( this );
		}
		~DLRObject()
		{
			size_t StackIdx = 0;
			for ( std::list<DLRObject *>::iterator it=DLRObjectStack.begin(); it!=DLRObjectStack.end(); ++it )
			{
				if (
						( *it == this )
						&&( StackIdx == StackID )
					)
				{
					*it = nullptr;
				}
				StackIdx += 1;
			}
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
			//DLRObjectStack.begin();
			return( 0 );
		}
	private:
		size_t StackID;
};




#endif //DLRObject_HPP