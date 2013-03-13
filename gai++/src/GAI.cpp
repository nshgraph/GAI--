#include <exception>
#include <string>
#include <map>


#include "GAI.h"
#include "Tracker.h"
#include "TrackerImpl.h"
#include "Dispatcher.h"

namespace GAI
{
    
    GAI* GAI::sharedInstance() {
        static GAI* sharedInstance = NULL;
        if( sharedInstance == NULL )
            sharedInstance = new GAI();
        return sharedInstance;
    }
    
    
    GAI::GAI()
    {
        
    }
    
    
    Tracker* GAI::createTracker(const std::string& aTrackingId) {
        // first attempt to retrive the tracker with the same id
        TrackerMap::const_iterator it = _trackers.find( aTrackingId );
        if( it != _trackers.end() )
            return it->second;
        // create a new tracker
        Tracker* new_tracker = new TrackerImpl(_dispatcher, aTrackingId, _productString, _version);
        _trackers[aTrackingId] = new_tracker;
        
        return new_tracker;
    }
    
    void GAI::dispatch() {
        _dispatcher->queueDispatch();
    }
    
    void GAI::removeTracker(const std::string& aTrackerId) {
        TrackerMap::iterator it = _trackers.find( aTrackerId );
        if( it != _trackers.end() )
            _trackers.erase(it);
    }
    
    std::string GAI::getProductString() const{
        return this->_productString;
    }
    
    std::string GAI::getVersion() const{
        return this->_version;
    }
    
    void GAI::setProductString(const std::string& productString)
    {
        this->_productString = productString;
    }
    
    void GAI::setVersion(const std::string& version)
    {
        this->_version = version;
    }
    
    Tracker* GAI::getDefaultTracker() const{
        return this->_defaultTracker;
    }
    
    void GAI::setDebug(bool aDebug) {
        this->_debug = aDebug;
    }
    
    bool GAI::isDebug() const{
        return this->_debug;
    }
    
    void GAI::setOptOut(const bool aOptOut) {
        this->_optOut = aOptOut;
    }
    
    bool GAI::isOptOut() const{
        return this->_optOut;
    }
    
    void GAI::setDispatchInterval(const double aDispatchInterval) {
        this->_dispatchInterval = aDispatchInterval;
    }
    
    double GAI::getDispatchInterval() const{
        return this->_dispatchInterval;
    }
    
}

