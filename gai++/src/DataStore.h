
#ifndef __DataStore_h__
#define __DataStore_h__

#include <string>
#include <list>
#include <map>
#include "Hit.h"

namespace GAI
{
    // A pure abstract class for dealing with permenant storage of data
	class DataStore
	{
    public:
        // Functions for managing the datastore as a whole
        virtual bool open() = 0;
        virtual bool save() = 0;
        virtual void close() = 0;
        
        // Functions for managing the datastore state
        virtual bool hasChanges() const = 0;
        
        virtual bool deleteAllEntities() = 0;
        virtual int entityCount() = 0;
        
        // Functions for managing hits in the datastore
        virtual bool deleteAllHits() = 0;
        virtual bool addHit(const Hit& hit) = 0;
        virtual int hitCount() = 0;
        virtual std::list<Hit> fetchHits(const unsigned int limit, bool removeFetchedFromDataStore) = 0;
        
        // Functions for managing properties in the datastore
        virtual bool deleteAllProperties() = 0;
        virtual bool addProperty(const std::string& name, const std::string& value) = 0;
        virtual std::string fetchProperty( const std::string& name ) = 0;
        virtual std::map<std::string,std::string> fetchProperties() = 0;
        virtual int propertyCount() = 0;
        
	};
}

#endif
