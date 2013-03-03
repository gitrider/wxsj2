#include "TrueSmoothHeightAction.h"
#include "SceneManipulator.h"

namespace Fairy {




    TrueSmoothHeightAction::TrueSmoothHeightAction(SceneManipulator* sceneManipulator)
        : TimeBasedHeightAction(sceneManipulator)
    {
    }

    const String& TrueSmoothHeightAction::getName(void) const
    {
        static const String name = "TrueSmoothHeightAction";
        return name;
    }

    void TrueSmoothHeightAction::_onBegin(const Point& pt)
    {
        TimeBasedHeightAction::_onBegin(pt);

        computeAverageHeight();
    }

    void TrueSmoothHeightAction::_onDrag(const Point& pt)
    {
        TimeBasedHeightAction::_onDrag(pt);

        computeAverageHeight();
    }

    void TrueSmoothHeightAction::computeAverageHeight(void)
    {

        Real totalHeight = 0;
        Real totalWeight = 0;

        JunctionSelection* selection = static_cast<JunctionSelection*>( getSceneManipulator()->_getSelection("JunctionSelection"));
        const JunctionSelection::JunctionMap& junctions = selection->getJunctions();
        JunctionSelection::JunctionMap::const_iterator it;
        for (it = junctions.begin(); it != junctions.end(); ++it)
        {
            const JunctionSelection::Junction& junction = it->second;
            totalWeight += junction.weight;
            totalHeight += getTerrainData()->getHeight(junction.x, junction.z) * junction.weight;

			if(it == junctions.begin())
			{
				MinBindPoint = std::pair<int, int>(junction.x,junction.z);
				MaxBindPoint = std::pair<int, int>(junction.x,junction.z);
			}else
			{
				MinBindPoint.first = MinBindPoint.first < junction.x ? MinBindPoint.first :  junction.x  ; 
				MinBindPoint.second = MinBindPoint.second < junction.z ? MinBindPoint.second :  junction.z  ; 
				MaxBindPoint.first = MaxBindPoint.first > junction.x ? MaxBindPoint.first :  junction.x  ; 
				MaxBindPoint.second = MaxBindPoint.second > junction.z ? MaxBindPoint.second :  junction.z  ; 
			}
        }
			 mWeightcnt=MaxBindPoint.first-MinBindPoint.first+1;
			 mHeightcnt=MaxBindPoint.second-MinBindPoint.second+1;

        mAverageHeight = totalWeight ? totalHeight / totalWeight : 0;
    }

    Real TrueSmoothHeightAction::_computeHeight(const JunctionSelection::Junction& junction, Real seconds)
    {

		float center_x = (MaxBindPoint.first - MinBindPoint.first ) /2 + MinBindPoint.first;
		float center_z = (MaxBindPoint.second- MinBindPoint.second) /2 + MinBindPoint.second;

		//¼ÆËãÈ¨ÖØ
	     Real height = getTerrainData()->getHeight(junction.x, junction.z);
		 float StrenghScalar = RadiusStrength(Ogre::Vector3(center_x,0,center_z),Ogre::Vector3(junction.x ,0,junction.z) ,0.1,(mWeightcnt+mHeightcnt)/4) ;

		float SmoothHeight=0;
		int   vertCnt = 0;
		for (int AdjacentZ = 0;AdjacentZ < 3;AdjacentZ++)
		{
			for (int AdjacentX = 0;AdjacentX < 3;AdjacentX++)
			{
				int VertX = junction.x - 1 + AdjacentX;
				int VertZ = junction.z - 1 + AdjacentZ;

				// prevent array out of bounds crash if X and Y are referencing a vertex on the edge of the map
				if ( false == getTerrainData()->isValidJunction(VertX, VertZ) )
				continue;
				SmoothHeight += getTerrainData()->getHeight(VertX, VertZ) ;
				vertCnt++;
			}
		}
		SmoothHeight=SmoothHeight/vertCnt;
		float Newheight = Lerp(height,SmoothHeight,std::min(StrenghScalar,1.0f)) ;
       Real diff = Newheight - height;
	   Newheight = height +   diff *seconds*getSceneManipulator()->getHeightAdjustSpeed() ;
	   return Newheight;

    }

}
