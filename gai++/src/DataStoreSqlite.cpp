//
//  DataStoreSqlite.cpp
//  gai++
//
//  Created by Nathan Holmberg on 8/03/13.
//  Copyright (c) 2013 nshgraph. All rights reserved.
//

#include "DataStoreSqlite.h"

#include <sqlite3.h>

namespace GAI
{
    
    DataStoreSqlite::DataStoreSqlite( const std::string& path ) :
    mPath(path),
    mDB(NULL)
    ///
    /// Construtor
    ///
    /// @param path
    ///     Path to either existing datastore or where new datastore will be created
    ///
    {
        
    }
    
    
    DataStoreSqlite& DataStoreSqlite::operator=( const DataStoreSqlite& other )
    ///
    /// Assign Operator. Will create a new database connection.
    ///
    /// @param other
    ///     The other data store to copy.
    ///
    {
        close();
        mPath = other.mPath;
        if( other.mDB )
            open();
        return *this;
    }
    
    
    DataStoreSqlite::~DataStoreSqlite()
    ///
    /// Destructor
    ///
    {
        close();
    }
    
    bool DataStoreSqlite::open()
    ///
    /// Open a connection to the DataStore, and we also perform an integrity check against the opened database
    /// using "PRAGMA integrity_check" command, if there is any error found then we attempt to remove the corrupt
    /// database and recreate a new one and we prevent GAI from using a corrupt database.
    ///
    /// @return
    ///     Whether the operation was successful
    ///
    {
        int return_code = sqlite3_open( mPath.c_str(), &mDB );
        
        // Attempt to recreate the database if it's corrupt
        if ( !databaseIntegrityCheck() )
        {
            // Remove the corrupt database
            if( remove( mPath.c_str() ) == 0 )
            {
                // Reopen a new connection to the DataStore after deleting
                return_code = sqlite3_open( mPath.c_str(), &mDB );
            }
            else
            {
                // Close the connection to the DataStore and return false if failed to remove the corrupt database
                close();
                return false;
            }
        }
        
        // Here we have to check the integrity again to make sure the recreated database is healthy to use
        if( return_code != SQLITE_OK || !databaseIntegrityCheck() || !initializeDatabase() )
        {
            close();
            return false;
        }
        return true;
    }
    bool DataStoreSqlite::save()
    ///
    /// Save the Datastore. After this operation the on disk datastore should represent any in-memory content
    ///
    /// @return
    ///     Whether the operation was successful
    ///
    {
        // nothing to do on save as we don't cache state: the db is always updated
        return true;
    }
    void DataStoreSqlite::close()
    ///
    /// Close a connection to the DataStore
    ///
    /// @return
    ///     Whether the operation was successful
    ///
    {
        if( mDB )
            sqlite3_close(mDB);
        mDB = NULL;
    }
    bool DataStoreSqlite::isOpen() const
    ///
    /// Query whether there is a connection open to the datastore
    ///
    /// @return
    ///     Whether a connection is open to the datastore
    ///
    {
        return (mDB != NULL);
    }
    
    // Functions for managing the datastore state
    bool DataStoreSqlite::hasChanges() const
    ///
    /// Query whether there is a difference in state between the on disk representation and the in memory
    ///
    /// @return
    ///     Whether there are outstanding changes to the Datastore
    ///
    {
        return false;
    }
    
    bool DataStoreSqlite::deleteAllEntities()
    ///
    /// Deletes all entities of any type in the datastore. After this operation the store should be empty
    ///
    /// @return
    ///     Whether the operation was successful
    ///
    {
        bool result = true;
        result &= deleteAllHits();
        result &= deleteAllProperties();
        return result;
    }
    int DataStoreSqlite::entityCount()
    ///
    /// Query how many entities of any type are in the datastore
    ///
    /// @return
    ///     Entity count
    ///
    {
        return hitCount() + propertyCount();
    }
    
    bool DataStoreSqlite::deleteAllHits()
    ///
    /// Delete all stored hits
    ///
    /// @return
    ///     Whether the operation was successful
    ///
    {
        tthread::lock_guard<tthread::mutex> lock(mDBMutex);
        int rc;
        rc = sqlite3_exec(mDB, "DELETE FROM hits", 0, 0, 0);
        if( rc != SQLITE_OK)
            return false;
        
        return true;
    }
    
