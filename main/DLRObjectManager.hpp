#ifndef DLRObjectManager_HPP
#define DLRObjectManager_HPP

#include <stdint.h>
#include <vector>
#include <functional>

class DLRObjectManager: public DLRObject
{
	public:
		DLRObjectManager()
		{
			int a = 3;
			a++;
		}
		~DLRObjectManager()
		{}
		error_t status()
		{
			return( 0 );
		}
		uint16_t size()
		{
			return( DLRObjectStack.size() );
		}
		error_t main_setup()
		{
			for ( std::vector<DLRObject *>::iterator it=DLRObjectStack.begin(); it!=DLRObjectStack.end(); ++it )
			{
				yield();
				(*it)->setup();
			}
			return( 0 );
		}
		error_t main_loop()
		{
			for ( std::vector<DLRObject *>::iterator it=DLRObjectStack.begin(); it!=DLRObjectStack.end(); ++it )
			{
				yield();
				(*it)->loop();
			}
			return( 0 );
		}
		
} ObjectManager;

#endif //DLRObjectManager_HPP