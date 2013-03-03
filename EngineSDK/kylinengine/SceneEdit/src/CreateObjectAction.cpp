#include "CreateObjectAction.h"
#include "SceneManipulator.h"

#include "Core/FairyUtils.h"
#include "Core/FairySceneInfo.h"
#include "Core/FairyObjectFactoryManager.h"
#include "Core/TerrainData.h"

#include "Core/FairyLightObject.h"

#include <OgreString.h>
#include <OgreVector3.h>
#include <OgreStringConverter.h>

namespace Fairy {

    CreateObjectAction::CreateObjectAction(SceneManipulator* sceneManipulator)
        : Action(sceneManipulator)
        , mDirty(true)
        , mObject()
		, mHasPosition(false)
		, mRangeSphereEntity(NULL)
		, mRangeSphereNode(NULL)
    {
		// create a entity for indicate the range of a point light
		mRangeSphereNode = getIndicatorRootSceneNode()->createChildSceneNode();

		Ogre::MeshPtr sphereMesh = Fairy::createCommonSphere("PointLightRangeSphere", 100, 16, 16);
		Ogre::MaterialPtr material = Fairy::createPureColourMaterial(Ogre::ColourValue(0, 1, 0, 0.75));

		material->getTechnique(0)->getPass(0)->setPolygonMode(Ogre::PM_WIREFRAME);
		material->getTechnique(0)->getPass(0)->setCullingMode(Ogre::CULL_NONE);

		mRangeSphereEntity = getSceneManager()->createEntity("CreateLightRangeSphere", sphereMesh->getName());
		mRangeSphereEntity->setMaterialName(material->getName());

		mRangeSphereNode->attachObject(mRangeSphereEntity);
		mRangeSphereNode->setVisible(false);
    }

    CreateObjectAction::~CreateObjectAction()
    {

		if (mRangeSphereEntity && mRangeSphereEntity->getParentSceneNode())
		{
			mRangeSphereEntity->getParentSceneNode()->detachObject(mRangeSphereEntity);			
		}
		if(mRangeSphereEntity)
		{
			getSceneManager()->destroyEntity(mRangeSphereEntity);
			mRangeSphereEntity = NULL;
		}
		if (mRangeSphereNode)
		{
			mRangeSphereNode->getCreator()->destroySceneNode(mRangeSphereNode);
			mRangeSphereNode = NULL;
		}
    }

    const String& CreateObjectAction::getName(void) const
    {
        static const String name = "CreateObjectAction";
        return name;
    }

    void CreateObjectAction::setParameter(const String& name, const String& value)
    {
        if (name == "type")
        {
            mObject.reset();
            mObjectType = value;
            mParameters.clear();
        }
        else if (Ogre::StringUtil::startsWith(name, "%", false))
        {
            mParameters[name.substr(1)] = value;
        }
		else if(name == "has position")
		{
			mHasPosition = Ogre::StringConverter::parseBool(value);
		}
        else
        {
            Action::setParameter(name, value);
        }

        mDirty = true;
    }

    String CreateObjectAction::getParameter(const String& name) const
    {
        if (name == "type")
        {
            return mObjectType;
        }
        else if (Ogre::StringUtil::startsWith(name, "%", false))
        {
			//david-<<
			Fairy::PropertyNameStringPairList::const_iterator it = mParameters.find(name.substr(1));
            if (it != mParameters.end())
                return it->second;
			//david->>
        }

        return Action::getParameter(name);
    }

    void CreateObjectAction::_onActive(bool active)
    {
        if (!active)
        {
            hide();
        }
    }

    void CreateObjectAction::_onMove(const Point& pt)
    {
        updateInstance(pt);
    }

    void CreateObjectAction::_onBegin(const Point& pt)
    {
        updateInstance(pt);
    }

    void CreateObjectAction::_onDrag(const Point& pt)
    {
        updateInstance(pt);
    }

    void CreateObjectAction::_onEnd(const Point& pt, bool canceled)
    {
        if (!canceled && updateInstance(pt))
        {
            assert(mObject);

            // Permanent it to final scene
            mObject->setName(getSceneManipulator()->getSceneInfo()->generateAutoName(mObject));
			getSceneManipulator()->addObject(mObject);			
        }

		if (mRangeSphereNode)
		{
			mRangeSphereNode->setVisible(false);
		}
        mObject.reset();
    }

    void CreateObjectAction::hide(void)
    {
		if (mRangeSphereNode)
		{
			mRangeSphereNode->setVisible(false);
		}
        mObject.reset();
    }

    bool CreateObjectAction::updateInstance(const Point& pt)
    {
        if (mObjectType.empty())
        {
            hide();
            return false;
        }

        Ogre::Vector3 position, normal;
        if (!getSceneManipulator()->getTerrainIntersects(pt, position, &normal, true))
        {
            hide();
            return false;
        }

        if (!mObject)
        {
            mObject = ObjectFactoryManager::getSingleton().createInstance(mObjectType);
            mObject->setPropertyList(mParameters);
            mObject->createRenderInstance(getSceneManipulator());		
        }
        else
        {
            mObject->setPropertyList(mParameters);
        }

        Ogre::Vector3 groundingPoint(Ogre::Vector3::ZERO);
        if (mObject->hasProperty("grounding point"))
        {
            // Adjust position by grounding point
            groundingPoint = VariantCast<Ogre::Vector3>(mObject->getProperty("grounding point"));
            position -= groundingPoint;
        }
        position = getSceneManipulator()->_alignPosition(position);
        position.y = getTerrainData()->getHeightAt(position.x, position.z) - groundingPoint.y;
        position = getSceneManipulator()->_alignPosition(position);
		if(!mHasPosition)
			mObject->setProperty("position", position);

		mRangeSphereNode->setVisible(false);
		Ogre::String type = mObject->getType();
		if ( type == "Light")
		{
			LightObject *lightObject = static_cast<LightObject *> (mObject.get());
			if (mObject->getPropertyAsString("type") == "point")
			{
				mRangeSphereNode->setPosition(position);
				mRangeSphereNode->setVisible(true);
			}
		}

        return true;
    }
}
