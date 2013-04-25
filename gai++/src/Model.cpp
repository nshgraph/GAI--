
#include "Model.h"

namespace GAI
{
	std::string Model::FailString = "";
    
    Model::Model()
    {
        
    }
    
    bool Model::hasParameter( const std::string& key) const
    ///
    /// Determines if the key provided exists in the Model
    ///
    /// @param key
    ///     The key to look for
    ///
    /// @return
    ///     Wether the property was found in either map
    ///
    {
        ParameterMap::const_iterator it = values.find(key);
        ParameterMap::const_iterator it_temp = temporaryValues.find(key);
        return ( it != values.end() || it_temp != temporaryValues.end() );
    }
    
    const std::string& Model::get(const std::string& key) const
    ///
    /// Retrieves the value of the assoicated key. Looks in the temporary values first
    ///
    /// @param key
    ///     The key to look for
    ///
    /// @return
    ///     The associated value or an empty string on fail
    ///
    {
        // first look in temporary map
        ParameterMap::const_iterator it = temporaryValues.find(key);
        if( it != temporaryValues.end() )
            return it->second;
        // then look in permenant
        it = values.find(key);
        if( it != values.end() )
            return it->second;
        // if fails return empty string
        return FailString;
    }
    
    Model::ParameterMap Model::getKeysAndValues() const
    ///
    /// Retrieves all current key value pairs
    ///
    /// @return
    ///     All properties of this Model
    ///
    {
        Model::ParameterMap result = values;
        result.insert( temporaryValues.begin(), temporaryValues.end() );
        return result;
    }
    
    void Model::set(const std::string& key, const std::string& value)
    ///
    /// Sets a 'permenant' property by assigning the value to the key. A pre-existing key will be overriden
    ///
    /// @param key
    ///     The key to look for
    /// @param value
    ///     The value to associate with the key
    ///
    {
        values[key] = value;
    }
    
    void Model::setForNextHit(const std::string& key, const std::string& value)
    ///
    /// Sets a 'temporary' property by assigning the value to the key. A pre-existing key will be overriden
    ///
    /// @param key
    ///     The key to look for
    /// @param value
    ///     The value to associate with the key
    ///
    {
        temporaryValues[key] = value;
    }
    
    void Model::setAll(const ParameterMap& parameters, bool isForNextHit)
    ///
    /// Adds all provided parameters to the model. Allows for setting both permenant and temporary
    ///
    /// @param parameters
    ///     The key to look for
    /// @param isForNextHit
    ///     Whether the provided parameters should be stored as permenant or temporary
    ///
    {
        if( isForNextHit )
            temporaryValues.insert( parameters.begin(), parameters.end() );
        else
            values.insert( parameters.begin(), parameters.end() );
    }
    
    void Model::remove(const std::string& key)
    ///
    /// Removes a key from the set of parameters
    ///
    /// @param key
    ///     The key to remove
    ///
    {
        values.erase( key );
        temporaryValues.erase( key );
    }
    
    void Model::clearTemporaryValues()
    ///
    /// Clears all temporary values in the Model while retaining those marked as permenant
    ///
    {
        temporaryValues.clear();
    }
}