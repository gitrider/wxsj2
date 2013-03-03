#include "AddLiquidObjectPlugin.h"
#include "SceneManipulator.h"

#include "Core/FairyTerrainLiquidObject.h"
#include "Core/FairyObjectProxy.h"
#include "Core/FairySceneInfo.h"
#include "Core/FairyUtils.h"
#include "Core/FairyTerrainLiquidObject.h"
#include "Core/FairyTerrainLiquid.h"
#include "Core/FairyObjectProxy.h"

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreAny.h>

namespace Fairy {

	class AddLiquidObjectPlugin::Indicator
	{
	public:
		Indicator(const ObjectPtr &object, SceneManipulator *sceneManipulator)
			: mProxy(NULL)
		{
			assert(sceneManipulator);

			mUserAny = Ogre::Any();
			mProxy = new ObjectProxy(object);

			Ogre::Vector3 pos = VariantCast<Ogre::Vector3>(object->getProperty("position"));
			mIndicatorSceneNode = sceneManipulator->getIndicatorRootSceneNode()->createChildSceneNode(pos);

			Real radius = 45;
			int rings = 16;
			int segments = 16;

			Ogre::MeshPtr sphereMesh = createCommonSphere(radius, rings, segments);
			Ogre::MaterialPtr material = createPureColourMaterial(
				Ogre::ColourValue(0, 0, 1, 0.25)   );

			mIndicatorEntity = sceneManipulator->getSceneManager()->createEntity(mIndicatorSceneNode->getName()+"_AddLiquidObjectPlugin", sphereMesh->getName());
			//david-<<
			//mIndicatorEntity->setNormaliseNormals(true);
			//david->>
			mIndicatorEntity->setMaterialName(material->getName());
			setUserObject(mProxy);
			
			mIndicatorSceneNode->attachObject(mIndicatorEntity);
			mIndicatorSceneNode->setScale(4,4,4);
			TerrainLiquidObject *terrainLiquidObject = static_cast<TerrainLiquidObject *> (object.get());
			terrainLiquidObject->getTerrainLiquid()->setQueryFlags(0);

			showIndicator(false);
		}

		~Indicator()
		{
			
            if (mIndicatorEntity)
            {
                mIndicatorSceneNode->getCreator()->destroyEntity(mIndicatorEntity);
				mIndicatorEntity = NULL;
            }
			if (mIndicatorSceneNode)
			{
				mIndicatorSceneNode->getCreator()->destroySceneNode(mIndicatorSceneNode);
				mIndicatorSceneNode = NULL;
			}

			if (mProxy)
			{
				delete mProxy;
				mProxy = NULL;
			}

		}

		void showIndicator( bool show )
		{
			assert(mIndicatorSceneNode);           

			mIndicatorSceneNode->showBoundingBox(show);
		}

		void setVisible( bool show )
		{
			assert(mIndicatorSceneNode);           

			mIndicatorSceneNode->setVisible(show);
		}

		void setPosition( const Ogre::Vector3 &pos )
		{
			assert(mIndicatorSceneNode);

			mIndicatorSceneNode->setPosition(pos);
		}    

		void setUserObject(Ogre::UserDefinedObject* obj)
		{
			mUserAny = Ogre::Any(obj);
			mIndicatorEntity->setUserAny(Ogre::Any(obj));
		}

		Ogre::UserDefinedObject* getUserObject(void) const
		{
			return mUserAny.isEmpty() ? 0 : Ogre::any_cast<Ogre::UserDefinedObject*>(mUserAny);
		}

	protected:

		Ogre::Any  mUserAny;

		Ogre::Entity *mIndicatorEntity;

		Ogre::SceneNode *mIndicatorSceneNode;

		ObjectProxy* mProxy;
	};

	AddLiquidObjectPlugin::AddLiquidObjectPlugin(Fairy::SceneManipulator* sceneManipulator)
	{
		assert(sceneManipulator);

		mSceneManipulator = sceneManipulator;
		mSceneManipulator->addSceneListener(this);
	}

	AddLiquidObjectPlugin::~AddLiquidObjectPlugin()
	{
		mSceneManipulator->removeSceneListener(this);

		for (Indicators::iterator i = mIndicators.begin(); i != mIndicators.end(); ++i )
		{
			if ( i->second )
			{
				delete i->second;
				i->second = NULL;
			}
		}
        mIndicators.clear();
	}

