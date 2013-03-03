#ifndef __FAIRYRESOURCECOLLECTION_H__
#define __FAIRYRESOURCECOLLECTION_H__

#include "FairyPrerequisites.h"

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

namespace Fairy {

    class ResourceCollection
    {
    public:
        ResourceCollection(void) {}
        virtual ~ResourceCollection() {}

        virtual void add(const Ogre::String& resourceType, const Ogre::String& resourceName) = 0;
    };

}

#endif 
