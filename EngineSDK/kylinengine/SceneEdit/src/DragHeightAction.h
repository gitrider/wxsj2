#ifndef __DragHeightAction_H__
#define __DragHeightAction_H__

#include "HeightAction.h"
#include "HitInfo.h"

namespace Fairy {

    class DragHeightAction : public HeightAction
    {
    public:
        DragHeightAction(SceneManipulator* sceneManipulator);

        const String& getName(void) const;

    protected:
        HitInfo mTerrainHitInfo;

        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);
    };

}

#endif // __DragHeightAction_H__
