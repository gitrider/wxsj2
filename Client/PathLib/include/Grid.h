#ifndef __Grid_H__
#define __Grid_H__

#include "PathLibType.h"
#include "MathLib.h"

#include "Triangle.h"


namespace PathLib
{
    typedef std::vector<uint> TriangleIDList;
    typedef std::vector<uint> EdgeIDList;

    class Grid
    {
    public:

        Grid()
            : mMinPos(Vector2(0.0f, 0.0f))
            , mMaxPos(Vector2(0.0f, 0.0f))
            , mXIndex(0)
            , mZIndex(0)
        {
            mTriangleIDList.clear();
            mEdgeIDList.clear();
        }

        inline bool operator == (const Grid& b) const
        {
            return (mXIndex == b.mXIndex) && (mZIndex == b.mZIndex);
        }

    public:

        Vector2 mMinPos;
        Vector2 mMaxPos;

        TriangleIDList mTriangleIDList;
        EdgeIDList mEdgeIDList;

        int mXIndex;
        int mZIndex;
    };
}

#endif