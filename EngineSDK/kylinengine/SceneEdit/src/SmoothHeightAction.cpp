#include "SmoothHeightAction.h"
#include "SceneManipulator.h"
#include <windows.h>
namespace Fairy {

    //////////////////////////////////////////////////////////////////////////
	template< class T, class U > T Lerp( const T& A, const T& B, const U& Alpha )
	{
		return (T)(A + Alpha * (B-A));
	}

    SmoothHeightAction::SmoothHeightAction(SceneManipulator* sceneManipulator)
        : TimeBasedHeightAction(sceneManipulator)
    {

		if ((::GetKeyState(VK_CAPITAL) & 0x0001) != 0)  //根据CAP判断类型
			mSmoothType = ENUM_SMOOTH_TYPE::SMOOTH_TYPE_TYPE_TRISMOOTH ;
		else
			mSmoothType = ENUM_SMOOTH_TYPE::SMOOTH_TYPE_TYPE_SMOOTH ;
    }

	void SmoothHeightAction::setTypeSelecting(ENUM_SMOOTH_TYPE type)
	{
		mSmoothType = type;
	}

    const String& SmoothHeightAction::getName(void) const
    {
        static const String name = "SmoothHeightAction";
        return name;
    }

    void SmoothHeightAction::_onBegin(const Point& pt)
    {
        TimeBasedHeightAction::_onBegin(pt);

        computeAverageHeight();
    }

    void SmoothHeightAction::_onDrag(const Point& pt)
    {
        TimeBasedHeightAction::_onDrag(pt);

		if (	mSmoothType == ENUM_SMOOTH_TYPE::SMOOTH_TYPE_TYPE_SMOOTH )
			computeAverageHeight();
    }

    void SmoothHeightAction::computeAverageHeight(void)
    {
        Real totalHeight = 0;
        Real totalWeight = 0;
		mMaxHeight = -1;
		mMinHeight = 100000.0f;

		MinBindPoint.first  = 500000.0f;
		MinBindPoint.second = 500000.0f;

		MaxBindPoint.first  = -1.0f;
		MaxBindPoint.second = -1.0f;

        JunctionSelection* selection = static_cast<JunctionSelection*>(
            getSceneManipulator()->_getSelection("JunctionSelection"));
        const JunctionSelection::JunctionMap& junctions = selection->getJunctions();
		if (junctions.size()<=0)
		{
			return;
		}

        JunctionSelection::JunctionMap::const_iterator it;
        for (it = junctions.begin(); it != junctions.end(); ++it)
        {
            const JunctionSelection::Junction& junction = it->second;
            totalWeight += junction.weight;
            totalHeight += getTerrainData()->getHeight(junction.x, junction.z) * junction.weight;

			mMaxHeight = getTerrainData()->getHeight(junction.x, junction.z) > mMaxHeight ? getTerrainData()->getHeight(junction.x, junction.z) :  mMaxHeight ; 
			mMinHeight = getTerrainData()->getHeight(junction.x, junction.z) < mMinHeight ? getTerrainData()->getHeight(junction.x, junction.z) :  mMinHeight ;

			MinBindPoint.first = MinBindPoint.first < junction.x ? MinBindPoint.first	 :  junction.x  ; 
			MinBindPoint.second = MinBindPoint.second < junction.z ? MinBindPoint.second :  junction.z  ; 
			MaxBindPoint.first = MaxBindPoint.first > junction.x ? MaxBindPoint.first	 :  junction.x  ; 
			MaxBindPoint.second = MaxBindPoint.second > junction.z ? MaxBindPoint.second :  junction.z  ; 
        }
        mAverageHeight = totalWeight ? totalHeight / totalWeight : 0;

		//----------------------
		//  lefttop					right top
		//  min.x max.y  ---------- max.x max.y
		//   |							|
		//   |							|
		//   |							|
		//   min.x min.y-----------max.x min.y

		float leftdown_h =  getTerrainData()->getHeight(MinBindPoint.first, MinBindPoint.second);
		float righttop_h =  getTerrainData()->getHeight(MaxBindPoint.first, MaxBindPoint.second);
		float lefttop_h =   getTerrainData()->getHeight(MinBindPoint.first, MaxBindPoint.second);
		float rightdown_h =  getTerrainData()->getHeight(MaxBindPoint.first, MinBindPoint.second);

		if (	abs(righttop_h-lefttop_h)   > 	abs(righttop_h-rightdown_h))
			ScalarX_axis = true;
		else
			ScalarX_axis = false;
	
	
    }

    Real SmoothHeightAction::_computeHeight(const JunctionSelection::Junction& junction, Real seconds)
    {
        Real height = getTerrainData()->getHeight(junction.x, junction.z);

		if (	mSmoothType == ENUM_SMOOTH_TYPE::SMOOTH_TYPE_TYPE_SMOOTH )
		{
	
		Real diff = mAverageHeight - height;
		Real secondsRequest = Ogre::Math::Abs(diff * junction.weight / getSceneManipulator()->_getHeightAdjustSpeed());
		if (secondsRequest < seconds)
			return mAverageHeight;
		else
			return height + diff * seconds / secondsRequest;
		}


		float StrenghScalar;
		if(ScalarX_axis)
			StrenghScalar = (float)(junction.x - MinBindPoint.first) / (float)(MaxBindPoint.first - MinBindPoint.first);
		else
			StrenghScalar = (float)(junction.z - MinBindPoint.second) / (float)(MaxBindPoint.second - MinBindPoint.second);

		if (StrenghScalar < 0.0f)
			StrenghScalar = 0.0f;

		float  minValue  =  mMaxHeight  ;
		float  maxValue  =  mMinHeight  ;
		float Newheight ;
		if (  getTerrainData()->getHeight(MaxBindPoint.first, MaxBindPoint.second)  <   getTerrainData()->getHeight(MinBindPoint.first, MinBindPoint.second)  )
			Newheight = Lerp(minValue,maxValue,min(StrenghScalar,1.0f)) ;
		else
			Newheight = Lerp(maxValue,minValue,min(StrenghScalar,1.0f)) ;

		Real diff = Newheight - height;
		Newheight = height +   diff *seconds*getSceneManipulator()->getHeightAdjustSpeed() ;
		return Newheight;

    }

}
