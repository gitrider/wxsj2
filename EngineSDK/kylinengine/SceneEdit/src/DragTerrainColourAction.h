#ifndef __DragTerrainColourAction_H__
#define __DragTerrainColourAction_H__
#include "Action.h"
#include "TerrainSelections.h"
#include "HitInfo.h"
#include <OgreVector2.h>  

namespace Fairy {	

	class DragTerrainColourAction : public Action
	{
	public:
		DragTerrainColourAction(SceneManipulator* sceneManipulator);

		const String& getName(void) const;

		void setParameter(const String& name, const String& value);
		enum ColourBrushType
		{
			CBT_TERRAIN_DIFFUSE,
			CBT_OBJECT_DIFFUSE,
		};
		
		enum ActionState
		{
			AS_BEGIN,
			AS_ACTIONING,
			AS_END,
		};

	protected:
		HitInfo mTerrainHitInfo;
		
		typedef Ogre::Vector2 Point;
		struct PointLess
		{
			bool operator()(const Point& v1, const Point& v2) const
			{
				if (v1.x < v2.x) return true;
				if (v1.x == v2.x && v1.y < v2.y) return true;

				return false;
			}
		};
		typedef std::map<Point, Ogre::ColourValue, PointLess> DiffuseInfoMap;
		DiffuseInfoMap mModifiedMap;

		ColourBrushType mBrushType;
		ActionState mActionState;
		typedef std::map<Ogre::String, Ogre::ColourValue> ModifiedObjectMap;

		ModifiedObjectMap mModifiedObjects;

		void _onMove(const Point& pt);
		void _onBegin(const Point& pt);
		void _onDrag(const Point& pt);
		void _onUpdate(const Point& pt, Real seconds);
		void _onEnd(const Point& pt, bool canceled);
		void _doPaint();

		virtual void _onActive(bool active);


		void doFinish(JunctionSelection* origin, bool canceled);
	};

}

#endif // __DragTerrainColourAction_H__