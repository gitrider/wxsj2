#include "FairyObjectFactory.h"
#include "FairyObject.h"

namespace Fairy
{

    void ObjectFactory::destroyInstance(Object* object)
    {
        delete object;
    }

}
