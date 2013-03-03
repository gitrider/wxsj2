#ifndef __DataMaker_H__
#define __DataMaker_H__

#include "PathLibInterface.h"

#include "Polygon.h"
#include "Triangle.h"
#include "Grid.h"
#include "Edge.h"
#include "ieee.h"

#include <vector>
#include <map>
#include <string>

namespace PathLib
{
    typedef std::vector<CPolygon> CPolygonList;
    typedef std::vector<Triangle> TriangleList;
    typedef std::vector<Grid> GridList;
    typedef std::vector<Edge> EdgeList;
     
    /// ���ڱ��涥�������붥����������ԣ����Է�����ö�������ȡ�ö�����ȫ�ֶ����б��е�����
    typedef std::map< std::pair<float, float>, ushort > PointIndexMap;

    /** ����Ѱ·����ĸ�����Ϣ���Լ����ֵ������빦��
    */
    class PathDataImpl : public PathData
    {
    public:

        PathDataImpl()
            : mGridSize(0)
            , mNumTilePerX(0)
            , mNumTilePerZ(0)
        {
        }

        ~PathDataImpl();

        /** �Ӿɰ��region�ļ���ȡ���赲�������Ϣ������cgal���������Ѱ·�������Ϣ
        @param fileName region�ļ���
        @param gridSize ÿ��grid�Ĵ�С����λ����Ϸ�߼�����
        */
        bool getDataFromRegion(const std::string& fileName, int gridSize);

        /// ����Ѱ·��Ϣ��.path�ļ�
        void exportPathFile(const std::string& fileName);

        /// ��.path�ļ��ж�ȡѰ·��Ϣ
        void importPathFile(const std::string& fileName);
        void importPathFile(char* pathFileMem, ulong fileSize);

        /// ��Ѱ·��Ϣ������xml�ļ��У�����鿴����
        void exportPathFileToXML(const std::string& fileName);

        void clear(void);

        std::pair<int, int> getGrid(float x, float z) const
        {
            return std::pair<int, int>(
                fast_int(x / mGridSize),
                fast_int(z / mGridSize));
        }

        bool isValidGrid(int x, int z) const
        {
            return 0 <= x && x < mNumTilePerX && 0 <= z && z < mNumTilePerZ;
        }

        const Grid& getGrid(int x, int z) const
        {
            assert (isValidGrid(x,z));
            return mGridList[z * mNumTilePerX + x];
        }

        /**
        */
        bool intersectTriangleInGrid(int x, int z, Vector2 startPos, Vector2 endPos, char isObstacle = 1) const;
        bool intersectEdgeInGrid(int x, int z, Vector2 startPos, Vector2 endPos) const;

        /// ��ȡ����getDataFromRegionʱ�������Ĵ�����Ϣ
        const std::string& getErrorString(void)
        {
            return mErrorString;
        }

        const Edge& getEdge(ushort index)
        {
            return mEdgeList[index];
        }   

        const Triangle& getTriangle(ushort index)
        {
            return mTriangleList[index];
        }

        const Vector2& getGlobalVertex(ushort index)
        {
            return mGlobalVertexList[index];
        }

    private:

        void setTriangleIslandID(Triangle& triangle, ushort islandID);

        ushort getVertexIndex(const Vector2& pos) const;

    private:

        std::string mErrorString;

    public:

        ushort mGridSize;
        ushort mNumTilePerX;
        ushort mNumTilePerZ;

        CPolygonList mCPolygonList;
        TriangleList mTriangleList;
        GridList mGridList;
        EdgeList mEdgeList;
        Point2DList mGlobalVertexList;
        PointIndexMap mPointIndexMap;
    };
}

#endif