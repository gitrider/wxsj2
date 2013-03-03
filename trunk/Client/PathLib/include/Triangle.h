#ifndef __Triangle_H__
#define __Triangle_H__

#include "PathLibType.h"
#include "MathLib.h"

#include "CDT.h"

namespace PathLib
{
    const unsigned short INVALID_TRI_INDEX = 65535;
    const unsigned short INVALID_VERTEX_INDEX = 65535;

    struct Triangle
    {
        Triangle()
            : minPos(Vector2(0.0f, 0.0f))
            , maxPos(Vector2(0.0f, 0.0f))
            , mFlag(0)
            , mIslandID(0)
        {
            id = INVALID_TRI_INDEX;
            neighborId[0] = INVALID_TRI_INDEX;
            neighborId[1] = INVALID_TRI_INDEX;
            neighborId[2] = INVALID_TRI_INDEX;

            vertexID[0] = INVALID_VERTEX_INDEX;
            vertexID[1] = INVALID_VERTEX_INDEX;
            vertexID[2] = INVALID_VERTEX_INDEX;
        }

        void calBoundingBox(void)
        {
            float minX = Math::POS_INFINITY;
            float minZ = Math::POS_INFINITY;
            float maxX = Math::NEG_INFINITY;
            float maxZ = Math::NEG_INFINITY;

            if (pos1.x > maxX)
                maxX = pos1.x;
            if (pos2.x > maxX)
                maxX = pos2.x;
            if (pos3.x > maxX)
                maxX = pos3.x;

            if (pos1.y > maxZ)
                maxZ = pos1.y;
            if (pos2.y > maxZ)
                maxZ = pos2.y;
            if (pos3.y > maxZ)
                maxZ = pos3.y;

            if (pos1.x < minX)
                minX = pos1.x;
            if (pos2.x < minX)
                minX = pos2.x;
            if (pos3.x < minX)
                minX = pos3.x;

            if (pos1.y < minZ)
                minZ = pos1.y;
            if (pos2.y < minZ)
                minZ = pos2.y;
            if (pos3.y < minZ)
                minZ = pos3.y;

            minPos.x = minX;
            minPos.y = minZ;
            maxPos.x = maxX;
            maxPos.y = maxZ;
        }

        inline void setObstacle(bool obstacle)
        {
            obstacle ? mFlag |= 0x1 : mFlag &= 0xfe;
        }

        inline bool getObstacle(void) const
        {
            return (mFlag & 0x1) != 0;
        }

        inline void setFlyable(bool flyable)
        {
            flyable ? mFlag |= 0x2 : mFlag &= 0xfd;
        }

        inline bool getFlyable(void) const
        {
            return (mFlag & 0x2) != 0;
        }


        ushort id;
        Vector2 pos1;
        Vector2 pos2;
        Vector2 pos3;

        Vector2 minPos;
        Vector2 maxPos;

        ushort vertexID[3];
        ushort neighborId[3];

        CGALCDT::Face_handle cgalFaceHandle;

        Vector2 midPoint;
        // 三角形的flag，8位，第0位表示是否为阻挡，第1位表示是否为可飞行区域
        char mFlag; 
        // 三角形所属的island的id，同个连通区域属于同个island
        ushort mIslandID;
    };
}

#endif