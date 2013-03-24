
#ifndef __Model_h__
#define __Model_h__

#include <map>
#include <string>

// This class stores parameters for use with sending a 'Hit'
// This inludes the functionality of storing a set of base values (that won't change between each use of the model)
// and a set of temporary values which can be cleared ready for the next use.


// model keys


namespace GAI
{
	class Model
    ///
    /// This class holds parameters that will be used for a hit. There is no validation on the parameters but
    /// this class can be passed to HitBuilder to create a real Hit
    ///
	{
    public:
        ///
        /// This typedef simplifies and instructs the kind of parameter map needed by functions of this class
        ///
        typedef std::map<std::string,std::string> ParameterMap;
    public:
        Model();
        bool hasParameter( const std::string& key) const;
        std::string get(const std::string& key) const;
        Model::ParameterMap getKeysAndValues() const;
        
        void set(const std::string& key, const std::string& value);
        void setForNextHit(const std::string& key, const std::string& value);
        void setAll(const ParameterMap& parameters, bool isForNextHit);
        
        void clearTemporaryValues();
    private:
        ParameterMap values; // set of values that will be stored until object destruction
        ParameterMap temporaryValues; // set of values that can be cleared with clearTemporaryValues()
	};
}

#endif
