
#include "Dispatcher.h"

#include <math.h>
#include <event2/thread.h>

#include "DataStore.h"
#include "GAIDefines.h"
#include "URLConnection.h"

namespace GAI
{
    
    struct RequestCallbackStruct
    ///
    /// Struct used when making a request to manage failed dispatches
    ///
    {
        RequestCallbackStruct( Dispatcher* dispatcher, const Hit& hit);
        Dispatcher* dispatcher;
        Hit hit;
    };
    
    RequestCallbackStruct::RequestCallbackStruct( Dispatcher* dispatcher, const Hit& hit) :
    dispatcher( dispatcher ),
    hit( hit )
    {
    }
	
	Dispatcher::Dispatcher( DataStore& data_store, bool opt_out, double dispatch_interval ) :
	mbOptOut( opt_out ),
	mDispatchInterval( dispatch_interval ),
    mDataStore( data_store ),
    mDispatchEventBase(event_base_new()),
    mDispatchEvent(NULL),
    mbThreadRunning(true),
    mbCancelDispatch(false),
    mbImmediateDispatch(false),
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
        mURLConnection->createUserAgentString("GAI++","1.0");
        // begin the initial dispatch
        setDispatchInterval(mDispatchInterval);
        setUseHttps(false);
	}
	
	Dispatcher::~Dispatcher()
    ///
    /// Destructor
    ///
	{
        delete mURLConnection;
		mbThreadRunning = false;
        mbCancelDispatch = true;
        // ensure the thread has ended
        mTimerThread.join();
        
        // instruct the event loop to stop
        event_base_loopbreak( mDispatchEventBase );
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
        mbCancelDispatch = false;
        mbImmediateDispatch = true;
	}
	
	void Dispatcher::cancelDispatch()
    ///
    /// Cancel the current dispatch.
    ///
    /// @return
    ///     Nothing
    ///
	{
        mbCancelDispatch = true;
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
    void Dispatcher::setUseHttps(const bool aUseHttps)
    ///
    /// Set whether HTTPS will be used
    ///
    /// @param aUseHttps
    ///     Whether to use Https
    ///
    {
        this->mbUseHttps = aUseHttps;
        if( aUseHttps )
            mURLConnection->setAddress(kGAIURLHTTPS,kGAIPort);
        else
            mURLConnection->setAddress(kGAIURLHTTP,kGAIPort);
    }
    
    bool Dispatcher::isUseHttps()
    ///
    /// Retreive whether the tracker will use secure connection
    ///
    /// @return
    ///     Whether HTTPS will be used
    ///
    {
        return this->mbUseHttps;
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
        mbCancelDispatch = false;
        std::list<Hit> hits;
        hits = mDataStore.fetchHits(50, true);
        std::string base_url = std::string(kGAIURLPage) + "?";
        while( hits.size() > 0 && !mbCancelDispatch )
        {
            // for each hit
            for( std::list<Hit>::const_iterator it = hits.begin(), it_end = hits.end(); it != it_end; it++ )
            {
                RequestCallbackStruct* cb_struct = new RequestCallbackStruct(this,(*it));
                mURLConnection->request( base_url + (*it).getDispatchURL(), Dispatcher::RequestCallback, cb_struct );
            }
            // fetch the next group of hits
            hits = mDataStore.fetchHits(50, true);
        }
        // put back any left over hits
        if( hits.size() > 0 )
        {
            mDataStore.addHits( hits );
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
		
		
		
        while(dispatcher->mbThreadRunning)
        {
            if( dispatcher->mbImmediateDispatch )
            {
                dispatcher->dispatch();
            }
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
        // if the callback wasn't successful then we need to put the hit back into the datastore
        if( !success )
        {
            cb_struct->dispatcher->mDataStore.addHit(cb_struct->hit);
        }
     
        delete cb_struct;
    }
	
}
