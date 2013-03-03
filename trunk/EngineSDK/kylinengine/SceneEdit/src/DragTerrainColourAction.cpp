#include "DragTerrainColourAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "TerrainSelections.h"

#include "WXModifyObjectPropertyOperator.h"
#include "WXModifyTerrainDiffuseOperator.h"
#include "WXOperatorManager.h"

#include "Core/FairyStaticEntityObject.h"
#include "Core/FairySceneInfo.h"

namespace Fairy {

	//////////////////////////////////////////////////////////////////////////

	DragTerrainColourAction::DragTerrainColourAction(SceneManipulator* sceneManipulator)
		: Action(sceneManipulator)
	{
		mBrushType = CBT_TERRAIN_DIFFUSE;	
		mActionState = AS_END;
 	}

	const String& DragTerrainColourAction::getName(void) const
	{
		static const String name = "DragTerrainColourAction";
		return name;
	}

	void DragTerrainColourAction::setParameter(const String& name, const String& value)
	{
		if (name == "switchType")
		{
			if (value == "TerrainDiffuse")
			{
				mBrushType = CBT_TERRAIN_DIFFUSE;
				mSceneManipulator->_fireShowHintChanged(false);
			}
			else if ("ObjectDiffuse")
			{
				mBrushType = CBT_OBJECT_DIFFUSE;
				mSceneManipulator->_fireShowHintChanged(true);
			}
		}
	}

	void DragTerrainColourAction::_onActive(bool active)
	{
		if (!active)
		{
			getSceneManipulator()->getHitIndicator("JunctionPoints")->hide();
			mSceneManipulator->_fireShowHintChanged(false);
		}
		else
		{
			mSceneManipulator->_fireShowHintChanged(mBrushType == CBT_OBJECT_DIFFUSE);
		}
	}
	//-----------------------------------------------------------------------
	void DragTerrainColourAction::doFinish(JunctionSelection* origin, bool canceled)
	{
		if (mBrushType == CBT_TERRAIN_DIFFUSE)
		{
			if (canceled)
			{

				if (mModifiedMap.empty())
				{
					return;
				}
				else
				{		
					int minX,minZ,maxX,maxZ;
					DiffuseInfoMap::iterator iter = mModifiedMap.begin();
					minX = maxX = iter->first.x;
					minZ = maxZ = iter->first.y;
					while (iter != mModifiedMap.end())
					{
						Point grid = iter->first;
						Ogre::ColourValue oldColor = iter->second;

						if (minX > grid.x)
							minX = grid.x;
						else if (maxX < grid.x)
							maxX = grid.x;

						if (minZ > grid.y)
							minZ = grid.y;
						else if (maxZ < grid.y)
							maxZ = grid.y;					

						getTerrainData()->setDiffuseInfo(grid.x, grid.y, oldColor);
						++iter;					
					}				

					getTerrain()->notifyColourModified(minX, minZ, maxX, maxZ);
				}
			}
			else
			{
				if (!mModifiedMap.empty())
				{
					std::auto_ptr<ModifyTerrainDiffuseOperator> op(new ModifyTerrainDiffuseOperator(getSceneManipulator()));
					for (DiffuseInfoMap::iterator iter = mModifiedMap.begin();iter != mModifiedMap.end(); ++iter)
					{
						Point grid = iter->first;
						Ogre::ColourValue oldColor = iter->second;

						const Ogre::ColourValue &newDiffuse = getTerrainData()->getDiffuseInfo(grid.x, grid.y);
						const Ogre::ColourValue &oldDiffuse = oldColor;

						if (newDiffuse != oldDiffuse)
						{
							op->add(grid.x, grid.y, oldDiffuse, newDiffuse);						
						}
					}				

					if (!op->empty())
					{
						getOperatorManager()->addOperator(op.release());
					}
				}
			}
		}
		else if (mBrushType == CBT_OBJECT_DIFFUSE)
		{
			if (canceled)
			{

				if (mModifiedObjects.empty())
				{
					return;
				}
				else
				{	
					for (ModifiedObjectMap::iterator it = mModifiedObjects.begin(); it != mModifiedObjects.end(); ++it)
					{
						Fairy::ObjectPtr object = getSceneManipulator()->getSceneInfo()->findObjectByName(it->first);
						if (object)
						{
							Ogre::String type = object->getType();
							if(Fairy::StaticEntityObject::msType == type)
							{
								Fairy::StaticEntityObject *staticEntityObject = static_cast<StaticEntityObject *> (object.get());
								staticEntityObject->setAdditionalDiffuse(it->second);
							}

							getSceneManipulator()->_fireObjectPropertyChanged(object, "additional diffuse");
						}					
					}				
				}
			}
			else
			{
				if (!mModifiedObjects.empty())
				{
					std::auto_ptr<ModifyObjectPropertyOperator> op(new ModifyObjectPropertyOperator(getSceneManipulator()));

					for (ModifiedObjectMap::iterator it = mModifiedObjects.begin(); it != mModifiedObjects.end(); ++it)
					{
						Fairy::ObjectPtr object = getSceneManipulator()->getSceneInfo()->findObjectByName(it->first);
						if (object)
						{
							Ogre::String type = object->getType();
							if(Fairy::StaticEntityObject::msType == type)
							{
								Fairy::StaticEntityObject *staticEntityObject = static_cast<StaticEntityObject *> (object.get());
								Ogre::ColourValue newDiffuse = staticEntityObject->getAdditionalDiffuse();
								Ogre::ColourValue oldDiffuse = it->second;
								if (newDiffuse != oldDiffuse)
								{
									op->add(object->getName(), "additional diffuse", oldDiffuse, newDiffuse);
								}
							}
						}
					}							

					if (!op->empty())
					{
						getOperatorManager()->addOperator(op.release());
					}
				}
			}
		}
		
	}

