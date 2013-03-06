#include <exception>
#include <string>
#include <list>


#include "HitBuilder.h"
#include "Hit.h"
#include "Model.h"
#include "HitType.h"

namespace GAI {


Hit* HitBuilder::createHit(HitType aType, Model* aModel) {
    throw "Not yet implemented";
}

std::list<std::string> HitBuilder::requiredParametersForType(std::string& aType) {
	throw "Not yet implemented";
}

}