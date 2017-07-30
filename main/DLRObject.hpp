#ifndef DLRObject_HPP
#define DLRObject_HPP

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