    bool DataStoreSqlite::addHit(const Hit& hit)
    ///
    /// Add a Hit to the datstore
    ///
    /// @param hit
    ///     The Hit object to add
    ///
    /// @return
    ///     Whether the operation was successful
    ///
    {
        tthread::lock_guard<tthread::mutex> lock(mDBMutex);
        int rc;
        char *zSQL = sqlite3_mprintf("INSERT INTO hits(version,url,timestamp) VALUES('%q','%q',%f)",
									 hit.getGaiVersion().c_str(),
									 hit.getDispatchURL().c_str(),
									 static_cast<double>(hit.getTimestamp()));
        rc = sqlite3_exec(mDB, zSQL, 0, 0, 0);
        sqlite3_free(zSQL);
        if( rc != SQLITE_OK)
        {
            return false;
        }
        
        return true;
    }
    
    
    bool DataStoreSqlite::addHits(const std::list<Hit>& hits)
    ///
    /// Add a list of Hits to the datstore
    ///
    /// @param hits
    ///     The  list of Hit objects to add
    ///
    /// @return
    ///     Whether the operation was successful
    ///
    {
        bool success = true;
        for( std::list<Hit>::const_iterator it = hits.begin(), it_end = hits.end(); it != it_end; it++ )
            success &= addHit( *it );
        return success;
    }
    
    std::list<Hit> DataStoreSqlite::fetchHits(const unsigned int limit, bool removeFetchedFromDataStore)
    ///
    /// Retrieves hits from the datstore up to a limit, optionally removes them after retrieval (atomically)
    ///
    /// @param limit
    ///     Maximum number of hits to return
    /// @param removeFetchedFromDataStore
    ///     Option to delete Hits from datastore before returning them
    ///
    /// @return
    ///     A List of Hit objects corresponding to those retrieved from the datastore
    ///
    {
        tthread::lock_guard<tthread::mutex> lock(mDBMutex);
        std::list<Hit> hits;
        
        int rc;
        sqlite3_stmt* statement = NULL;
        char* zSQL = NULL;
        char* zSQL_delete = NULL;
        sqlite3_exec(mDB, "BEGIN", NULL, NULL, NULL);
        
        zSQL = sqlite3_mprintf("SELECT * FROM hits ORDER BY id LIMIT %i",limit);
        rc = sqlite3_prepare_v2(mDB, zSQL, -1, &statement, 0);
        
        // step through the returned hits
        while( sqlite3_step( statement ) == SQLITE_ROW)
        {
            char* version = (char *)sqlite3_column_text( statement, 1 );
            char* url = (char *)sqlite3_column_text( statement, 2 );
            uint64_t timestamp = static_cast<uint64_t>( sqlite3_column_double( statement, 3 ) );
            if( version && url )
            {
                hits.push_back(createHit(version, url, timestamp));
            }
        }
        
        // if appropriate, delete the hits
        if( removeFetchedFromDataStore && rc == SQLITE_OK )
        {
            zSQL_delete = sqlite3_mprintf("DELETE FROM hits WHERE id IN (SELECT id FROM hits ORDER BY id LIMIT %i)",limit);
            rc = sqlite3_exec(mDB, zSQL_delete, 0, 0, 0);
            sqlite3_free(zSQL_delete);
            
        }
        sqlite3_free(zSQL);
        sqlite3_finalize(statement);
        
        // either commit or rollback
        if( rc != SQLITE_OK)
        {
            sqlite3_exec(mDB, "ROLLBACK", NULL, NULL, NULL);
            hits.clear();
        }
        else
        {
            sqlite3_exec(mDB, "COMMIT", NULL, NULL, NULL);
        }
        
        
        return hits;
    }
    
    int DataStoreSqlite::hitCount()
    ///
    /// Returns a count of all Hits in the datastore
    ///
    /// @return
    ///     Hit Count
    ///
    {
        tthread::lock_guard<tthread::mutex> lock(mDBMutex);
        int rc;
        int rows = 0;
        sqlite3_stmt *statement = NULL;
        rc = sqlite3_prepare_v2(mDB, "SELECT COUNT(*) FROM hits;", -1, &statement, 0);
        if( rc != SQLITE_OK)
            return rows;
        
        rc = sqlite3_step(statement);
        if (rc == SQLITE_ROW ) {
            rows = sqlite3_column_int(statement, 0);
        }
        
        sqlite3_finalize(statement);
        
        return rows;
    }
    
    bool DataStoreSqlite::deleteAllProperties()
    ///
    /// Deletes all properties (key-value pairs) in the datastore
    ///
    /// @return
    ///  Whether the operation was successful
    ///
    {
        tthread::lock_guard<tthread::mutex> lock(mDBMutex);
        int rc;
        rc = sqlite3_exec(mDB, "DELETE FROM properties", 0, 0, 0);
        if( rc != SQLITE_OK)
            return false;
        
        return true;
    }
    
    bool DataStoreSqlite::addProperty(const std::string& name, const std::string& value)
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
    {
        tthread::lock_guard<tthread::mutex> lock(mDBMutex);
        int rc;
        sqlite3_exec(mDB, "BEGIN", NULL, NULL, NULL);
        char *zSQL = sqlite3_mprintf("INSERT INTO properties(key,value) VALUES('%q','%q') ", name.c_str(), value.c_str());
        rc = sqlite3_exec(mDB, zSQL, 0, 0, 0);
        sqlite3_free(zSQL);
        if( rc != SQLITE_OK)
        {
            zSQL = sqlite3_mprintf("UPDATE properties SET value='%q' WHERE key='%q' ", value.c_str(), name.c_str());
            rc = sqlite3_exec(mDB, zSQL, 0, 0, 0);
            sqlite3_free(zSQL);
        }
        if( rc != SQLITE_OK)
        {
            sqlite3_exec(mDB, "ROLLBACK", NULL, NULL, NULL);
            return false;
        }
        sqlite3_exec(mDB, "COMMIT", NULL, NULL, NULL);
        
        return true;
    }
    
