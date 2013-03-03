#ifndef __FAIRYSCENESOLVER_H__
#define __FAIRYSCENESOLVER_H__

#include "FairyResourceSolver.h"

namespace Fairy {
    class SceneInfo;

    ResourceSolverPtr findUnsolvedSceneItems(SceneInfo* sceneInfo);

}

#endif // 
