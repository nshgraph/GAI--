
#include "Dispatcher.h"

#include <math.h>

#include "DataStore.h"
#include "GAI.h"

namespace GAI
{
	
	Dispatcher::Dispatcher( DataStore& data_store, bool opt_out, double dispatch_interval ) :
	mbOptOut( opt_out ),
	mDispatchInterval( dispatch_interval ),
    mDataStore( data_store ),
    mDispatchEventBase(event_base_new()),
    mDispatchEvent(NULL),
    mbThreadRunning(true),
    mTimerThread( Dispatcher::TimerThreadFunction, (void*)this )
    ///
    /// Constructor
    ///
    /// @param data_store
	///		DataStore object reference for dispatcher to store hits
    ///
    /// @param opt_out
	///		Whether Google Analytics tracking is enabled/disabled
    ///
    /// @param dispatch_interval
    ///     Time (in seconds) that each dispatch will occur
    ///
	{
        // begin the initial dispatch
        setDispatchInterval(mDispatchInterval);
	}
	
	Dispatcher::~Dispatcher()
    ///
    /// Destructor
    ///
	{
		mbThreadRunning = false;
        
        // instruct the event loop to stop
        event_base_loopbreak( mDispatchEventBase );
        // ensure the thread has ended
        mTimerThread.join();
		if( mDispatchEventBase )
		{
            // destroy event loop
			event_base_free( mDispatchEventBase );
		}
		
		mDataStore.close();
	}
    
    bool Dispatcher::storeHit( const Hit& hit )
    ///
    /// Stores a Hit in the DataStore
    ///
    /// @param hit
    ///     The Hit object to store
    ///
    /// @return
    ///     Whether the operation was successful
    ///
    {
		if( mbOptOut )
			return true;
		
		if( !mDataStore.isOpen() )
			mDataStore.open();
		
		return mDataStore.addHit( hit );
    }
	
	void Dispatcher::queueDispatch()
    ///
    /// Send all pending Hits in the DataStore
    ///
    /// @return
    ///     Nothing
    ///
	{
        printf("Dispatching\n");
	}
	
	void Dispatcher::cancelDispatch()
    ///
    /// Cancel the current dispatch.
    ///
    /// @return
    ///     Nothing
    ///
	{
        printf("Cancel Dispatch\n");
	}
	
	bool Dispatcher::isOptOut() const
    ///
    /// Return whether Google Analytics tracking is enabled/disabled
    ///
    /// @return
    ///     Tracking is enabled/disabled
    ///
	{
		return mbOptOut;
	}
	
	void Dispatcher::setOptOut( const bool opt_out )
    ///
    /// Set whether Google Analytics tracking is enabled/disabled.
	/// If opt_out is true, Hits will not be stored in the DataStore.
    ///
    /// @param opt_out
    ///     The enabled/disable boolean
    ///
    /// @return
    ///     Nothing
    ///
	{
		mbOptOut = opt_out;
	}
	
	int Dispatcher::getDispatchInterval() const
    ///
    /// Return the dispatch interval.
    ///
    /// @return
    ///     The dispatch interval
    ///
	{
		return mDispatchInterval;
	}
	
	void Dispatcher::setDispatchInterval( const double dispatch_interval )
    ///
    /// Set the dispatch interval. Stored Hits are attempted to be sent to
	/// Google Analytics each time this interval expires.
	///
    /// @param dispatch_interval
    ///     The dispatch interval in seconds
    ///
    /// @return
    ///     Nothing
    ///
	{
		mDispatchInterval = dispatch_interval;
		
		if( !mDispatchEvent )
		{
            mDispatchEvent = event_new( mDispatchEventBase, -1, EV_TIMEOUT|EV_PERSIST, Dispatcher::TimerCallback, this );
        }
        
        const double seconds = floor( mDispatchInterval );
        const double micro_seconds = ( mDispatchInterval - seconds ) * 1000000;
        const struct timeval timeout = {seconds, micro_seconds};
        event_add( mDispatchEvent, &timeout );
	}
	
    void Dispatcher::TimerThreadFunction( void* context )
	///
	/// Thread which enters an event loop that will trigger a callback after
	/// each dispatch interval.
	///
    /// @param context
    ///     A void* (the Dispatcher) which will be passed to the callback
	///
    /// @return
    ///     Nothing
    ///
	{
		Dispatcher *dispatcher = static_cast<Dispatcher*>( context );
		
		
		
        while(dispatcher->mbThreadRunning)
        {
            event_base_loop(dispatcher->mDispatchEventBase, EVLOOP_NONBLOCK);
        }
	}
	
	void Dispatcher::TimerCallback( evutil_socket_t file_descriptor, short events, void* context )
	///
	/// The callback after each dispatch interval has passed. The context will trigger
	/// a queueDispatch on the context (the Dispatcher).
	///
    /// @param file_descriptor
	///
    /// @param events
	///
    /// @param context
    ///     A void* (the Dispatcher) passed to the callback
	///
    /// @return
    ///     Nothing
    ///
	{
		Dispatcher *dispatcher = static_cast<Dispatcher*>( context );
		
		dispatcher->queueDispatch();
	}
	
}
