#ifndef __FAIRYMATERIALSCRIPTSOLVER_H__
#define __FAIRYMATERIALSCRIPTSOLVER_H__

#include "FairyResourceSolver.h"

namespace Fairy {

    ResourceSolverPtr findUnsolvedMaterialTextures(const Ogre::String& group = Ogre::StringUtil::BLANK);

}

#endif // 
