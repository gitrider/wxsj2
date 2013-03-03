#include "PathData.h"
#include "UtilFunc.h"

#include "PolygonSet.h"

#define TIXML_USE_STL
#include <tinyxml.h>

namespace PathLib
{
    /// 当前path文件的版本号
    ushort PATH_HEADER = 0X1000;
    /// region文件中的文件版本信息
    const char REGION_EDIT_VER1[] = "REGION_EDIT_01";

    //-----------------------------------------------------------------------
    void PathDataImpl::clear(void)
    {
        mErrorString.clear();
        mCPolygonList.clear();
        mTriangleList.clear();
        mGridList.clear();
        mEdgeList.clear();
        mGlobalVertexList.clear();
        mPointIndexMap.clear();
    }
    //-----------------------------------------------------------------------
    PathDataImpl::~PathDataImpl()
    {
        clear();
    }
    //-----------------------------------------------------------------------
    bool PathDataImpl::getDataFromRegion(const std::string& fileName, int gridSize)
    {
        clear();

        std::string regionName = fileName + ".region";
        std::string terrainName = fileName + ".terrain";

        mGridSize = gridSize;

        // 从terrain中读取各种信息
        TiXmlDocument xmldoc;

        TiXmlElement* pObjectElement = NULL;

        std::ifstream terrainFileStream(terrainName.c_str(), std::ios::in | std::ios::binary );

		if (terrainFileStream.is_open() == false) return false;
		

		terrainFileStream.seekg(0, std::ios_base::end);
        size_t fileSize = terrainFileStream.tellg();
        terrainFileStream.seekg(0, std::ios_base::beg);

        char* pBuf = new char[fileSize+1];
        terrainFileStream.read(pBuf, static_cast<std::streamsize>(fileSize));

        pBuf[fileSize] = '\0';

        xmldoc.Parse(pBuf);

        delete [] pBuf;

        if(xmldoc.Error())
        {
            mErrorString += "TiXMLDocument Error\n";
            return false;
        }

        pObjectElement = xmldoc.RootElement();
        if(!pObjectElement)
        {
            mErrorString += "TiXMLDocument Root Element is NULL\n";
            return false;
        }

        int xSize = 0;
        int zSize = 0;

        pObjectElement->Attribute("xsize", &xSize);
        pObjectElement->Attribute("zsize", &zSize);
        
        mNumTilePerX = (xSize-1) / mGridSize + 1;
        mNumTilePerZ = (zSize-1) / mGridSize + 1;

        terrainFileStream.close();

        // 读取region信息
        std::ifstream stream;
        stream.open( regionName.c_str(), std::ios::in | std::ios::binary );

		if (stream.is_open() == false) return false;

        char* version = new char[14];
        stream.read(version, 14);

        bool newRegionVersion = false;

        // 判断region文件的版本
        if( strncmp( version, REGION_EDIT_VER1, strlen(REGION_EDIT_VER1) ) == 0 )
            newRegionVersion = true;

        delete [] version;

        // 如果是旧版本，就把文件流设为开始处
        if (!newRegionVersion)
            stream.seekg(0, std::ios_base::beg);

        int regionCount = 0;

        stream.read(static_cast<char*>(static_cast<void*>(&regionCount)),
            static_cast<std::streamsize>(sizeof(int)));

#define USE_CGAL_JOIN 1

#if USE_CGAL_JOIN
        CPS_Polygon_set_2 polygonSet;
#endif
        // 每个region
        for (int i = 0; i < regionCount; ++i)
        {
            int regionID = 0;
            int regionFlag = 0;
            int regionPointCount = 0;

            // 读取region id
            stream.read(static_cast<char*>(static_cast<void*>(&regionID)),
                static_cast<std::streamsize>(sizeof(regionID)));

            if (newRegionVersion)
                stream.read(static_cast<char*>(static_cast<void*>(&regionFlag)),
                    static_cast<std::streamsize>(sizeof(regionFlag)));

            stream.read(static_cast<char*>(static_cast<void*>(&regionPointCount)),
                static_cast<std::streamsize>(sizeof(regionPointCount)));

            CPolygon cpolygon;
            JoinPolygon joinPolygon;

            PointList tempList;

            // region中的每个点
            for (int j = 0; j < regionPointCount; ++j)
            {
                Vector3 pos;

                stream.read( static_cast<char*>(static_cast<void*>(&pos)),
                    static_cast<std::streamsize>(sizeof(pos)));

                float tempFloat = pos.y;
                pos.y = 0.0;
                pos.z = tempFloat;

                bool insert = true;
                // 检查是否有重复点
                for (size_t tempIndex = 0; tempIndex < tempList.size(); ++tempIndex)
                {
                    if (tempList[tempIndex] == pos)
                    {
                        insert = false;
                        break;
                    }
                }

                if (insert)
                {
                    tempList.push_back(pos);

                    cpolygon.pushPoint(Vector2(pos.x, pos.z));
                    joinPolygon.pushPoint(Vector2(pos.x, pos.z));

                }
            }

            // 多于2个点才划分
            if (tempList.size() > 2)
            {     

                //////////////////////////////////////////////////////////////////////////
                // 先都转成逆时针的

                if (cpolygon.mCGALPolygon.is_simple())
                {
                    cpolygon.counterClockwise();

                    // 向polygon中设置flag
                    if ((regionFlag & 0x1) != 0)
                    {
                        cpolygon.mRegionFlag |= 0x1;
                    }

                    mCPolygonList.push_back(cpolygon);

                }
                else
                {
                    //logMgr.logMessage( "not simple : ");
                }                

#if USE_CGAL_JOIN

                if (joinPolygon.mCGALPolygon.is_simple())
                {
                    joinPolygon.counterClockwise();

                    polygonSet.join(joinPolygon.mCGALPolygon);

                }
                else
                {
           //         logMgr.logMessage( "not simple : ");
                }

#endif
            }
        } // end of regions

        CDT cdt;
#if USE_CGAL_JOIN
        std::list<CPS_Polygon_with_holes_2> res;
        std::list<CPS_Polygon_with_holes_2>::const_iterator it;

        size_t polygonCount = polygonSet.number_of_polygons_with_holes();

        polygonSet.polygons_with_holes (std::back_inserter (res));
        for (it = res.begin(); it != res.end(); ++it) 
        {
            PointList points;

            const CPS_Polygon_with_holes_2& polygon_with_hole = *it;
            size_t holeCount = polygon_with_hole.number_of_holes();

            bool unbounded = polygon_with_hole.is_unbounded();

            //logMgr.logMessage(Ogre::StringConverter::toString(holeCount) + " " +
            //    Ogre::StringConverter::toString(unbounded));

            if (!unbounded)
            {
                const CPS_Polygon_with_holes_2::General_polygon_2& P = polygon_with_hole.outer_boundary();

                for (CPS_Polygon_with_holes_2::General_polygon_2::Edge_const_iterator  vit = P.edges_begin(); vit != P.edges_end(); ++vit)
                {
                    Vector2 start1(static_cast<float>(to_double((*vit).source().x())),
                        static_cast<float>(to_double((*vit).source().y())));
                    Vector2 end1(static_cast<float>(to_double((*vit).target().x())),
                        static_cast<float>(to_double((*vit).target().y())));

                    cdt.insert(start1, end1);

                    mEdgeList.push_back( Edge( start1, end1, static_cast<ushort>(mEdgeList.size()) ) );
                }
            } 

            CPS_Polygon_with_holes_2::Hole_const_iterator  hit;

            for (hit = polygon_with_hole.holes_begin(); hit != polygon_with_hole.holes_end(); ++hit)
            {
                const CPS_Polygon_with_holes_2::General_polygon_2& P = *hit;

                for (CPS_Polygon_with_holes_2::General_polygon_2::Edge_const_iterator  vit = P.edges_begin(); vit != P.edges_end(); ++vit)
                {
                    Vector2 start1(static_cast<float>(to_double((*vit).source().x())),
                        static_cast<float>(to_double((*vit).source().y())));
                    Vector2 end1(static_cast<float>(to_double((*vit).target().x())),
                        static_cast<float>(to_double((*vit).target().y())));

                    cdt.insert(start1, end1);

                    mEdgeList.push_back( Edge( start1, end1, static_cast<ushort>(mEdgeList.size()) ) );
                }
            }
        }

#else
        for (size_t polygonIndex = 0; polygonIndex < mCPolygonList.size(); ++polygonIndex)
        {
            CPolygon& polygon = mCPolygonList[polygonIndex];

            // 多边形中的每条边
            for (EdgeIterator p1ei = polygon.mCGALPolygon.edges_begin();
                p1ei != polygon.mCGALPolygon.edges_end(); ++p1ei)
            {
                // 边的两个点
                Vector2 start1((*p1ei).source().x(), (*p1ei).source().y());
                Vector2 end1((*p1ei).target().x(), (*p1ei).target().y());

                cdt.insert(start1, end1);

                mEdgeList.push_back( Edge(start1, end1, mEdgeList.size()) );
            }
        }

#endif

        // 构建顶点列表
        for (CGALCDT::Finite_vertices_iterator fit = cdt.mCGALCDT.finite_vertices_begin();
            fit != cdt.mCGALCDT.finite_vertices_end(); ++fit)
        {
            Triangle triangle;

            CGALCDT::Vertex_handle face = fit;

            Vector2 pos(face->point().x(), face->point().y());

            if (mPointIndexMap.find(std::pair<float, float>(pos.x, pos.y)) != mPointIndexMap.end())
            {
                //LogManager::getSingleton().logMessage("point exist");
            }

       //     mPointIndexMap.insert(PointIndexMap::value_type(pos,mGlobalVertexList.size()));
            mPointIndexMap[ std::pair<float, float>(pos.x, pos.y) ] = mGlobalVertexList.size();
            mGlobalVertexList.push_back( pos );

            //LogManager::getSingleton().logMessage("vertex : " + Ogre::StringConverter::toString(Vector2(
            //    face->point().x(), face->point().y())));
        }

        if (mPointIndexMap.size() != mGlobalVertexList.size())
        {
            //LogManager::getSingleton().logMessage("Map size : " + Ogre::StringConverter::toString(mPointIndexMap.size())
            //    + " vertex list size : " + Ogre::StringConverter::toString(mGlobalVertexList.size()));
        }

        // 把三角形加入到全局列表
        for (CGALCDT::Finite_faces_iterator fit = cdt.mCGALCDT.finite_faces_begin();
            fit != cdt.mCGALCDT.finite_faces_end(); ++fit)
        {
            Triangle triangle;

            CGALCDT::Face_handle face = fit;

            triangle.pos1.x = face->vertex(0)->point().x();
            triangle.pos1.y = face->vertex(0)->point().y();
            triangle.pos2.x = face->vertex(1)->point().x();
            triangle.pos2.y = face->vertex(1)->point().y();
            triangle.pos3.x = face->vertex(2)->point().x();
            triangle.pos3.y = face->vertex(2)->point().y();

            // 查找三角形顶点所对应的ID
            triangle.vertexID[0] = getVertexIndex(triangle.pos1);
            triangle.vertexID[1] = getVertexIndex(triangle.pos2);
            triangle.vertexID[2] = getVertexIndex(triangle.pos3);

            triangle.id = static_cast<ushort>(mTriangleList.size());
            triangle.cgalFaceHandle = face;

            triangle.midPoint.x = (triangle.pos1.x + triangle.pos2.x + triangle.pos3.x) / 3;
            triangle.midPoint.y = (triangle.pos1.y + triangle.pos2.y + triangle.pos3.y) / 3;

            // 遍历所有的region多边形，如果这个三角形的中心点在多边形内，说明他是属于这个region的
            // 也就是说是阻挡区域三角形
            for (size_t polygonIndex = 0; polygonIndex < mCPolygonList.size(); ++polygonIndex)
            {
                CPolygon& polygon = mCPolygonList[polygonIndex];

                if (polygon.ptInPolygon(triangle.midPoint))
                {
                    triangle.setObstacle(true);

                    // 设置可飞越标志位
                    if ((polygon.mRegionFlag & 0x1) != 0)
                    {
                        triangle.setFlyable(true);
                    }
                    break;
                }
            }

            triangle.calBoundingBox();

            mTriangleList.push_back(triangle);
        }

        // edge信息
        for (size_t edgeIndex = 0; edgeIndex < mEdgeList.size(); ++edgeIndex)
        {
            Edge& edge = mEdgeList[edgeIndex];

            edge.vertexID[0] = getVertexIndex(edge.mPoint1);
            edge.vertexID[1] = getVertexIndex(edge.mPoint2);

            bool firstTri = true;

            for (size_t triIndex = 0; triIndex < mTriangleList.size(); ++triIndex)
            {
                Triangle& triangle = mTriangleList[triIndex];

                if ( (edge.vertexID[0] == triangle.vertexID[0] ||
                    edge.vertexID[0] == triangle.vertexID[1] ||
                    edge.vertexID[0] == triangle.vertexID[2]) &&
                    (edge.vertexID[1] == triangle.vertexID[0] ||
                    edge.vertexID[1] == triangle.vertexID[1] ||
                    edge.vertexID[1] == triangle.vertexID[2]) )
                {
                    if (firstTri)
                    {
                        edge.triangleID[0] = triangle.id;
                        firstTri = false;
                    }
                    else
                        edge.triangleID[1] = triangle.id;
                }
            }
        }
        
        // 建立三角形间的链接关系
        for (size_t triIndex = 0; triIndex < mTriangleList.size(); ++triIndex)
        {
            Triangle& triangle = mTriangleList[triIndex];

            CGALCDT::Face_handle faceHandle = triangle.cgalFaceHandle;
            

            for (int i=0; i<3; ++i)
            {
                CGALCDT::Face_handle neighborFace = faceHandle->neighbor(i);

                for (size_t triIndex1 = 0; triIndex1 < mTriangleList.size(); ++triIndex1)
                {
                    if (mTriangleList[triIndex1].cgalFaceHandle == neighborFace)
                    {
                        triangle.neighborId[i] = static_cast<ushort>(triIndex1);
                        break;
                    }
                }
            }
        }

        // 设置island信息
        ushort islandID = 0;
        for (size_t i=0; i<mTriangleList.size(); ++i)
        {
            Triangle& triangle = mTriangleList[i];

            if (false == triangle.getObstacle() && triangle.mIslandID == 0)
            {
                ++islandID;
                setTriangleIslandID(triangle, islandID);
            }
        }

        // 设置grid信息
        mGridList.resize(mNumTilePerZ * mNumTilePerX);

        // 划分grid
        for (int z = 0; z < mNumTilePerZ; ++z)
            for (int x = 0; x < mNumTilePerX; ++x)
            {
                Grid grid;
                grid.mMinPos.x = static_cast<float>(x * mGridSize);
                grid.mMinPos.y = static_cast<float>(z * mGridSize);
                grid.mMaxPos.x = grid.mMinPos.x + mGridSize;
                grid.mMaxPos.y = grid.mMinPos.y + mGridSize;
                grid.mXIndex = x;
                grid.mZIndex = z;

                // 把每个三角形分配到grid中
                for (size_t triIndex = 0; triIndex < mTriangleList.size(); ++triIndex)
                {
                    const Triangle& triangle = mTriangleList[triIndex];

                    if (intersectGridTriangle(grid, triangle))
                    {
                        grid.mTriangleIDList.push_back(triIndex);
                    }
                }

                // 把edge分配到grid中
                for (size_t edgeIndex = 0; edgeIndex < mEdgeList.size(); ++edgeIndex)
                {
                    const Edge& edge = mEdgeList[edgeIndex];

                    if (edgeInGrid(edge.mPoint1, edge.mPoint2, grid))
                    {
                        grid.mEdgeIDList.push_back(edgeIndex);
                    }
                }

                mGridList[z*mNumTilePerX + x] = grid;
            }

        return true;
    }
    //-----------------------------------------------------------------------
    bool PathDataImpl::intersectTriangleInGrid(int x, int z, Vector2 startPos, Vector2 endPos, char isObstacle) const
    {
        //LogManager::getSingleton().logMessage("check triangle : " + StringConverter::toString(x) + " "
        //    + StringConverter::toString(z));

        const Grid& grid = getGrid(x, z);

        for (size_t triIndex = 0; triIndex < grid.mTriangleIDList.size(); ++triIndex)
        {
            const Triangle& triangle = mTriangleList[ grid.mTriangleIDList[triIndex] ];

            if (isObstacle)
            {
                if (triangle.getObstacle())
                {
                    float xcorss, ycorss;

                    if (intersectWith(startPos, endPos, triangle.pos1, triangle.pos2, xcorss, ycorss))
                        return true;
                    if (intersectWith(startPos, endPos, triangle.pos2, triangle.pos3, xcorss, ycorss))
                        return true;
                    if (intersectWith(startPos, endPos, triangle.pos1, triangle.pos3, xcorss, ycorss))
                        return true;
                }
            }
            else
            {
                if (false == triangle.getObstacle())
                {
                    float xcorss, ycorss;

                    if (intersectWith(startPos, endPos, triangle.pos1, triangle.pos2, xcorss, ycorss))
                        return true;
                    if (intersectWith(startPos, endPos, triangle.pos2, triangle.pos3, xcorss, ycorss))
                        return true;
                    if (intersectWith(startPos, endPos, triangle.pos1, triangle.pos3, xcorss, ycorss))
                        return true;
                }
            }
        } 

        return false;
    }
    //-----------------------------------------------------------------------
    bool PathDataImpl::intersectEdgeInGrid(int x, int z, Vector2 startPos, Vector2 endPos) const
    {
        const Grid& grid = getGrid(x, z);

        for (size_t edgeIndex = 0; edgeIndex < grid.mEdgeIDList.size(); ++edgeIndex)
        {
            const Edge& edge = mEdgeList[ grid.mEdgeIDList[edgeIndex] ];
            
            float xcorss, ycorss;

            if (intersectWith(startPos, endPos, edge.mPoint1, edge.mPoint2, xcorss, ycorss))
            {
                //LogManager::getSingleton().logMessage("check edge intersect : " + StringConverter::toString(x) + " "
                //    + StringConverter::toString(z));

                return true;
            }
        } 

        //LogManager::getSingleton().logMessage("check edge unintersect : " + StringConverter::toString(x) + " "
        //    + StringConverter::toString(z));

        return false;
    }
    //-----------------------------------------------------------------------
    void PathDataImpl::exportPathFile(const std::string& fileName)
    {

		// vertex count
        ushort vertexcount = static_cast<ushort>(mGlobalVertexList.size());
	
		if (vertexcount == 0 )
			return;

        std::ofstream stream;
        stream.open( fileName.c_str(), std::ios::out | std::ios::binary );

        /**
        header uint16

        vertex count ushort
        global vertex list sizeof(Vector2) * vertexcount

        triangle count ushort
            for each triangle
            vertex id 3 * ushort
            neighbor id 3 * ushort
            flag
            islandid

        edge count ushort
            for each edge
            vertex id 2 * ushort
            triangle id 2 * ushort

        grid x size ushort
        grid x tile size ushort
        grid z tile size ushort
        for each grid
            triangle id count ushort
            edge id count ushort
            triangle id list sizeof(ushort) * triangle id count
            edge id list sizeof(ushort) * edge id count        
        */

        // header 
        stream.write(static_cast<char*>(static_cast<void*>(&PATH_HEADER)), sizeof(uint16));


        stream.write(static_cast<char*>(static_cast<void*>(&vertexcount)), sizeof(ushort));
        // global vertex list
        stream.write(static_cast<char*>(static_cast<void*>(&mGlobalVertexList[0])), sizeof(Vector2) * vertexcount);

        // triangle count
        ushort trianglecount = static_cast<ushort>(mTriangleList.size());
        stream.write(static_cast<char*>(static_cast<void*>(&trianglecount)), sizeof(ushort));
        // triangle info
        for (ushort i=0; i<trianglecount; ++i)
        {
            const Triangle& triangle = mTriangleList[i];

            ushort data[6];
            data[0] = triangle.vertexID[0];
            data[1] = triangle.vertexID[1];
            data[2] = triangle.vertexID[2];

            data[3] = triangle.neighborId[0];
            data[4] = triangle.neighborId[1];
            data[5] = triangle.neighborId[2];

            stream.write(static_cast<char*>(static_cast<void*>(&data[0])), sizeof(ushort) * 6);
            stream.write(&triangle.mFlag, sizeof(char));

            ushort islandID = triangle.mIslandID;
            stream.write(static_cast<char*>(static_cast<void*>(&islandID)), sizeof(ushort));
        }

        // edge count
        ushort edgecount = static_cast<ushort>(mEdgeList.size());
        stream.write(static_cast<char*>(static_cast<void*>(&edgecount)), sizeof(ushort));
        // edge info
        for(ushort i=0; i<mEdgeList.size(); ++i)
        {
            const Edge& edge = mEdgeList[i];

            ushort data[4];
            data[0] = edge.vertexID[0];
            data[1] = edge.vertexID[1];
            data[2] = edge.triangleID[0];
            data[3] = edge.triangleID[1];
            stream.write(static_cast<char*>(static_cast<void*>(&data[0])), sizeof(ushort) * 4);
        }

        // grid count
        stream.write(static_cast<char*>(static_cast<void*>(&mGridSize)), sizeof(float));
        stream.write(static_cast<char*>(static_cast<void*>(&mNumTilePerX)), sizeof(ushort));
        stream.write(static_cast<char*>(static_cast<void*>(&mNumTilePerZ)), sizeof(ushort));

        // grid info
        for(ushort i=0; i<mGridList.size(); ++i)
        {
            const Grid& grid = mGridList[i];

            ushort trianglesize = static_cast<ushort>(grid.mTriangleIDList.size());
            ushort edgesize = static_cast<ushort>(grid.mEdgeIDList.size());

            stream.write(static_cast<char*>(static_cast<void*>(&trianglesize)), sizeof(ushort));
            stream.write(static_cast<char*>(static_cast<void*>(&edgesize)), sizeof(ushort));

            const TriangleIDList& triIDList = grid.mTriangleIDList;

            for (size_t j=0; j<triIDList.size(); ++j)
            {
                ushort id = triIDList[j];
                stream.write(static_cast<char*>(static_cast<void*>(&id)), sizeof(ushort));

            }

            const EdgeIDList& edgeIDList = grid.mEdgeIDList;

            for (size_t j=0; j<edgeIDList.size(); ++j)
            {
                ushort id = edgeIDList[j];
                stream.write(static_cast<char*>(static_cast<void*>(&id)), sizeof(ushort));
            }
        }        

        stream.close();
    }
    //-----------------------------------------------------------------------
    void PathDataImpl::importPathFile(const std::string& fileName)
    {
        std::ifstream stream;
        stream.open( fileName.c_str(), std::ios::in | std::ios::binary );

        stream.seekg(0, std::ios_base::end);
        size_t fileSize = stream.tellg();
        stream.seekg(0, std::ios_base::beg);

        char* fileMem = new char[fileSize];
        stream.read(fileMem, static_cast<std::streamsize>(fileSize));

        importPathFile(fileMem, static_cast<ulong>(fileSize));

        delete [] fileMem;

        stream.close();
    }
    //-----------------------------------------------------------------------
    void PathDataImpl::importPathFile(char* pathFileMem, ulong fileSize)
    {
        clear();

        char* filePointer = pathFileMem;

        ushort header;

        memcpy(&header, filePointer, sizeof(ushort));
        filePointer += sizeof(ushort);

        // vertex
        ushort vertexcount;
        memcpy(&vertexcount, filePointer, sizeof(ushort));
        filePointer += sizeof(ushort);

        mGlobalVertexList.resize(vertexcount);
        memcpy(&mGlobalVertexList[0], filePointer, sizeof(Vector2) * vertexcount);
        filePointer += sizeof(Vector2) * vertexcount;

        // triangle
        ushort trianglecount;
        memcpy(&trianglecount, filePointer, sizeof(ushort));
        filePointer += sizeof(ushort);

        mTriangleList.resize(trianglecount);
        for (ushort i=0; i<trianglecount; ++i)
        {
            Triangle& triangle = mTriangleList[i];

            ushort data[6];
            memcpy(&data[0], filePointer, sizeof(ushort) * 6);
            filePointer += sizeof(ushort) * 6;


            triangle.vertexID[0] = data[0];
            triangle.vertexID[1] = data[1];
            triangle.vertexID[2] = data[2];

            triangle.neighborId[0] = data[3];
            triangle.neighborId[1] = data[4];
            triangle.neighborId[2] = data[5];

            memcpy(&triangle.mFlag, filePointer, sizeof(char));
            filePointer += sizeof(char);

            memcpy(&triangle.mIslandID, filePointer, sizeof(ushort));
            filePointer += sizeof(ushort);

            triangle.id = i;

            triangle.pos1 = mGlobalVertexList[ triangle.vertexID[0] ];
            triangle.pos2 = mGlobalVertexList[ triangle.vertexID[1] ];
            triangle.pos3 = mGlobalVertexList[ triangle.vertexID[2] ];

            triangle.midPoint.x = (triangle.pos1.x + triangle.pos2.x + triangle.pos3.x) / 3;
            triangle.midPoint.y = (triangle.pos1.y + triangle.pos2.y + triangle.pos3.y) / 3;

            triangle.calBoundingBox();
        }

        // edge
        ushort edgecount;
        memcpy(&edgecount, filePointer, sizeof(ushort));
        filePointer += sizeof(ushort);

        mEdgeList.resize(edgecount);
        for(ushort i=0; i<edgecount; ++i)
        {
            Edge& edge = mEdgeList[i];

            ushort data[4];
            memcpy(&data[0], filePointer, sizeof(ushort) * 4);
            filePointer += sizeof(ushort) * 4;

            edge.vertexID[0] = data[0];
            edge.vertexID[1] = data[1];
            edge.triangleID[0] = data[2];
            edge.triangleID[1] = data[3];

            edge.mID = i;
            edge.mPoint1 = mGlobalVertexList[ edge.vertexID[0] ];
            edge.mPoint2 = mGlobalVertexList[ edge.vertexID[1] ];

        }

        // grid
        memcpy(&mGridSize, filePointer, sizeof(float));
        filePointer += sizeof(float);

        memcpy(&mNumTilePerX, filePointer, sizeof(ushort));
        filePointer += sizeof(ushort);

        memcpy(&mNumTilePerZ, filePointer, sizeof(ushort));
        filePointer += sizeof(ushort);

        ushort gridcount = mNumTilePerX * mNumTilePerZ; 

        mGridList.resize(gridcount);

        for (int z = 0; z < mNumTilePerZ; ++z)
            for (int x = 0; x < mNumTilePerX; ++x)
            {
                Grid& grid = mGridList[z*mNumTilePerX + x];
                grid.mMinPos.x = static_cast<float>(x * mGridSize);
                grid.mMinPos.y = static_cast<float>(z * mGridSize);
                grid.mMaxPos.x = grid.mMinPos.x + mGridSize;
                grid.mMaxPos.y = grid.mMinPos.y + mGridSize;
                grid.mXIndex = x;
                grid.mZIndex = z;

                ushort trianglesize;
                memcpy(&trianglesize, filePointer, sizeof(ushort));
                filePointer += sizeof(ushort);

                ushort edgesize;
                memcpy(&edgesize, filePointer, sizeof(ushort));
                filePointer += sizeof(ushort);

                for (ushort triIndex = 0; triIndex < trianglesize; ++triIndex)
                {
                    ushort id;
                    memcpy(&id, filePointer, sizeof(ushort));
                    filePointer += sizeof(ushort);

                    grid.mTriangleIDList.push_back(id);
                }

                for (ushort edgeIndex = 0; edgeIndex < edgesize; ++edgeIndex)
                {
                    ushort id;
                    memcpy(&id, filePointer, sizeof(ushort));
                    filePointer += sizeof(ushort);

                    grid.mEdgeIDList.push_back(id);
                }
            }
    }
    //-----------------------------------------------------------------------
    ushort PathDataImpl::getVertexIndex(const Vector2& pos) const
    {
        PointIndexMap::const_iterator it = mPointIndexMap.find(std::pair<float,float>(pos.x, pos.y));

        if (it == mPointIndexMap.end())
            return INVALID_VERTEX_INDEX;
        else
            return it->second;
    }
    //-----------------------------------------------------------------------
    void PathDataImpl::setTriangleIslandID(Triangle& triangle, ushort islandID)
    {
        triangle.mIslandID = islandID;

        for (int i=0; i<3; ++i)
        {
            ushort neighborID = triangle.neighborId[i];

            if (neighborID != INVALID_TRI_INDEX)
            {
                Triangle& neighborTri = mTriangleList[neighborID];

                if (false == neighborTri.getObstacle() && neighborTri.mIslandID != islandID)
                {
                    if (neighborTri.mIslandID != 0)
                    {
                        //LogManager::getSingleton().logMessage("error island : ");
                        continue;
                    }

                    setTriangleIslandID(neighborTri, islandID);
                }

            }
        }
    }
    //-----------------------------------------------------------------------
    void PathDataImpl::exportPathFileToXML(const std::string& fileName)
    {
        TiXmlDocument* exportDocument = new TiXmlDocument();
        exportDocument->InsertEndChild(TiXmlElement("PathInfo"));

        // 输出header
        TiXmlElement* rootNode = exportDocument->RootElement();
        TiXmlElement* headerNode = 
            rootNode->InsertEndChild(TiXmlElement("Header"))->ToElement();

        headerNode->SetAttribute("version", PATH_HEADER);

        // 输出vertex
        size_t vertexCount = mGlobalVertexList.size();
        TiXmlElement* vertexListNode = 
            rootNode->InsertEndChild(TiXmlElement("vertex List"))->ToElement();
        vertexListNode->SetAttribute("count", static_cast<int>(vertexCount));

        for (size_t i=0; i<vertexCount; ++i)
        {
            TiXmlElement* vertexNode = 
                vertexListNode->InsertEndChild(TiXmlElement("vertex"))->ToElement();

            const Vector2& pos = mGlobalVertexList[i];

            vertexNode->SetAttribute("id", static_cast<int>(i));

            vertexNode->SetDoubleAttribute("x", pos.x);
            vertexNode->SetDoubleAttribute("y", pos.y);
        }

        // 输出triangle
        size_t triangleCount = mTriangleList.size();
        TiXmlElement* triangleListNode = 
            rootNode->InsertEndChild(TiXmlElement("triangle List"))->ToElement();
        triangleListNode->SetAttribute("count", static_cast<int>(triangleCount));

        for (size_t i=0; i<triangleCount; ++i)
        {
            TiXmlElement* triangleNode = 
                triangleListNode->InsertEndChild(TiXmlElement("triangle"))->ToElement();

            const Triangle& triangle = mTriangleList[i];

            triangleNode->SetAttribute("id", triangle.id);

            triangleNode->SetAttribute("vertex1", triangle.vertexID[0]);
            triangleNode->SetAttribute("vertex2", triangle.vertexID[1]);
            triangleNode->SetAttribute("vertex3", triangle.vertexID[2]);

            triangleNode->SetAttribute("neighbor1", triangle.neighborId[0]);
            triangleNode->SetAttribute("neighbor2", triangle.neighborId[1]);
            triangleNode->SetAttribute("neighbor3", triangle.neighborId[2]);

            triangleNode->SetAttribute("island", triangle.mIslandID);

            triangleNode->SetAttribute("obstacle", triangle.getObstacle() ? "true" : "false");
            triangleNode->SetAttribute("flyable", triangle.getFlyable() ? "true" : "false");
        }

        // 输出edge
        size_t edgeCount = mEdgeList.size();
        TiXmlElement* edgeListNode = 
            rootNode->InsertEndChild(TiXmlElement("edge List"))->ToElement();
        edgeListNode->SetAttribute("count", static_cast<int>(edgeCount));

        for (size_t i=0; i<edgeCount; ++i)
        {
            TiXmlElement* edgeNode = 
                edgeListNode->InsertEndChild(TiXmlElement("edge"))->ToElement();

            const Edge& edge = mEdgeList[i];

            edgeNode->SetAttribute("id", edge.mID);

            edgeNode->SetAttribute("vertex1", edge.vertexID[0]);
            edgeNode->SetAttribute("vertex2", edge.vertexID[1]);

            edgeNode->SetAttribute("triangle1", edge.triangleID[0]);
            edgeNode->SetAttribute("triangle2", edge.triangleID[1]);
        }

        // 输出grid
        size_t gridCount = mGridList.size();
        TiXmlElement* gridListNode = 
            rootNode->InsertEndChild(TiXmlElement("grid List"))->ToElement();

        gridListNode->SetAttribute("count", static_cast<int>(gridCount));
        gridListNode->SetAttribute("grid size", mGridSize);
        gridListNode->SetAttribute("grid num per x", mNumTilePerX);
        gridListNode->SetAttribute("grid num per z", mNumTilePerZ);

        for(size_t i=0; i<gridCount; ++i)
        {
            TiXmlElement* gridNode = 
                gridListNode->InsertEndChild(TiXmlElement("grid"))->ToElement();

            const Grid& grid = mGridList[i];

            gridNode->SetAttribute("id", static_cast<int>(i));
            gridNode->SetAttribute("x", grid.mXIndex);
            gridNode->SetAttribute("z", grid.mZIndex);

            TiXmlElement* triangleListNode = 
                gridNode->InsertEndChild(TiXmlElement("triangle ID list"))->ToElement();

            size_t trianglesize = grid.mTriangleIDList.size();
            triangleListNode->SetAttribute("count", static_cast<int>(trianglesize));

            for (size_t triIndex = 0; triIndex < trianglesize; ++triIndex)
            {
                TiXmlElement* triangleNode = 
                    triangleListNode->InsertEndChild(TiXmlElement("triangle ID"))->ToElement();

                triangleNode->SetAttribute("id", triIndex);
                triangleNode->SetAttribute("triangle id", static_cast<int>(grid.mTriangleIDList[triIndex]));
            }

            TiXmlElement* edgeListNode = 
                gridNode->InsertEndChild(TiXmlElement("edge ID list"))->ToElement();

            size_t edgesize = grid.mEdgeIDList.size();
            edgeListNode->SetAttribute("count", static_cast<int>(edgesize));

            for (size_t edgeIndex = 0; edgeIndex < edgesize; ++edgeIndex)
            {
                TiXmlElement* edgeNode = 
                    edgeListNode->InsertEndChild(TiXmlElement("edge ID"))->ToElement();

                edgeNode->SetAttribute("id", static_cast<int>(edgeIndex));
                edgeNode->SetAttribute("edge id", grid.mEdgeIDList[edgeIndex]);
            }            
        }       

        // Write out to a file
        exportDocument->SaveFile(fileName + ".xml");

        delete exportDocument;
    }
}