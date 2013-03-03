#ifndef __ScorpioPrerequisites_H__
#define __ScorpioPrerequisites_H__

#include <OgrePrerequisites.h>

//-----------------------------------------------------------------------
// Windows Settings
//-----------------------------------------------------------------------
#if (OGRE_PLATFORM == OGRE_PLATFORM_WIN32) && !defined(__MINGW32__)
#   if (OGRE_VERSION >= 0x010300 && defined(OGRE_STATIC_LIB)) ||    \
	(OGRE_VERSION < 0x010300 && OGRE_DYNAMIC_LINKAGE == 0)
#       pragma message "No dynamic linkage"
#       define _ScorpioExport
#   else
#       ifdef PLUGIN_Scorpio_EXPORTS
#           define _ScorpioExport __declspec(dllexport)
#       else
#           define _ScorpioExport __declspec(dllimport)
#       endif
#   endif
#else
#   define _ScorpioExport
#endif

namespace Ogre
{
    // Predeclare classes
    class Cloth;
    class Fabric;

    class UserCollideTester;
    class Collider;
    class StdCollider;
    class UserCollider;
    class ColliderSet;
    class CollisionManager;
}

#endif // __ScorpioPrerequisites_H__
