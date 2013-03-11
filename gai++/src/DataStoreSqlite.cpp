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
        if( return_code )
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
        return true;
    }
    bool DataStoreSqlite::addProperty(const std::string& name, const std::string& value)
    {
        return true;
    }
    std::string DataStoreSqlite::fetchProperty( const std::string& name )
    {
        return "";
    }
    std::map<std::string,std::string> DataStoreSqlite::fetchProperties()
    {
        std::map<std::string,std::string>  properties;
        return properties;
    }
    int DataStoreSqlite::propertyCount()
    {
        return 0;
    }
}