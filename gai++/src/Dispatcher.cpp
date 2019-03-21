
#include "Dispatcher.h"

#ifdef WIN32
#include <windows.h>
#endif
#include <math.h>
#include <event2/thread.h>

#include "DataStore.h"
#include "DebugPrint.h"
#include "GAIDefines.h"
#include "Timestamp.h"
#include "URLBuilder.h"
#include "URLConnection.h"

namespace GAI
{
    
    struct RequestCallbackStruct
    ///
    /// Struct used when making a request to manage failed dispatches
    ///
    {
        RequestCallbackStruct( Dispatcher* dispatcher, const int id);
        Dispatcher* dispatcher;
        int hitId;
    };
    
    RequestCallbackStruct::RequestCallbackStruct( Dispatcher* dispatcher, const int id) :
    dispatcher( dispatcher ),
    hitId( id )
    {
    }
	
	Dispatcher::Dispatcher( DataStore& data_store,
						    const bool opt_out,
						    const double dispatch_interval,
						    const std::string& address,
						    const int port ) :
	mbOptOut( opt_out ),
	mDispatchInterval( dispatch_interval ),
    mDataStore( data_store ),
    mDispatchEventBase(event_base_new()),
    mDispatchEvent(NULL),
    mbThreadRunning(true),
	mbEvenLoopStarted(false),
    mbCancelDispatch(false),
    mbImmediateDispatch(false),
	mLastDispatchedHitId(0),
	mPendingRequests(0),
    mTimerThread( Dispatcher::TimerThreadFunction, (void*)this ),
    mURLConnection( NULL )
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
        mURLConnection = new URLConnection( mDispatchEventBase );
        mURLConnection->createUserAgentString( "GAI++", "1.0" );

        setDispatchInterval( mDispatchInterval );
		setAddress( address, port );
	}
	
	Dispatcher::~Dispatcher()
    ///
    /// Destructor
    ///
	{
		mbThreadRunning = false;
		mbCancelDispatch = true;
		event_base_loopbreak( mDispatchEventBase );
		mTimerThread.join();

		event_free( mDispatchEvent );
		event_base_free( mDispatchEventBase );
	}

	void Dispatcher::startEventLoop()
	///
	/// Starts the main event loop
	///
	/// @return Nothing
	///
	{
		mbEvenLoopStarted = true;
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
        mbCancelDispatch = false;
        mbImmediateDispatch = true;
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

	void Dispatcher::setAddress( const std::string& address, const int port )
	///
	/// Set the host address to be used for dispatched hits
	///
	/// @param address
	///     Host Address
	///
	/// @param port
	///     Host Port
	///
	{
		mURLConnection->setAddress( address, port );
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
    
    void Dispatcher::dispatch()
	///
	/// Perform the actual dispatch of any records in the datastore
    ///
    /// @return
    ///     Nothing
    ///
    {
		std::list<Hit> hits;
        hits = mDataStore.fetchHits( mLastDispatchedHitId, kDispatchBlockSize );

		while( hits.size() > 0 && !mbCancelDispatch )
        {
            // for each hit
            for( std::list<Hit>::const_iterator it = hits.begin(), it_end = hits.end(); it != it_end; it++ )
            {
				RequestCallbackStruct* cb_struct = new RequestCallbackStruct( this, it->getId() );
				mURLConnection->requestPOST( UrlBuilder::createPOSTURL(*it), UrlBuilder::createPOSTPayload(*it, Timestamp::generateTimestamp() ), Dispatcher::RequestCallback, cb_struct );
				mLastDispatchedHitId = it->getId();
            }

            // fetch the next group of hits
            hits = mDataStore.fetchHits( mLastDispatchedHitId, kDispatchBlockSize );
        }
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
        while( dispatcher->mbThreadRunning )
        {
            if( dispatcher->mbImmediateDispatch )
            {
                dispatcher->mbImmediateDispatch = false;
                dispatcher->dispatch();
            }

			if( dispatcher->mbEvenLoopStarted )
			{
                event_base_loop(dispatcher->mDispatchEventBase, EVLOOP_NONBLOCK);
			}
			
#ifdef WIN32
			Sleep( 2000 );
#else
			sleep( 2 );
#endif
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
		dispatcher->dispatch();
	}
    
    void Dispatcher::RequestCallback( bool success, void* param )
	///
	/// The callback after each dispatch interval has passed. The context will trigger
	/// a queueDispatch on the context (the Dispatcher).
	///
    /// @param success
    ///     Whether the dispatch was successful
	///
    /// @param param
    ///     Param passed with dispatch. Should be of type RequestCallbackStruct*
	///
    /// @return
    ///     Nothing
    ///
	{
		RequestCallbackStruct* cb_struct = (RequestCallbackStruct*)param;

        if( success )
		{
			cb_struct->dispatcher->mDataStore.deleteHit( cb_struct->hitId );
        }
        delete cb_struct;
    }
	
}
