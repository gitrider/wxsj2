#include "PathLibInterface.h"

#include "PathData.h"
#include "PathFinder.h"
#include "BuildingCollision.h"

namespace PathLib
{

    PathFinder::~PathFinder(void)
    {
    }

    PathData::~PathData(void)
    {
    }

    PathData* PathLibRoot::getPathData(void)
    {
        return new PathDataImpl;
    }

    PathFinder* PathLibRoot::getPathFinder(void)
    {
        return new PathFinderImpl;
    }

	BuildingCollision* PathLibRoot::getBuildingCollsion(void)
	{
		return new BuildingCollsionImpl;
	}

    void PathLibRoot::destroyPathData(PathData* data)
    {
        delete data;
    }

    void PathLibRoot::destroyPathFinder(PathFinder* finder)
    {
        delete finder;
    }

	void PathLibRoot::destroyBuildingCollision(BuildingCollision* building)
	{
		delete building;
	}


}