    std::string DataStoreSqlite::fetchProperty( const std::string& name )
    ///
    /// Retrieves the value of a property specified
    ///
    /// @param name
    ///     Key Name
    ///
    /// @return
    ///     Value of property as retrieved. Or empty string if failed
    ///
    {
        tthread::lock_guard<tthread::mutex> lock(mDBMutex);
        int rc;
        sqlite3_stmt *statement = NULL;
        std::string result = "";
        char *zSQL = sqlite3_mprintf("SELECT * FROM properties WHERE key='%q'", name.c_str() );
        rc = sqlite3_prepare_v2(mDB, zSQL, -1, &statement, 0);
        sqlite3_free(zSQL);
        if( rc != SQLITE_OK)
            return result;
        
        // step through all properties
        sqlite3_step( statement );
        if( sqlite3_column_text( statement, 0 ) )
            result = (char *)sqlite3_column_text( statement, 1 );
        
        sqlite3_finalize(statement);
        return result;
    }
    
    DataStore::PropertyMap DataStoreSqlite::fetchProperties()
    ///
    /// Retrieves all properties in the data store
    ///
    /// @return
    ///     Key Value map
    ///
    {
        tthread::lock_guard<tthread::mutex> lock(mDBMutex);
        DataStore::PropertyMap  properties;
        int rc;
        sqlite3_stmt *statement = NULL;
        rc = sqlite3_prepare_v2(mDB, "SELECT * FROM properties;", -1, &statement, 0);
        if( rc != SQLITE_OK)
            return properties;
        // step through all properties
        sqlite3_step( statement );
        while( sqlite3_column_text( statement, 0 ) )
        {
            properties[(char *)sqlite3_column_text( statement, 0 )] = (char *)sqlite3_column_text( statement, 1 );
            sqlite3_step( statement );
        }
        sqlite3_finalize(statement);
        return properties;
    }
    
    
    int DataStoreSqlite::propertyCount()
    ///
    /// Returns a count of all Key Value pairs in the datastore
    ///
    /// @return
    ///  Property Count
    ///
    {
        tthread::lock_guard<tthread::mutex> lock(mDBMutex);
        int rc;
        int rows = 0;
        sqlite3_stmt *statement = NULL;
        rc = sqlite3_prepare_v2(mDB, "SELECT COUNT(*) FROM properties;", -1, &statement, 0);
        if( rc != SQLITE_OK)
            return rows;
        
        rc = sqlite3_step(statement);
        if (rc == SQLITE_ROW ) {
            rows = sqlite3_column_int(statement, 0);
        }
        
        sqlite3_finalize(statement);

        return rows;
    }
    
    bool DataStoreSqlite::initializeDatabase()
    ///
    /// Ensure that the database has all strutures necessary
    ///
    /// @return
    ///     Whether the operation was successful
    ///
    {
        tthread::lock_guard<tthread::mutex> lock(mDBMutex);
        int rc;
        rc = sqlite3_exec(mDB, "CREATE TABLE properties (key TEXT PRIMARY KEY, value TEXT);", NULL,NULL,NULL);
        // ignore rc, if there is an issue with this, it is probably because the table already exists
        rc = sqlite3_exec(mDB, "CREATE TABLE hits (id INTEGER PRIMARY KEY, version TEXT, url TEXT, timestamp REAL);", NULL,NULL,NULL);
        // ignore rc, if there is an issue with this, it is probably because the table already exists
        return true;
    }
    
    bool DataStoreSqlite::databaseIntegrityCheck()
    ///
    ///  Ensure that the database is healthy to use
    ///
    ///  @return
    ///     Whether the database we opened is healthy to use
    ///
    {
        bool passed_integrity_check = false;
        
        sqlite3_stmt *statement = NULL;
        
        // Perform an integrity check on the opened database
        sqlite3_prepare_v2( mDB, "PRAGMA integrity_check", -1, &statement, 0 );
        
        //
        //  Integrity check result (if 'PRAGMA integrity_check' finds no errors, a single row with the value 'ok' is returned):
        //  ╔═════════════════╗
        //  ║ integrity_check ║
        //  ╟─────────────────╢
        //  ║ result          ║
        //  ╚═════════════════╝
        //
        sqlite3_step( statement );
        if( sqlite3_column_text( statement, 0 ) )
        {
            char* result = (char *)sqlite3_column_text( statement, 0 );
            passed_integrity_check = strcmp( result, "ok" ) == 0;
        }
        
        // Destroy the statement
        sqlite3_finalize( statement );
        
        return passed_integrity_check;
    }
}
