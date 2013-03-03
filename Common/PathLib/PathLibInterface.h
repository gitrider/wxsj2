#ifndef __PathLibInterface_H__
#define __PathLibInterface_H__

#include "PathLibPrerequisites.h"
#include "PathLibType.h"
#include "MathLib.h"

#include <string>
#include <map>

namespace PathLib
{
    class PathData;
    class Grid;
    struct Edge;
    struct Triangle;

    struct ResultPoint
    {
        ResultPoint(float _x, float _y)
            : x(_x)
            , y(_y)
        {
        }

        float x;
        float y;
    };
    typedef std::vector<ResultPoint> ResultPoints;

    struct IntersectPoint
    {
        ushort flag;
        Vector2 intersectPos;
    };
    typedef std::map<float, IntersectPoint> IntersectPoints;

    class _PathLibExport PathFinder
    {
    public:

        virtual ~PathFinder(void) = 0;

        virtual void init(PathData* dataMaker) = 0;

        virtual void clear(void) = 0;

        virtual bool findPath(float startx, float startz, float endx, float endz) = 0;

        virtual bool findPath(uint startID, uint endID, const Vector2& startPos, const Vector2& endPos) = 0;
        virtual bool getDirectPath(float startx, float startz, float endx, float endz) = 0;

        virtual bool isPointInRegion(float x, float z, uint& triangleID, char obstacle = 1) = 0;

        virtual size_t getNumPathPoints(void) const = 0;

        virtual const ResultPoint& getResultPoint(size_t index) const = 0;

        virtual const IntersectPoints& getIntersectPoints(void) = 0;
    };

    class _PathLibExport PathData
    {
    public:

        virtual ~PathData(void) = 0;

		virtual bool getDataFromRegion(const std::string& fileName, int gridSize) = 0;

        /// 导出寻路信息到.path文件
        virtual void exportPathFile(const std::string& fileName) = 0;

        /// 从.path文件中读取寻路信息
        virtual void importPathFile(const std::string& fileName) = 0;
        virtual void importPathFile(char* pathFileMem, ulong fileSize) = 0;

        /// 把寻路信息导出到xml文件中，方便查看数据
        virtual void exportPathFileToXML(const std::string& fileName) = 0;

        virtual void clear(void) = 0;

        virtual std::pair<int, int> getGrid(float x, float z) const = 0;

        virtual bool isValidGrid(int x, int z) const = 0;

        virtual const Grid& getGrid(int x, int z) const = 0;
        /**
        */
        virtual bool intersectTriangleInGrid(int x, int z, Vector2 startPos, Vector2 endPos, char isObstacle = 1) const = 0;
        virtual bool intersectEdgeInGrid(int x, int z, Vector2 startPos, Vector2 endPos) const = 0;

        /// 获取调用getDataFromRegion时所产生的错误信息
		virtual const std::string& getErrorString(void) = 0;

        virtual const Edge& getEdge(ushort index) = 0;
        virtual const Triangle& getTriangle(ushort index) = 0;
        virtual const Vector2& getGlobalVertex(ushort index) = 0;

    };

	struct ResultPoint3
	{
		float x, y, z;
        ResultPoint3(float _x, float _y, float _z)
            : x(_x)
            , y(_y)
			, z(_z)
        {
        }
	};

	class _PathLibExport BuildingCollision
	{
	public:
		virtual bool openSceneFile(const char* pSource, unsigned int fileSize) = 0;
		virtual bool getHeight(float x, float& y, float z) = 0;
		virtual bool collsionRay(const ResultPoint3& org, const ResultPoint3& dir, ResultPoint3& hit) = 0;
	};

    class _PathLibExport PathLibRoot
    {
    public:

        PathData* getPathData(void);
        PathFinder* getPathFinder(void);
		BuildingCollision* getBuildingCollsion(void);

        void destroyPathData(PathData* data);
        void destroyPathFinder(PathFinder* finder);
		void destroyBuildingCollision(BuildingCollision* building);
    };
}




#endif