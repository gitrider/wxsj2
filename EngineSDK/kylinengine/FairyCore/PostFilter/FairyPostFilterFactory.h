#ifndef __FAIRYPOSTFILTERFACTORY_H__
#define __FAIRYPOSTFILTERFACTORY_H__

#include "FairyPostFilterPrerequisites.h"

#include <OgreFactoryObj.h>

namespace Fairy {

// forward declaration
class PostFilter;

class PostFilterFactory : public Ogre::FactoryObj<PostFilter>
{
public:
    virtual void _init(void) = 0;
};

}

#endif // 
