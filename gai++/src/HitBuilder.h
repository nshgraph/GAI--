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
		static Hit* createHit(HitType aType, Model* aModel);
    public:
		std::list<std::string> requiredParametersForType(std::string& aType);
	};
}

#endif
