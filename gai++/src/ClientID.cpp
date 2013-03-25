#include <exception>
#include <string>


#include "ClientID.h"
#include "DataStore.h"


extern "C"
{
#ifdef WIN32
#include <Rpc.h>
#else
#include <uuid/uuid.h>
#endif
}

std::string newUUID()
///
/// This function generates a UUID in a cross-platform manner
///
/// @return
///     A UUID
///
{
#ifdef WIN32
    UUID uuid;
    UuidCreate ( &uuid );
    
    unsigned char * str;
    UuidToStringA ( &uuid, &str );
    
    std::string s( ( char* ) str );
    
    RpcStringFreeA ( &str );
#else
    uuid_t uuid;
    uuid_generate_random ( uuid );
    char s[37];
    uuid_unparse ( uuid, s );
#endif
    return s;
}

namespace GAI
{
    
    std::string ClientID::generateClientID( DataStore& store )
    ///
    /// This function either retrieves or generates a unique ID for this client
    ///
    /// @param store
    ///     DataStore to use for retrieving and storing created clientID
    ///
    /// @return
    ///     The UUID generated for this client
    ///
    {
        // attempt to get clientID from data store
        std::string id = store.fetchProperty( "clientID");
        
        // if this is an empty string then this isn't valid so create
        if( id.empty())
        {
            id = newUUID();
            // store the newly created id
            store.addProperty("clientID", id);
        }
        
        return id;
    }
    
}