#include "PathFinder.h"
#include "UtilFunc.h"

#include "PathData.h"
#include "ieee.h"

#include <fstream>
#include <algorithm>

namespace PathLib
{
    //-----------------------------------------------------------------------
    PathFinderImpl::PathFinderImpl(PathData* dataMaker)
    {
        mNodeInfos = 0;
        mOffsetWidth = 0.1f;
        init(dataMaker);
    }
    //-----------------------------------------------------------------------
    PathFinderImpl::~PathFinderImpl()
    {
        clear();
    }
    //-----------------------------------------------------------------------
    void PathFinderImpl::init(PathData* dataMaker)
    {
        clear();

        mTriangleList = static_cast<PathDataImpl*>(dataMaker)->mTriangleList;
        mDataMaker = dataMaker;

        mNodeInfos = new NodeInfo[mTriangleList.size()];
    }
    //-----------------------------------------------------------------------
    void PathFinderImpl::clear(void)
    {
        if (mNodeInfos)
        {
            delete [] mNodeInfos;
            mNodeInfos = 0;

        }

        mResultPath.clear();
        mOpenList.clear();
        mCloseList.clear();
        mResultPoints.clear();
        mPathVerteces.clear();
    }
    //-----------------------------------------------------------------------
    bool PathFinderImpl::findPath(float startx, float startz, float endx, float endz)
    {
        uint startID, endID;

        // 判断起始点是否在可走区域中
        if (false == isPointInRegion(startx, startz, startID, 0))
        {
            return false;
        }
        // 判断目标点是否在可走区域中
        if (false == isPointInRegion(endx, endz, endID, 0))
        {
            return false;
        }

        const Triangle& startTri = mTriangleList[startID];
        const Triangle& endTri = mTriangleList[endID];

        // 如果起始点所在的三角形和目标点所在的三角形处于不同的island，说明不可以到达
        if (startTri.mIslandID != endTri.mIslandID)
            return false;
        
        return findPath(startID, endID, Vector2(startx, startz), Vector2(endx, endz));
    }
    //-----------------------------------------------------------------------
    bool PathFinderImpl::findPath(uint startID, uint endID, const Vector2& startPos, const Vector2& endPos)
    { 
        // 查找之前，清空之前的数据
        ::memset(mNodeInfos, 0, sizeof(NodeInfo) * mTriangleList.size());

        mOpenList.clear();
        mCloseList.clear();
        mResultPath.clear();
        mPathVerteces.clear();
        mResultPoints.clear();

        // 起点终点相同，返回
        if (startID == endID)
        {
            mResultPoints.push_back(ResultPoint(startPos.x, startPos.y));
            mResultPoints.push_back(ResultPoint(endPos.x, endPos.y));

            return true;
        }

        // 检测起点终点的三角形是否连通
        if ( isWalkAble(startPos, endPos) )
        {
            mResultPoints.push_back(ResultPoint(startPos.x, startPos.y));
            mResultPoints.push_back(ResultPoint(endPos.x, endPos.y));

            return true;
        }

        // 把起点放入open list
        mNodeInfos[startID].state = OPENSTATE;
        mNodeInfos[startID].gValue = 0;
        mNodeInfos[startID].triangleId = startID;

        mOpenList.push_back(startID);

        // 当前工作的格子坐标
        uint currentId = 0;

        // 只要open list不为空，就一直执行
        while (mOpenList.size())
        {
            // 找出f值最小的node，直接取出第一个item，因为binary heap保证了第一个元素的f值最小
            NodeInfo& minTileInfo = mNodeInfos[ mOpenList[0] ];

            currentId = mOpenList[0];

            const Triangle& currentTri = mTriangleList[currentId];

            // 如果当前工作点是目标点，就说明找到了
            if (currentId == endID)
            {
                // 输出路径
                uint triangleId = 0;

                triangleId = minTileInfo.parentTriId;

                mResultPath.push_back(endID);

                while(triangleId != startID) 
                {
                    mResultPath.push_back(triangleId);

                    NodeInfo& tile = mNodeInfos[triangleId];
                    triangleId = tile.parentTriId;

                }

                mResultPath.push_back(startID);

                // 平滑路径，得到拐点
                smoothPath(mResultPath, startPos, endPos);

                return true;
            }            

            {
                // 删除open list中最小的item
                // binary heap删除
                //1.把最后一个元素删除，移到第一个位置
                mNodeInfos[ mOpenList[0] ].state = NOINIT;

                int openListSize = static_cast<int>(mOpenList.size());
                mOpenList[0] = mOpenList[openListSize-1];
                mOpenList.erase(mOpenList.end()-1);
                --openListSize;      

                int heapIndex = 0;
                int swapIndex = 0;
                // 2.拿一个元素和他的孩子比较
                do 
                {
                    swapIndex = heapIndex;
                    // 两个孩子都存在
                    if ( ((swapIndex+1)*2) <= (openListSize - 1) )
                    {
                        NodeInfo& tile = mNodeInfos[ mOpenList[swapIndex] ];
                        NodeInfo& child1 = mNodeInfos[ mOpenList[(swapIndex+1)*2 - 1] ];
                        NodeInfo& child2 = mNodeInfos[ mOpenList[(swapIndex+1)*2] ];


                        if ( (tile.hValue+tile.gValue) >= (child1.hValue+child1.gValue) )
                        {
                            heapIndex = (swapIndex+1)*2 - 1;
                        }

                        NodeInfo& tempNode = mNodeInfos[ mOpenList[heapIndex] ];
                        if ( (tempNode.hValue+tempNode.gValue) >= (child2.hValue+tempNode.gValue) )
                        {
                            heapIndex = (swapIndex+1)*2;
                        }

                    }
                    // 只有一个孩子存在
                    else if ( ((swapIndex+1)*2 - 1) <= (openListSize - 1) )
                    {
                        NodeInfo& tile = mNodeInfos[ mOpenList[swapIndex] ];
                        NodeInfo& child1 = mNodeInfos[ mOpenList[(swapIndex+1)*2 - 1] ];

                        if ( (tile.hValue+tile.gValue) >= (child1.hValue+child1.gValue) )
                        {
                            heapIndex = (swapIndex+1)*2 - 1;
                        }
                    }

                    if (heapIndex != swapIndex)
                    {
                        uint temp = mOpenList[swapIndex];
                        mOpenList[swapIndex] = mOpenList[heapIndex];
                        mOpenList[heapIndex] = temp;
                    }
                    else
                        break;

                } 
                while(true);
            }

            // 放入close list
            minTileInfo.state = CLOSESTATE;
            mCloseList.push_back(minTileInfo.triangleId);

            // 检查与这个三角形连通的三角形
            for (int neighborIndex = 0; neighborIndex < 3; ++neighborIndex)
            {
                // 是否在合法范围内
                if (INVALID_TRI_INDEX == currentTri.neighborId[neighborIndex])
                    continue;

                Triangle& neighborTri = mTriangleList[ currentTri.neighborId[neighborIndex] ];

                NodeInfo& neighborNodeInfo = mNodeInfos[ neighborTri.id ];
                bool inOpenList = neighborNodeInfo.state == OPENSTATE;
                bool inCloseList = neighborNodeInfo.state == CLOSESTATE;
                bool walkAble =  !neighborTri.getObstacle();

                if (!walkAble)
                {
                    continue;
                }

                // 如果不在close list中，该格可走，不在open list中
                if ( false == inCloseList )
                {
                    if (false == inOpenList)
                    {
                        NodeInfo& tileInfo = mNodeInfos[neighborTri.id];
                        tileInfo.triangleId = neighborTri.id;

                        // 指向当前格
                        tileInfo.parentTriId = currentId;

                        // 在close中肯定能找到parent tile，因为在检查八个邻接的格子之前，已经把当前格
                        // 从open中删除，并加入到close中
                        NodeInfo& parentTile = minTileInfo;

                        // 计算value
                        tileInfo.gValue = 
                            (neighborTri.midPoint.x - currentTri.midPoint.x) * (neighborTri.midPoint.x - currentTri.midPoint.x)
                            + (neighborTri.midPoint.y - currentTri.midPoint.y) * (neighborTri.midPoint.y - currentTri.midPoint.y)
                            + parentTile.gValue;

                        const Triangle& triangle = mTriangleList[ currentTri.id ];
                        const Triangle& nextTri = mTriangleList[ neighborTri.id ];

                        Vector2 edgePos1, edgePos2;

                        int pointCount = 0;

                        for (int j=0; j<3; ++j)
                        {
                            for (int k=0; k<3; ++k)
                            {
                                if (triangle.vertexID[j] == nextTri.vertexID[k])
                                {
                                    if (pointCount == 0)
                                    {
                                        edgePos1 = mDataMaker->getGlobalVertex( triangle.vertexID[j] );
                                    }
                                    else if (pointCount == 1)
                                    {
                                        edgePos2 = mDataMaker->getGlobalVertex( triangle.vertexID[j] );
                                    }

                                    ++pointCount;
                                }
                            }
                        }

                        float misValue = Math::POS_INFINITY;

                        float tempHvalue = (edgePos1.x - endPos.x) * (edgePos1.x - endPos.x)
                            + (edgePos1.y - endPos.y) * (edgePos1.y - endPos.y);
                        
                        if (tempHvalue < misValue)
                        {
                            misValue = tempHvalue;
                        }

                        tempHvalue = (edgePos2.x - endPos.x) * (edgePos2.x - endPos.x)
                            + (edgePos2.y - endPos.y) * (edgePos2.y - endPos.y);

                        if (tempHvalue < misValue)
                        {
                            misValue = tempHvalue;
                        }                  
              
                        tileInfo.hValue = misValue;

                        tileInfo.state = OPENSTATE;
                        mOpenList.push_back(neighborTri.id);

                        {
                            // binary heap排序
                            size_t heapIndex = mOpenList.size()-1;

                            while (heapIndex != 0)
                            {
                                NodeInfo& tile1 = mNodeInfos[ mOpenList[heapIndex] ];
                                NodeInfo& tile2 = mNodeInfos[ mOpenList[(heapIndex-1)/2] ];

                                if ( (tile1.gValue+tile1.hValue) <= (tile2.gValue+tile2.hValue) )
                                {
                                    uint temp = tile2.triangleId;
                                    mOpenList[(heapIndex-1)/2] = tile1.triangleId;
                                    mOpenList[heapIndex] = temp;

                                    heapIndex = (heapIndex-1)/2;
                                }
                                else
                                    break;
                            }
                        }
                    }   
                    // 如果已经在open list中了，更新信息
                    else
                    {
                        // 找到open list中的位置
                        for (size_t i=0; i<mOpenList.size(); ++i)
                        {
                            if (mOpenList[i] == neighborTri.id)
                            {
                                size_t index = i;
                                NodeInfo& openNode = mNodeInfos[neighborTri.id];
                                NodeInfo& parentTile = minTileInfo;

                                // 计算value
                                float gValue = 
                                    (neighborTri.midPoint.x - currentTri.midPoint.x) * (neighborTri.midPoint.x - currentTri.midPoint.x)
                                    + (neighborTri.midPoint.y - currentTri.midPoint.y) * (neighborTri.midPoint.y - currentTri.midPoint.y)
                                    + parentTile.gValue;

                                if (gValue < openNode.gValue)
                                {
                                    //Ogre::LogManager::getSingleton().logMessage("open already value : " +
                                    //    Ogre::StringConverter::toString(gValue) + " " +
                                    //    Ogre::StringConverter::toString(openNode.gValue) );

                                    openNode.gValue = gValue;

                                    openNode.parentTriId = currentId;

                                    {
                                        // 重新对堆进行排序
                                        while (index != 0)
                                        {
                                            //Ogre::LogManager::getSingleton().logMessage("open already value index : " +
                                            //    Ogre::StringConverter::toString(index));

                                            NodeInfo& tile1 = mNodeInfos[ mOpenList[index] ];
                                            NodeInfo& tile2 = mNodeInfos[ mOpenList[(index-1)/2] ];

                                            if ( (tile1.gValue+tile1.hValue) <= (tile2.gValue+tile2.hValue) )
                                            {
                                                uint temp = tile2.triangleId;
                                                mOpenList[(index-1)/2] = tile1.triangleId;
                                                mOpenList[index] = temp;

                                                index = (index-1)/2;

                                            }
                                            else
                                                break;
                                        }
                                    }
                                }

                                break;
                            }
                        }
                    }
                }
                // 在close list中
                else
                {
                //    NodeInfo& closeNode = getTileInfoFromCloseList(neighborTri.id);
                    NodeInfo& closeNode = mNodeInfos[neighborTri.id];
                    NodeInfo& parentTile = minTileInfo;

                    // 计算value
                    float gValue = 
                        (neighborTri.midPoint.x - currentTri.midPoint.x) * (neighborTri.midPoint.x - currentTri.midPoint.x)
                        + (neighborTri.midPoint.y - currentTri.midPoint.y) * (neighborTri.midPoint.y - currentTri.midPoint.y)
                        + parentTile.gValue;

                    if (gValue < closeNode.gValue)
                    {
                        //Ogre::LogManager::getSingleton().logMessage("close already value : " +
                        //    Ogre::StringConverter::toString(gValue) + " " +
                        //    Ogre::StringConverter::toString(closeNode.gValue) );

                        closeNode.gValue = gValue;

                        closeNode.parentTriId = currentId;                           
                    }
                }
            }    
        } 

        // 能到达这里，说明找不到路径了
        return false;
    }
    //-----------------------------------------------------------------------
    bool PathFinderImpl::getDirectPath(float startx, float startz, float endx, float endz)
    {
        mIntersectPoints.clear();
        // 用dda来检测两点间经过的grid
        std::pair<int, int> checkGridIndex = mDataMaker->getGrid(startx, startz);
        std::pair<int, int> nextGridIndex = mDataMaker->getGrid(endx, endz);

        int x0 = checkGridIndex.first;
        int y0 = checkGridIndex.second;
        int x1 = nextGridIndex.first;
        int y1 = nextGridIndex.second;

        Vector2 startPos(startx, startz), endPos(endx, endz);

        if (x0 == x1 && y0 == y1)
        {
            intersectEdgeInGrid(mIntersectPoints, x0, y0, startPos, endPos);
        }

        int startX,startY,endX,endY;

        if (x0 > x1)
        {
            startX = x1;
            endX = x0;
        }
        else
        {
            startX = x0;
            endX = x1;
        }

        if (y0 > y1)
        {
            startY = y1;
            endY = y0;
        }
        else
        {
            startY = y0;
            endY = y1;
        }

        // 判断x相同的情况
        if (x0 == x1)
        {
            for (int i=startY; i<=endY; ++i)
            {
                intersectEdgeInGrid(mIntersectPoints, x0, i, startPos, endPos);            
            }
        }

        if (y0 == y1)
        {
            for (int i=startX; i<=endX; ++i)
            {
                intersectEdgeInGrid(mIntersectPoints, i, y0, startPos, endPos);
            }
        }

        float dx,dy,m;

        dx = (float)(x1 - x0);
        dy = (float)(y1 - y0);

        float xlength = abs(dx);
        float ylength = abs(dy);

        if (ylength > xlength)
            m = dx / dy;
        else
            m = dy /dx;

        int startIndex,endIndex;
        float coord;

        if (xlength > ylength)
        {
            startIndex = startX;
            endIndex = endX;
            coord = x1 > x0 ? (float)(y0) : (float)(y1);
        }
        else
        {
            startIndex = startY;
            endIndex = endY;
            coord = y1 > y0 ? (float)(x0) : (float)(x1);
        }

        int coord1, coord2;
        for (int i=startIndex; i<=endIndex; ++i)
        {
            if (xlength > ylength)
            {
                coord1 = i;
                coord2 = fast_ifloor(coord + 0.5f);

            }
            else
            {
                coord1 = fast_ifloor(coord + 0.5f);
                coord2 = i;

            }

            intersectEdgeInGrid(mIntersectPoints, coord1, coord2, startPos, endPos);

            if (xlength > ylength)
            {
                if (mDataMaker->isValidGrid(coord1, coord2-1))
                    intersectEdgeInGrid(mIntersectPoints, coord1, coord2-1, startPos, endPos);
                if (mDataMaker->isValidGrid(coord1, coord2+1))
                    intersectEdgeInGrid(mIntersectPoints, coord1, coord2+1, startPos, endPos);
            }
            else
            {
                if (mDataMaker->isValidGrid(coord1-1, coord2))
                    intersectEdgeInGrid(mIntersectPoints, coord1-1, coord2, startPos, endPos);
                if (mDataMaker->isValidGrid(coord1+1, coord2))
                    intersectEdgeInGrid(mIntersectPoints, coord1+1, coord2, startPos, endPos);               
            }

            coord += m;
        }

        return true;
    }
    //-----------------------------------------------------------------------
    void PathFinderImpl::smoothPath(PathPoints& points, const Vector2& startPos, const Vector2& endPos)
    {
        PathVerteces visitedPoints;

        mPathVerteces.push_back(endPos);

        Vector2 end = endPos;
       
        for (size_t i=0; i<points.size()-1; ++i)
        {
            const Triangle& triangle = mTriangleList[ points[i] ];
            const Triangle& nextTri = mTriangleList[ points[i+1] ];

            Vector2 edgePos1, edgePos2;
            ushort edgePos1ID, edgePos2ID;

            int pointCount = 0;

            for (int j=0; j<3; ++j)
            {
                for (int k=0; k<3; ++k)
                {
                    if (triangle.vertexID[j] == nextTri.vertexID[k])
                    {
                        if (pointCount == 0)
                        {
                            edgePos1 = mDataMaker->getGlobalVertex( triangle.vertexID[j] );
                            edgePos1ID = triangle.vertexID[j];
                        }
                        else if (pointCount == 1)
                        {
                            edgePos2 = mDataMaker->getGlobalVertex( triangle.vertexID[j] );
                            edgePos2ID = triangle.vertexID[j];
                        }

                        ++pointCount;
                    }
                }
            }

            //if (pointCount > 2)
            //    LogManager::getSingleton().logMessage("point count > 2:");

            float minDis = Math::POS_INFINITY;

            Vector2 minPos;    
            ushort minPosID;

            float dis;        

            dis = (edgePos1.x - startPos.x) * (edgePos1.x - startPos.x) + (edgePos1.y - startPos.y) * (edgePos1.y - startPos.y)
                + (edgePos1.x - end.x) * (edgePos1.x - end.x) + (edgePos1.y - end.y) * (edgePos1.y - end.y);

            if (dis < minDis)
            {
                minDis = dis;
                minPos = edgePos1;
                minPosID = edgePos1ID;
            }

            dis = (edgePos2.x - startPos.x) * (edgePos2.x - startPos.x) + (edgePos2.y - startPos.y) * (edgePos2.y - startPos.y)
                + (edgePos2.x - end.x) * (edgePos2.x - end.x) + (edgePos2.y - end.y) * (edgePos2.y - end.y);

            if (dis < minDis)
            {
                minDis = dis;
                minPos = edgePos2;
                minPosID = edgePos2ID;
            }


            /*float crossx, crossy;
            bool lineCross = intersectWith(triangle.midPoint, nextTri.midPoint, edgePos1, edgePos2, crossx, crossy);

            if (lineCross)
            {
                dis = (nextTri.midPoint.x - startPos.x) * (nextTri.midPoint.x - startPos.x) + (nextTri.midPoint.y - startPos.y) * (nextTri.midPoint.y - startPos.y)
                    + (nextTri.midPoint.x - end.x) * (nextTri.midPoint.x - end.x) + (nextTri.midPoint.y - end.y) * (nextTri.midPoint.y - end.y);

                if (dis < minDis)
                {
                    minDis = dis;
                    minPos = nextTri.midPoint;
                }

            }*/

            bool insert = true;

            for (size_t i=0; i<visitedPoints.size(); ++i)
            {
                if (visitedPoints[i].pathPos == minPos)
                {
                    insert = false;
                    break;
                }
            }

            if (insert)
            {  
                //      end = minPos;
                visitedPoints.push_back(minPos);

                bool firstPos = true;
                ushort edgePosID[2];

                // 找到点所在的grid
                std::pair<int, int> gridIndex = mDataMaker->getGrid(minPos.x, minPos.y);

                const Grid& grid = mDataMaker->getGrid(gridIndex.first, gridIndex.second);

                for (size_t edgeIndex = 0; edgeIndex < grid.mEdgeIDList.size(); ++edgeIndex)
                {
                    const Edge& edge = mDataMaker->getEdge( grid.mEdgeIDList[edgeIndex] );

                    if (edge.vertexID[0] == minPosID)
                    {
                        if (firstPos)
                        {
                            edgePosID[0] = edge.vertexID[1];
                            firstPos = false;
                        }
                        else
                        {
                            edgePosID[1] = edge.vertexID[1];
                        }
                    }
                    else if (edge.vertexID[1] == minPosID)
                    {
                        if (firstPos)
                        {
                            edgePosID[0] = edge.vertexID[0];
                            firstPos = false;
                        }
                        else
                        {
                            edgePosID[1] = edge.vertexID[0];
                        }
                    }
                }

                Vector2 edgeDir1 = (mDataMaker->getGlobalVertex(  edgePosID[0] ) - minPos).normalisedCopy();
                Vector2 edgeDir2 = (mDataMaker->getGlobalVertex(  edgePosID[1] ) - minPos).normalisedCopy();
                Vector2 direction = (edgeDir1 + edgeDir2) / 2;
                direction.normalise();

                Vector2 tempPos = minPos;
                uint id;

                tempPos += direction*mOffsetWidth;

                if (isPointInRegion(tempPos.x, tempPos.y, id, 1))
                {
                    //LogManager::getSingleton().logMessage("Start Index is not valid : " +
                    //    Ogre::StringConverter::toString(startx) + " " + Ogre::StringConverter::toString(startz));

                    tempPos = minPos;
                    tempPos += -direction*mOffsetWidth;
                }

                minPos = tempPos;

                mPathVerteces.push_back(minPos);
            }
            /*else
            {
                if (lineCross)
                {
                    minPos = nextTri.midPoint;
                    minPos += ( (triangle.midPoint + nextTri.midPoint)/2 -minPos ).normalisedCopy()*30;

                    mPathVerteces.push_back(minPos);
                }
            }*/
        }


        mPathVerteces.push_back(startPos);

        /*size_t checkPointIndex = 0;
        size_t currentPointIndex = 1;
        size_t nextPointIndex = 2;

        if (mPathVerteces.empty() || mPathVerteces.size() < 3)
            return;

        while (nextPointIndex < mPathVerteces.size())
        {
            if ( isWalkAble(mPathVerteces[checkPointIndex].pathPos, mPathVerteces[nextPointIndex].pathPos) )
            {
                mPathVerteces[currentPointIndex].used = false;
                currentPointIndex = nextPointIndex;
                ++nextPointIndex;
            }
            else
            {
                checkPointIndex = currentPointIndex;
                currentPointIndex = nextPointIndex;
                ++nextPointIndex;
            }
        }*/
        size_t checkPointIndex = 0;
        size_t currentPointIndex = 1;
        size_t nextPointIndex = 1;

        if (mPathVerteces.empty() || mPathVerteces.size() < 3)
            return;


        while (nextPointIndex < mPathVerteces.size())
        {
            for (size_t i = nextPointIndex; i<mPathVerteces.size(); ++i)
            {
                if ( isWalkAble(mPathVerteces[checkPointIndex].pathPos, mPathVerteces[i].pathPos) )
                {
        //            mPathVerteces[currentPointIndex].used = false;
        //            currentPointIndex = nextPointIndex;
                    nextPointIndex = i;
                }

            }

            mPathVerteces[checkPointIndex].used = true;
            mPathVerteces[nextPointIndex].used = true;

            checkPointIndex = nextPointIndex;
            nextPointIndex = checkPointIndex + 1;
            //else
            //{
            //    checkPointIndex = currentPointIndex;
            //    currentPointIndex = nextPointIndex;
            //    ++nextPointIndex;
            //}
        }

        for (size_t i = 0; i < mPathVerteces.size(); ++i)
        {
            if (mPathVerteces[mPathVerteces.size() - 1 - i].used)
                mResultPoints.push_back(ResultPoint(mPathVerteces[mPathVerteces.size() - 1 - i].pathPos.x, mPathVerteces[mPathVerteces.size() - 1 - i].pathPos.y));
        }
    }
    //-----------------------------------------------------------------------
    bool PathFinderImpl::isWalkAble(const Vector2& startPos, const Vector2& endPos)
    {
        // 用dda来检测两点间经过的grid
        std::pair<int, int> checkGridIndex = mDataMaker->getGrid(startPos.x, startPos.y);
        std::pair<int, int> nextGridIndex = mDataMaker->getGrid(endPos.x, endPos.y);

        int x0 = checkGridIndex.first;
        int y0 = checkGridIndex.second;
        int x1 = nextGridIndex.first;
        int y1 = nextGridIndex.second;

        //LogManager::getSingleton().logMessage("is walkable : " + StringConverter::toString(x0) + " "
        //    + StringConverter::toString(y0) + " "
        //    + StringConverter::toString(x1) + " "
        //    + StringConverter::toString(y1));

        if (x0 == x1 && y0 == y1)
        {
            if (mDataMaker->intersectEdgeInGrid(x0, y0,
                startPos, endPos))
                return false;
            else
                return true;

        }

        int startx,starty,endx,endy;

        if (x0 > x1)
        {
            startx = x1;
            endx = x0;
        }
        else
        {
            startx = x0;
            endx = x1;
        }

        if (y0 > y1)
        {
            starty = y1;
            endy = y0;
        }
        else
        {
            starty = y0;
            endy = y1;
        }

        // 判断x相同的情况
        if (x0 == x1)
        {
            for (int i=starty; i<=endy; ++i)
            {
                if (mDataMaker->intersectEdgeInGrid(x0, i,
                            startPos, endPos))
                            return false;
            }

            return true;
        }

        if (y0 == y1)
        {
            for (int i=startx; i<=endx; ++i)
            {
                if (mDataMaker->intersectEdgeInGrid(i, y0,
                    startPos, endPos))
                    return false;
            }

            return true;
        }

        float dx,dy,m;

        dx = (float)(x1 - x0);
        dy = (float)(y1 - y0);

        float xlength = abs(dx);
        float ylength = abs(dy);

        if (ylength > xlength)
            m = dx / dy;
        else
            m = dy /dx;

        int startIndex,endIndex;
        float coord;

        if (xlength > ylength)
        {
            startIndex = startx;
            endIndex = endx;
            coord = x1 > x0 ? (float)(y0) : (float)(y1);
        }
        else
        {
            startIndex = starty;
            endIndex = endy;
            coord = y1 > y0 ? (float)(x0) : (float)(x1);
        }

        int coord1, coord2;
        for (int i=startIndex; i<=endIndex; ++i)
        {
            if (xlength > ylength)
            {
                coord1 = i;
                coord2 = fast_ifloor(coord + 0.5f);

            }
            else
            {
                coord1 = fast_ifloor(coord + 0.5f);
                coord2 = i;

            }

            if (mDataMaker->intersectEdgeInGrid(coord1, coord2,
                startPos, endPos))
                return false;

            if (xlength > ylength)
            {
                if (mDataMaker->isValidGrid(coord1, coord2-1))
                {
                    if (mDataMaker->intersectEdgeInGrid(coord1, coord2-1,
                        startPos, endPos))
                        return false;
                }

                if (mDataMaker->isValidGrid(coord1, coord2+1))
                {
                    if (mDataMaker->intersectEdgeInGrid(coord1, coord2+1,
                        startPos, endPos))
                        return false;
                }
            }
            else
            {
                if (mDataMaker->isValidGrid(coord1-1, coord2))
                {
                    if (mDataMaker->intersectEdgeInGrid(coord1-1, coord2,
                        startPos, endPos))
                        return false;
                }

                if (mDataMaker->isValidGrid(coord1+1, coord2))
                {
                    if (mDataMaker->intersectEdgeInGrid(coord1+1, coord2,
                        startPos, endPos))
                        return false;
                }
            }


            coord += m;
        }

        return true;
    }
    //-----------------------------------------------------------------------
    bool PathFinderImpl::isPointInRegion(float x, float z, uint& triangleID, char obstacle)
    {
        // 找出所在的格子
        std::pair<int, int> gridIndex = mDataMaker->getGrid(x, z);

        const Grid& grid = mDataMaker->getGrid(gridIndex.first, gridIndex.second);

        for (size_t triIndex = 0; triIndex < grid.mTriangleIDList.size(); ++triIndex)
        {
            const Triangle& triangle = mDataMaker->getTriangle( grid.mTriangleIDList[triIndex] );

            if (obstacle)
            {   
                if (triangle.getObstacle())
                {
                    if ( Math::pointInTri2D(Vector2(x, z), triangle.pos1, triangle.pos2, triangle.pos3) )
                    {
                        triangleID = triangle.id;
                        return true;
                    }
                }
            }
            else
            {
                if (false == triangle.getObstacle())
                {
                    if ( Math::pointInTri2D(Vector2(x, z), triangle.pos1, triangle.pos2, triangle.pos3) )
                    {
                        triangleID = triangle.id;
                        return true;
                    }
                }
            }
        }

        return false;
    }
    //-----------------------------------------------------------------------
    bool PathFinderImpl::intersectEdgeInGrid(IntersectPoints& outputPoints, int x, int z, Vector2 startPos, Vector2 endPos) const
    {
        bool intersect = false;

        const Grid& grid = mDataMaker->getGrid(x, z);

        for (size_t edgeIndex = 0; edgeIndex < grid.mEdgeIDList.size(); ++edgeIndex)
        {
            const Edge& edge = mDataMaker->getEdge( grid.mEdgeIDList[edgeIndex] );
                    
            float xcross, yxross;
            if (intersectWith(startPos, endPos, edge.mPoint1, edge.mPoint2, xcross, yxross))
            {   
                IntersectPoint intersectPoint;
                intersectPoint.intersectPos.x = xcross;
                intersectPoint.intersectPos.y = yxross;

                ushort tri1ID = edge.triangleID[0];

                if (tri1ID != INVALID_TRI_INDEX)
                {
                    const Triangle& triangle1 = mTriangleList[tri1ID];

                    if (triangle1.getObstacle())
                    {
                        intersectPoint.flag = triangle1.mFlag;
                    }

                }
                ushort tri2ID = edge.triangleID[1];

                if (tri2ID != INVALID_TRI_INDEX)
                {
                    const Triangle& triangle2 = mTriangleList[tri2ID];

                    if (triangle2.getObstacle())
                    {
                        intersectPoint.flag = triangle2.mFlag;
                    }
                }

                outputPoints.insert( IntersectPoints::value_type( 
                    Math::squareDistance(startPos, intersectPoint.intersectPos), intersectPoint) );
                intersect = true;
            }
        } 

        return intersect;
    }
    //-----------------------------------------------------------------------
    bool PathFinderImpl::intersectTriangleInGrid(IntersectPoints& outputPoints, int x, int z, Vector2 startPos, Vector2 endPos, char isObstacle) const
    {
        //LogManager::getSingleton().logMessage("check triangle : " + StringConverter::toString(x) + " "
        //    + StringConverter::toString(z));

        bool intersect = false;

        const Grid& grid = mDataMaker->getGrid(x, z);

        for (size_t triIndex = 0; triIndex < grid.mTriangleIDList.size(); ++triIndex)
        {
            const Triangle& triangle = mTriangleList[ grid.mTriangleIDList[triIndex] ];

            if (isObstacle)
            {
                if (triangle.getObstacle())
                {
                    float xcorss, ycorss;

                    if (intersectWith(startPos, endPos, triangle.pos1, triangle.pos2, xcorss, ycorss))
                    {
                        IntersectPoint intersectPoint;
                        intersectPoint.flag = triangle.mFlag;
                        intersectPoint.intersectPos.x = xcorss;
                        intersectPoint.intersectPos.y = ycorss;

                        outputPoints.insert( IntersectPoints::value_type( 
                            Math::squareDistance(startPos, intersectPoint.intersectPos), intersectPoint) );

                        intersect = true;
                    }
                    if (intersectWith(startPos, endPos, triangle.pos2, triangle.pos3, xcorss, ycorss))
                    {
                        IntersectPoint intersectPoint;
                        intersectPoint.flag = triangle.mFlag;
                        intersectPoint.intersectPos.x = xcorss;
                        intersectPoint.intersectPos.y = ycorss;

                        outputPoints.insert( IntersectPoints::value_type( 
                            Math::squareDistance(startPos, intersectPoint.intersectPos), intersectPoint) );
                        intersect = true;

                    }
                    if (intersectWith(startPos, endPos, triangle.pos1, triangle.pos3, xcorss, ycorss))
                    {
                        IntersectPoint intersectPoint;
                        intersectPoint.flag = triangle.mFlag;
                        intersectPoint.intersectPos.x = xcorss;
                        intersectPoint.intersectPos.y = ycorss;

                        outputPoints.insert( IntersectPoints::value_type( 
                            Math::squareDistance(startPos, intersectPoint.intersectPos), intersectPoint) );
                        intersect = true;

                    }
                }
            }
            else
            {
                if (false == triangle.getObstacle())
                {
                    float xcorss, ycorss;

                    if (intersectWith(startPos, endPos, triangle.pos1, triangle.pos2, xcorss, ycorss))
                    {
                        IntersectPoint intersectPoint;
                        intersectPoint.flag = triangle.mFlag;
                        intersectPoint.intersectPos.x = xcorss;
                        intersectPoint.intersectPos.y = ycorss;

                        outputPoints.insert( IntersectPoints::value_type( 
                            Math::squareDistance(startPos, intersectPoint.intersectPos), intersectPoint) );
                        intersect = true;

                    }
                    if (intersectWith(startPos, endPos, triangle.pos2, triangle.pos3, xcorss, ycorss))
                    {
                        IntersectPoint intersectPoint;
                        intersectPoint.flag = triangle.mFlag;
                        intersectPoint.intersectPos.x = xcorss;
                        intersectPoint.intersectPos.y = ycorss;

                        outputPoints.insert( IntersectPoints::value_type( 
                            Math::squareDistance(startPos, intersectPoint.intersectPos), intersectPoint) );
                        intersect = true;

                    }
                    if (intersectWith(startPos, endPos, triangle.pos1, triangle.pos3, xcorss, ycorss))
                    {
                        IntersectPoint intersectPoint;
                        intersectPoint.flag = triangle.mFlag;
                        intersectPoint.intersectPos.x = xcorss;
                        intersectPoint.intersectPos.y = ycorss;

                        outputPoints.insert( IntersectPoints::value_type( 
                            Math::squareDistance(startPos, intersectPoint.intersectPos), intersectPoint) );
                        intersect = true;

                    }
                }
            }
        } 

        return intersect;
    }
}