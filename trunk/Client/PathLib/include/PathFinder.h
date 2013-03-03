#ifndef __PathFinder_H__
#define __PathFinder_H__

#include "PathLibInterface.h"

#include <vector>
#include <map>
#include "Triangle.h"
#include "Edge.h"

namespace PathLib
{
    class PathData;

    typedef std::vector<Triangle> TriangleList;
    typedef std::vector<Edge> EdgeList;

#define NOINIT 0
#define OPENSTATE 1
#define CLOSESTATE 2

    struct NodeInfo
    {
        NodeInfo()
            : triangleId(0), parentTriId(0)
            , hValue(0.0f), gValue(0.0f), state(NOINIT)
        {
        }

        uint triangleId;
        uint parentTriId;
        float hValue;
        float gValue;
        char state; // 0 ��ʼ 1 open 2 close
    };

    typedef std::vector<NodeInfo> NodeInfos;

    struct PathVertex
    {
        PathVertex(const Vector2& pos, bool isused = false)
            : pathPos(pos)
            , used(isused)
        {
        }

        bool used;
        Vector2 pathPos;
    };

    typedef std::vector<ushort> PathPoints;
    typedef std::vector<PathVertex> PathVerteces;
    
    typedef std::vector<uint> StateList;

    class PathFinderImpl : public PathFinder
    {
    public:

        PathFinderImpl(PathData* dataMaker);
        PathFinderImpl()
            : mNodeInfos(0)
            , mOffsetWidth(0.1f)
        {}

        ~PathFinderImpl();

        void init(PathData* dataMaker);

        void clear(void);

        bool findPath(float startx, float startz, float endx, float endz);

        bool findPath(uint startID, uint endID, const Vector2& startPos, const Vector2& endPos);

        bool getDirectPath(float startx, float startz, float endx, float endz);

        /** �ж�һ�����Ƿ�����ָ���������У�����ǣ��������ڵ������ε�ID
        @param x ���жϵĵ��x��������
        @param z ���жϵĵ��z��������
        @param triangleID �����ڵ�������ID
        @param obstacle ��Ҫ�жϵ��������ͣ�1Ϊ�赲����0Ϊ��������
        */
        bool isPointInRegion(float x, float z, uint& triangleID, char obstacle = 1);

        size_t getNumPathPoints(void) const
        {
            return mResultPoints.size();
        }

        const ResultPoint& getResultPoint(size_t index) const
        {
            assert (index < mResultPoints.size() && "Index out of bounds!");

            return mResultPoints[index];
        }

        const IntersectPoints& getIntersectPoints(void)
        {
            return mIntersectPoints;
        };

    public:

        PathPoints mResultPath;
        PathVerteces mPathVerteces;
        ResultPoints mResultPoints;
        IntersectPoints mIntersectPoints;

    protected:

        void smoothPath(PathPoints& points, const Vector2& startPos, const Vector2& endPos);

        bool isWalkAble(const Vector2& startPos, const Vector2& endPos);

        bool intersectTriangleInGrid(IntersectPoints& outputPoints, int x, int z, Vector2 startPos, Vector2 endPos, char isObstacle = 1) const;
        bool intersectEdgeInGrid(IntersectPoints& outputPoints, int x, int z, Vector2 startPos, Vector2 endPos) const;
    protected:

        StateList mOpenList;
        StateList mCloseList;

        TriangleList mTriangleList;

        PathData* mDataMaker;
        NodeInfo* mNodeInfos;

        float mOffsetWidth;
    };

}

#endif