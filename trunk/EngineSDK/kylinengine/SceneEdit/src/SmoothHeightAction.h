#ifndef __SmoothHeightAction_H__
#define __SmoothHeightAction_H__

#include "TimeBasedHeightAction.h"

namespace Fairy {

    class JunctionSelection;

    class SmoothHeightAction : public TimeBasedHeightAction
    {
    public:
		enum ENUM_SMOOTH_TYPE
		{
			SMOOTH_TYPE_TYPE_INVALID = -1,
			SMOOTH_TYPE_TYPE_SMOOTH,			// 有阶梯效果的坡面
			SMOOTH_TYPE_TYPE_TRISMOOTH,					// 消平坡面
			SMOOTH_TYPE_TYPE_NUMBERS
		};
        SmoothHeightAction(SceneManipulator* sceneManipulator);
		void setTypeSelecting(ENUM_SMOOTH_TYPE type);
        const String& getName(void) const;

    protected:
        Real mAverageHeight;
		Real mMaxHeight;
		Real mMinHeight;
		int  mSmoothType;

		std::pair<int, int> MinBindPoint ;
		std::pair<int, int> MaxBindPoint ;
		bool ScalarX_axis;
        void computeAverageHeight(void);

        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        Real _computeHeight(const JunctionSelection::Junction& junction, Real seconds);
    };

}

#endif // __SmoothHeightAction_H__
