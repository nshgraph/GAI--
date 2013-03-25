//
//  HitStore.h
//  gai++
//
//  Created by Nathan Holmberg on 10/03/13.
//  Copyright (c) 2013 nshgraph. All rights reserved.
//

#ifndef __HitStore_h__
#define __HitStore_h__

#include <string>


namespace GAI
{
    class Hit;
    
	class HitStore
	{
    public:
        virtual bool storeHit( const Hit& hit ) = 0;
	};
}

#endif