	//////////////////////////////////////////////////////////////////////////
	void
		AddLiquidObjectPlugin::onSceneReset(void)
	{
		for (Indicators::iterator i = mIndicators.begin(); i != mIndicators.end(); ++i )
		{
			if ( i->second )
			{
				delete i->second;
				i->second = NULL;
			}
		}
        mIndicators.clear();

		typedef Fairy::SceneInfo::Objects Objects;
		const Fairy::SceneInfo* sceneInfo = mSceneManipulator->getSceneInfo();
		const Objects& objects = sceneInfo->getObjects();
		for (Objects::const_iterator it = objects.begin(); it != objects.end(); ++it)
		{
			Ogre::String type = (*it)->getType();

			// 判断，如果是能处理的类型（TerrainLiquidObject），就处理
			if ( type == "TerrainLiquid" )
			{
				TerrainLiquidObject *terrainLiquidObject = static_cast<TerrainLiquidObject *> ((*it).get());
				assert (terrainLiquidObject);
				Indicator *indicator = new Indicator(*it, mSceneManipulator);

				std::pair<Indicators::iterator, bool> inserted =
					mIndicators.insert(Indicators::value_type(*it, indicator));

				assert(inserted.second);
			}
		}
	}

	void 
		AddLiquidObjectPlugin::onAddObject(const ObjectPtr& object)
	{
		Ogre::String type = object->getType();

		// 判断，如果是能处理的类型（TerrainLiquidObject），就处理
		if ( type == "TerrainLiquid" )
		{
			TerrainLiquidObject *terrainLiquidObject = static_cast<TerrainLiquidObject *> (object.get());
			
			assert (terrainLiquidObject && terrainLiquidObject->getType() == "TerrainLiquid");
			
			Indicator *indicator = new Indicator(object,mSceneManipulator);

			std::pair<Indicators::iterator, bool> inserted =
				mIndicators.insert(Indicators::value_type(object, indicator));

			assert(inserted.second);
		}
	}

	void 
		AddLiquidObjectPlugin::onRemoveObject(const ObjectPtr& object)
	{
		Ogre::String type = object->getType();

		// 判断，如果是能处理的类型（TerrainLiquidObject），就处理
		if ( type == "TerrainLiquid" )
		{
			Indicators::iterator i = mIndicators.find(object);

			if ( i != mIndicators.end() )
			{
				delete i->second;
				i->second = NULL;
				mIndicators.erase(i);                
			}
		}
	}

	/*   void 
	AddLiquidObjectPlugin::onRenameObject(const ObjectPtr& object, const String& oldName)
	{
	}*/

	void 
		AddLiquidObjectPlugin::onSelectObject(const ObjectPtr& object)
	{
		Ogre::String type = object->getType();

		// 判断，如果是能处理的类型（TerrainLiquidObject），就处理
		if ( type == "TerrainLiquid" )
		{
			Indicators::iterator i = mIndicators.find(object);

			if ( i != mIndicators.end() )
			{
				i->second->showIndicator(true);
			}
		}

	}

	void 
		AddLiquidObjectPlugin::onDeselectObject(const ObjectPtr& object)
	{
		Ogre::String type = object->getType();

		// 判断，如果是能处理的类型（TerrainLiquidObject），就处理
		if ( type == "TerrainLiquid" )
		{
			Indicators::iterator i = mIndicators.find(object);

			if ( i != mIndicators.end() )
			{
				i->second->showIndicator(false);
			}
		}
	}

	void 
		AddLiquidObjectPlugin::onDeselectAllObjects(void)
	{ 
		for (Indicators::iterator i = mIndicators.begin(); i != mIndicators.end(); ++i )
		{
			if ( i->second )
			{
				i->second->showIndicator(false);
			}
		}        
	}

	void 
		AddLiquidObjectPlugin::onObjectPropertyChanged(const ObjectPtr& object, const String& name)
	{
		Ogre::String type = object->getType();

		// 判断，如果是能处理的类型（TerrainLiquidObject），就处理
		if ( type == "TerrainLiquid" )
		{
			Indicators::iterator i = mIndicators.find(object);

			if ( i != mIndicators.end() )
			{
				if ( name == "position" )
					i->second->setPosition( VariantCast<Ogre::Vector3>(object->getProperty("position")) );                
			}
		}
	}

	void AddLiquidObjectPlugin::onSelectableChanged(const Ogre::String& objType, bool bSelectable)
	{
		if ( objType == "TerrainLiquid")
		{
			for (Indicators::iterator i = mIndicators.begin(); i != mIndicators.end(); ++i )
			{
				if ( i->second )
				{
					i->second->setVisible(bSelectable);
					i->second->showIndicator(false);
				}
			}
		}
	}

	void AddLiquidObjectPlugin::onSelectableChanged(const ObjectPtr& object, bool bSelectable)
	{
		if ( object->getType() == "TerrainLiquid")
		{
			Indicators::iterator i = mIndicators.find(object);
			if ( i != mIndicators.end() )
			{
				i->second->setVisible(bSelectable);
				i->second->showIndicator(false);
			}
		}
	}
}