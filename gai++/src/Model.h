
#ifndef __Model_h__
#define __Model_h__

#include <map>
#include <string>


namespace GAI
{
	class Model
	{
    public:
        typedef std::map<std::string,std::string> ParameterMap;
    public:
        Model();
        std::string get(const std::string& key);
        Model::ParameterMap getKeysAndValues();
        
        void set(const std::string& key, const std::string& value);
        void setForNextHit(const std::string& key, const std::string& value);
        void setAll(const ParameterMap& parameters, bool isForNextHit);
        
        void clearTemporaryValues();
    private:
        ParameterMap values;
        ParameterMap temporaryValues;
	};
}

#endif
