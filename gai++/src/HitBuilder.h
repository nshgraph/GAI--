#include <exception>
#include <string>
#include <list>


#ifndef __HitBuilder_h__
#define __HitBuilder_h__

#include "HitType.h"

namespace GAI
{
	class Hit;
	class Model;
    
	class HitBuilder
	{
    public:
        HitBuilder();
        bool createHit(const HitType aType, const Model& aModel, Hit*& hit) const;
    protected:
        std::list<std::string> mRequirements[kNumHitTypes];
        const std::list<std::string>& requiredParametersForType(const HitType aType) const;
        
	};
}

#endif
