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
     
    /// 用于保存顶点坐标与顶点索引的配对，可以方便地用顶点坐标取得顶点在全局顶点列表中的索引
    typedef std::map< std::pair<float, float>, ushort > PointIndexMap;

    /** 保存寻路所需的各种信息，以及各种导出导入功能
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

        /** 从旧版的region文件中取得阻挡多边形信息，并用cgal计算出各种寻路所需的信息
        @param fileName region文件名
        @param gridSize 每个grid的大小，单位是游戏逻辑坐标
        */
        bool getDataFromRegion(const std::string& fileName, int gridSize);

        /// 导出寻路信息到.path文件
        void exportPathFile(const std::string& fileName);

        /// 从.path文件中读取寻路信息
        void importPathFile(const std::string& fileName);
        void importPathFile(char* pathFileMem, ulong fileSize);

        /// 把寻路信息导出到xml文件中，方便查看数据
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

        /// 获取调用getDataFromRegion时所产生的错误信息
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