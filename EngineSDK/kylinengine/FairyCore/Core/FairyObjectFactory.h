#ifndef __FAIRYOBJECTFACTORY_H__
#define __FAIRYOBJECTFACTORY_H__

#include "FairyFactory.h"

namespace Fairy
{
    class Object;

    class ObjectFactory : public Factory<Object>
    {
    public:
        void destroyInstance(Object* object);
    };

}

#endif 
