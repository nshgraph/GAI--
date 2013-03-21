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
        // nothing to do on save as we don't cache state: the db is always updated
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
        return false;
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
        int rc;
        rc = sqlite3_exec(mDB, "DELETE FROM hits", 0, 0, 0);
        if( rc != SQLITE_OK)
            return false;
        
        return true;
    }
    bool DataStoreSqlite::addHit(const Hit& hit)
    {
        int rc;
        char *zSQL = sqlite3_mprintf("INSERT INTO hits(version,url,timestamp) VALUES('%q','%q',%f) ", hit.getGaiVersion().c_str(), hit.getDispatchURL().c_str(),hit.getTimestamp());
        rc = sqlite3_exec(mDB, zSQL, 0, 0, 0);
        sqlite3_free(zSQL);
        if( rc != SQLITE_OK)
        {
            return false;
        }
        
        return true;
    }
    std::list<Hit> DataStoreSqlite::fetchHits(const unsigned int limit, bool removeFetchedFromDataStore)
    {
        std::list<Hit> hits;
        
        int rc;
        sqlite3_stmt *statement = NULL;
        char *zSQL = NULL;
        char* zSQL_delete = NULL;
        sqlite3_exec(mDB, "BEGIN", NULL, NULL, NULL);
        
        zSQL = sqlite3_mprintf("SELECT * FROM hits ORDER BY id LIMIT %i",limit);
        rc = sqlite3_prepare_v2(mDB, zSQL, -1, &statement, 0);
        
        // step through the returned hits
        while( sqlite3_step( statement ) == SQLITE_ROW)
        {
            char* version = (char *)sqlite3_column_text( statement, 1 );
            char* url = (char *)sqlite3_column_text( statement, 2 );
            double timestamp = sqlite3_column_double( statement, 3 );
            hits.push_back(createHit(version,url,timestamp));
        }
        
        // if appropriate, delete the hits
        if( removeFetchedFromDataStore && rc == SQLITE_OK )
        {
            zSQL_delete = sqlite3_mprintf("DELETE FROM hits WHERE id IN (SELECT id FROM hits ORDER BY id LIMIT %i)",limit);
            rc = sqlite3_exec(mDB, zSQL_delete, 0, 0, 0);
            printf("%i",rc);
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
    {
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
        rc = sqlite3_exec(mDB, "CREATE TABLE properties (key TEXT PRIMARY KEY, value TEXT);", NULL,NULL,NULL);
        // ignore rc, if there is an issue with this, it is probably because the table already exists
        rc = sqlite3_exec(mDB, "CREATE TABLE hits (id INTEGER PRIMARY KEY, version TEXT, url TEXT, timestamp REAL);", NULL,NULL,NULL);
        // ignore rc, if there is an issue with this, it is probably because the table already exists
        return true;
    }
}