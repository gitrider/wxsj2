#ifndef __TrueSmoothHeightAction_H__
#define __TrueSmoothHeightAction_H__

#include "TimeBasedHeightAction.h"

namespace Fairy {

    class JunctionSelection;

	 //////////////////////////////////////////////////////////////////////////
	/**
	 * Determines how strongly the editing circles affect a vertex.
	 *
	 * @param	ToolCenter		The location being pointed at on the terrain
	 * @param	Vertex			The vertex being affected
	 * @param	MinRadius		The outer edge of the inner circle
	 * @param	MaxRadius		The outer edge of the outer circle
	 *
	 * @return	A vaue between 0-1, representing how strongly the tool should affect the vertex.
	 */


	inline float RadiusStrength(const Ogre::Vector3& ToolCenter,const Ogre::Vector3& Vertex,float MinRadius,float MaxRadius)
	{
		float	Distance = (Vertex - ToolCenter).length();
		if (Distance <= MinRadius)
		{
			return 1.0f;
		}
		else if (Distance < MaxRadius)
		{
			return (1.0f - (Distance - MinRadius) / (MaxRadius - MinRadius));
		}
		else
		{
			return 0.0f;
		}
	}


	template< class T, class U > T Lerp( const T& A, const T& B, const U& Alpha )
	{
		return (T)(A + Alpha * (B-A));
	}

    class TrueSmoothHeightAction : public TimeBasedHeightAction
    {
    public:
        TrueSmoothHeightAction(SceneManipulator* sceneManipulator);

        const String& getName(void) const;

    protected:
        Real mAverageHeight;
		std::pair<int, int> MinBindPoint ;
		std::pair<int, int> MaxBindPoint ;
		float mWeightcnt;
        float mHeightcnt; 
        void computeAverageHeight(void);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        Real _computeHeight(const JunctionSelection::Junction& junction, Real seconds);
    };

}

#endif // __SmoothHeightAction_H__
