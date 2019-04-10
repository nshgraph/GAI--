
#ifndef __DataStore_h__
#define __DataStore_h__

#include <string>
#include <list>
#include <map>
#include "Hit.h"

namespace GAI
{
    // A abstract class for dealing with permenant storage of data
	class DataStore
	{
    public:
        ///
        /// Type definition for a key value map
        typedef std::map<std::string,std::string> PropertyMap;
    public:
        virtual ~DataStore(){}
        // Functions for managing the datastore as a whole
        
        ///
        /// Open a connection to the DataStore
        ///
        /// @return
        ///     Whether the operation was successful
        ///
        virtual bool open() = 0;
        
        ///
        /// Save the Datastore. After this operation the on disk datastore should represent any in-memory content
        ///
        /// @return
        ///     Whether the operation was successful
        ///
        virtual bool save() = 0;
        
        ///
        /// Close a connection to the DataStore
        ///
        /// @return
        ///     Whether the operation was successful
        ///
        virtual void close() = 0;
        
        // Functions for managing the datastore state

        ///
        /// Query whether there is a difference in state between the on disk representation and the in memory
        ///
        /// @return
        ///     Whether there are outstanding changes to the Datastore
        ///
        virtual bool hasChanges() const = 0;
        
        
        ///
        /// Query whether there is a connection open to the datastore
        ///
        /// @return
        ///     Whether a connection is open to the datastore
        ///
        virtual bool isOpen() const = 0;
        
        ///
        /// Deletes all entities of any type in the datastore. After this operation the store should be empty
        ///
        /// @return
        ///     Whether the operation was successful
        ///
        virtual bool deleteAllEntities() = 0;
        
        ///
        /// Query how many entities of any type are in the datastore
        ///
        /// @return
        ///     Entity count
        ///
        virtual int entityCount() = 0;
        
        // Functions for managing hits in the datastore

		///
		/// Delete a hit with the givin id
		///
		/// @return
		///     Whether the operation was successful
		///
		virtual bool deleteHit(const int id) = 0;

        ///
        /// Delete all stored hits
        ///
        /// @return
        ///     Whether the operation was successful
        ///
        virtual bool deleteAllHits() = 0;
        
        ///
        /// Add a Hit to the datstore
        ///
        /// @param hit
        ///     The Hit object to add
        ///
        /// @return
        ///     Whether the operation was successful
        ///
        virtual bool addHit(const Hit& hit) = 0;
        
        ///
        /// Add a list of Hits to the datstore
        ///
        /// @param hits
        ///     The  list of Hit objects to add
        ///
        /// @return
        ///     Whether the operation was successful
        ///
        virtual bool addHits(const std::list<Hit>& hits) = 0;
        
        ///
        /// Returns a count of all Hits in the datastore
        ///
        /// @return
        ///     Hit Count
        ///
        virtual int hitCount() = 0;
        
        ///
        /// Retrieves hits from the datstore from a given timestamp up to a limit
		///
		/// @param from_timestamp
		///     The hit timestamp to begin the fetch from
		///
		/// @param limit
		///     The maximum number of hits to return
		///
        /// @return
        ///     A List of Hit objects corresponding to those retrieved from the datastore
        ///
        virtual std::list<Hit> fetchHits(const uint64_t from_timestamp, const unsigned int limit) = 0;
        
        // Functions for managing properties in the datastore
        
        ///
        /// Deletes all properties (key-value pairs) in the datastore
        ///
        /// @return
        ///  Whether the operation was successful
        ///
        virtual bool deleteAllProperties() = 0;
        
        ///
        /// Adds a property (key-value pair) to the datastore
        ///
        /// @param name
        ///     Key name
        /// @param value
        ///     Value to store
        ///
        /// @return
        ///     Whether the operation was successful
        ///
        virtual bool addProperty(const std::string& name, const std::string& value) = 0;
        
        ///
        /// Retrieves the value of a property specified
        ///
        /// @param name
        ///     Key Name
        ///
        /// @return
        ///     Value of property as retrieved. Or empty string if failed
        ///
        virtual std::string fetchProperty( const std::string& name ) = 0;
        
        ///
        /// Retrieves all properties in the data store
        ///
        /// @return
        ///     Key Value map
        ///
        virtual PropertyMap fetchProperties() = 0;
        
        ///
        /// Returns a count of all Key Value pairs in the datastore
        ///
        /// @return
        ///  Property Count
        ///
        virtual int propertyCount() = 0;
    protected:
        
        ///
        /// This internal function can be used by subclasses to generate a hit from a given set of values
        /// This is needed to overcome the protected constructor of the Hit class
		///
		/// @param id
		///     The id of this hit
		/// @param version
		///     GAI Protocol Version
        /// @param url
        ///     Pre-encoded URL of values associated with the hit
        /// @param timestamp
        ///     Timestamp for this hit
        ///
        /// @return
        ///     Generated Hit instance
        ///
        Hit createHit(const int id, const std::string& version, const std::string& url, const uint64_t timestamp)
        {
            return Hit(id, version, url, timestamp);
        }
        
	};
}

#endif