	void DragTerrainColourAction::_onMove(const Point& pt)
	{
		getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
		getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
	}

	void DragTerrainColourAction::_onBegin(const Point& pt)
	{
		getSceneManipulator()->makeTerrainHitInfo(mTerrainHitInfo, pt);
		getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
		getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);

		mModifiedMap.clear();

		if (!mTerrainHitInfo.hitted)
			return;

		mActionState = AS_BEGIN;

		_doPaint();
	}

	void DragTerrainColourAction::_onDrag(const Point& pt)
	{
		 getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
		 getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
		 _doPaint();
	}

	void DragTerrainColourAction::_onUpdate(const Point& pt, Real seconds)
	{
		JunctionSelection* selection = static_cast<JunctionSelection*>(
			getSceneManipulator()->_getSelection("JunctionSelection"));
		const JunctionSelection::JunctionMap& junctions = selection->getJunctions();
		if ((int)junctions.size() <=0)
		{			
			return;
		}	

		if (mActionState != AS_END && mBrushType == CBT_OBJECT_DIFFUSE)
		{
			JunctionSelection::JunctionMap::const_iterator it;

			std::pair<int, int> MinBindPoint ;
			std::pair<int, int> MaxBindPoint ;

			for (it = junctions.begin(); it != junctions.end(); ++it)
			{
				const JunctionSelection::Junction& junction = it->second;
				if(it == junctions.begin())
				{
					MinBindPoint = std::pair<int, int>(junction.x,junction.z);
					MaxBindPoint = std::pair<int, int>(junction.x,junction.z);
				}
				else
				{
					MinBindPoint.first = MinBindPoint.first < junction.x ? MinBindPoint.first :  junction.x  ; 
					MinBindPoint.second = MinBindPoint.second < junction.z ? MinBindPoint.second :  junction.z  ; 
					MaxBindPoint.first = MaxBindPoint.first > junction.x ? MaxBindPoint.first :  junction.x  ; 
					MaxBindPoint.second = MaxBindPoint.second > junction.z ? MaxBindPoint.second :  junction.z  ; 
				}
			}

			Ogre::ColourValue targetColour =  getSceneManipulator()->getDiffuseColour();
			Ogre::ColourValue diff = Ogre::ColourValue::White -  getSceneManipulator()->getDiffuseColour();
			Ogre::ColourValue delta = diff *seconds*getSceneManipulator()->getHeightAdjustSpeed();

			if (mActionState == AS_BEGIN)
			{
				mModifiedObjects.clear();
			}

			for (int z = MinBindPoint.second; z<= MaxBindPoint.second; ++z)
			{
				for (int x = MinBindPoint.first; x<= MaxBindPoint.first; ++x)
				{
					Ogre::Vector2 point(x, z);
					if (mSceneManipulator->mGridObjsMap.find(point) != mSceneManipulator->mGridObjsMap.end())
					{
						Fairy::SceneManipulator::GridObjsMap::iterator  iter = mSceneManipulator->mGridObjsMap.find(point);
						Fairy::SceneManipulator::StaticObjects& vec = iter->second;
						for (Fairy::SceneManipulator::StaticObjects::iterator obj = vec.begin(); obj!= vec.end(); ++obj)
						{
							Fairy::StaticEntityObject* staticEntity = *obj;
							Ogre::ColourValue preColour = staticEntity->getAdditionalDiffuse();
							Ogre::ColourValue newColour = Ogre::ColourValue::White;
							
							if (mModifiedObjects.find(staticEntity->getName()) == mModifiedObjects.end())
							{
								mModifiedObjects.insert(ModifiedObjectMap::value_type(staticEntity->getName(), preColour));

								newColour = mSceneManipulator->getClearFlag()?( preColour + delta):(newColour - delta);									
							}
							else
							{									
								newColour = mSceneManipulator->getClearFlag()?(preColour + delta):(preColour -delta);								
							}
							
							if (!mSceneManipulator->getClearFlag())
							{
								if ( newColour.r < targetColour.r ||
									newColour.g < targetColour.g ||
									newColour.b < targetColour.b )
								{
									newColour = targetColour;
								}
							}

							newColour.saturate();
							staticEntity->setAdditionalDiffuse(newColour);
							Fairy::ObjectPtr object = getSceneManipulator()->getSceneInfo()->findObjectByName(staticEntity->getName());
							getSceneManipulator()->_fireObjectPropertyChanged(object, "additional diffuse");
						}
					}
				}
			}
		}

		if (mActionState == AS_BEGIN)
		{
			mActionState = AS_ACTIONING;
		}
	}

	void DragTerrainColourAction::_doPaint()
	{
		JunctionSelection* selection = static_cast<JunctionSelection*>(
			getSceneManipulator()->_getSelection("JunctionSelection"));
		const JunctionSelection::JunctionMap& junctions = selection->getJunctions();
		if ((int)junctions.size() <=0)
		{			
			return;
		}

		if (mBrushType == CBT_TERRAIN_DIFFUSE)
		{
			Ogre::ColourValue color(getSceneManipulator()->getDiffuseColour());
			JunctionSelection::JunctionMap::const_iterator it;

			for (it = junctions.begin(); it != junctions.end(); ++it)
			{
				const JunctionSelection::Junction& junction = it->second;				

				Ogre::ColourValue oldColor = getTerrainData()->getDiffuseInfo(junction.x, junction.z);

				mModifiedMap.insert(DiffuseInfoMap::value_type(Point(junction.x, junction.z),oldColor));

				getTerrainData()->setDiffuseInfo(junction.x, junction.z, color);
			}	

			selection->notifyColourModified();
		}		
	}	

	void DragTerrainColourAction::_onEnd(const Point& pt, bool canceled)
	{
		doFinish(static_cast<JunctionSelection*>(getSceneManipulator()->_getSelection("JunctionSelection")), canceled);
		getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
		getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);

		mActionState = AS_END;
	}
}
