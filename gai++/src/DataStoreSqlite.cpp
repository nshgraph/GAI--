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
    mbHasChanges(false),
    mDB(NULL)
    {
        
    }
    DataStoreSqlite::~DataStoreSqlite()
    {
        close();
    }
    // Functions for managing the datastore as a whole
    bool DataStoreSqlite::open()
    {
        int return_code = sqlite3_open(mPath.c_str(), &mDB);
        if( return_code != SQLITE_OK || !initializeDatabase())
        {
            close();
            return false;
        }
        return true;
    }
    bool DataStoreSqlite::save()
    {
        // all changes are now saved;
        mbHasChanges = false;
        return true;
    }
    void DataStoreSqlite::close()
    {
        if( mDB )
            sqlite3_close(mDB);
        mDB = NULL;
    }
    bool DataStoreSqlite::isOpen() const
    {
        return (mDB != NULL);
    }
    
    // Functions for managing the datastore state
    bool DataStoreSqlite::hasChanges() const
    {
        return mbHasChanges;
    }
    
    bool DataStoreSqlite::deleteAllEntities()
    {
        bool result = true;
        result &= deleteAllHits();
        result &= deleteAllProperties();
        return result;
    }
    int DataStoreSqlite::entityCount()
    {
        return hitCount() + propertyCount();
    }
    
    // Functions for managing hits in the datastore
    bool DataStoreSqlite::deleteAllHits()
    {
        return true;
    }
    bool DataStoreSqlite::addHit(const Hit& hit)
    {
        return true;
    }
    int DataStoreSqlite::hitCount()
    {
        return 0;
    }
    std::list<Hit> DataStoreSqlite::fetchHits(const unsigned int limit, bool removeFetchedFromDataStore)
    {
        std::list<Hit> hits;
        return hits;
    }
    
    // Functions for managing properties in the datastore
    bool DataStoreSqlite::deleteAllProperties()
    {
        int rc;
        rc = sqlite3_exec(mDB, "DELETE FROM properties", 0, 0, 0);
        if( rc != SQLITE_OK)
            return false;
        
        return true;
    }
    bool DataStoreSqlite::addProperty(const std::string& name, const std::string& value)
    {
        int rc;
        char *zSQL = sqlite3_mprintf("INSERT INTO properties(key,value) VALUES('%q','%q')", name.c_str(), value.c_str());
        rc = sqlite3_exec(mDB, zSQL, 0, 0, 0);
        sqlite3_free(zSQL);
        if( rc != SQLITE_OK)
            return false;
        
        return true;
    }
    std::string DataStoreSqlite::fetchProperty( const std::string& name )
    {
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
    {
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
    {
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
    
    // ensure that the database has all strutures necessary
    bool DataStoreSqlite::initializeDatabase()
    {
        int rc;
        sqlite3_stmt *statement = NULL;
        rc = sqlite3_prepare_v2(mDB, "CREATE TABLE properties (key TEXT PRIMARY KEY, value TEXT);", -1, &statement, 0);
        if( rc != SQLITE_OK)
            return true; // if there is an issue with this, it is probably because the table already exists
        if (sqlite3_step(statement) != SQLITE_DONE)
            return false;
        sqlite3_finalize(statement);
        return true;
    }
}