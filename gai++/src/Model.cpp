
#include "Model.h"

namespace GAI
{
    
    Model::Model()
    {
        
    }
    std::string Model::get(const std::string& key)
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
        return "";
    }
    Model::ParameterMap Model::getKeysAndValues()
    {
        Model::ParameterMap result = values;
        result.insert( temporaryValues.begin(), temporaryValues.end() );
        return result;
    }
    
    void Model::set(const std::string& key, const std::string& value)
    {
        values[key] = value;
    }
    void Model::setForNextHit(const std::string& key, const std::string& value)
    {
        temporaryValues[key] = value;
    }
    void Model::setAll(const ParameterMap& parameters, bool isForNextHit)
    {
        if( isForNextHit )
            temporaryValues.insert( parameters.begin(), parameters.end() );
        else
            values.insert( parameters.begin(), parameters.end() );
    }
    
    void Model::clearTemporaryValues()
    {
        temporaryValues.clear();
    }
}