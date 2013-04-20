
#include "GAIDefines.h"

#include <string>
#include <sstream>

#include "UrlBuilder.h"
#include "Timestamp.h"

namespace GAI {
    
    
    std::string UrlBuilder::createURL(const Hit& hit)
    ///
    /// This function generates a URL from a given hit
    ///
    /// @param hit
    ///     The hit to generate a url for
    ///
    /// @return
    ///     URL representing the hit
    ///
    {
        double timestamp = Timestamp::generateTimestamp();
        std::stringstream str;
        str << kGAIURLPage << "?" << hit.getDispatchURL() << "&" << kQueueTimeModelKey << "=" << (int)(timestamp - hit.getTimestamp());
        return str.str();
    }
}