#ifndef __FAIRYTERRAINSOLVER_H__
#define __FAIRYTERRAINSOLVER_H__

#include "FairyResourceSolver.h"

namespace Fairy {
    class Terrain;

    ResourceSolverPtr findUnsolvedTerrainBrushes(Terrain* terrain);

}

#endif // 
