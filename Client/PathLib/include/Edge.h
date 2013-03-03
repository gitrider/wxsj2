#ifndef __Edge_H__
#define __Edge_H__

#include "PathLibType.h"

namespace PathLib
{
    struct Edge
    {
        Edge()
        {

        }

        Edge(const Vector2& p1, const Vector2& p2, ushort id)
            : mPoint1(p1)
            , mPoint2(p2)
            , mID(id)
        {
            vertexID[0] = INVALID_VERTEX_INDEX;
            vertexID[1] = INVALID_VERTEX_INDEX;

            triangleID[0] = INVALID_TRI_INDEX;
            triangleID[1] = INVALID_TRI_INDEX;
        }

        ushort mID;
        Vector2 mPoint1;
        Vector2 mPoint2;

        ushort vertexID[2];
        ushort triangleID[2];
    };
}

#endif
