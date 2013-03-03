#include "AddParticleSystemObjectPlugin.h"
#include "SceneManipulator.h"

#include "Core/FairyParticleSystemObject.h"
#include "Core/FairyObjectProxy.h"
#include "Core/FairySceneInfo.h"
#include "Core/FairyUtils.h"

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreParticleSystem.h>
#include <OgreAny.h>
namespace Fairy {

    class AddParticleSystemObjectPlugin::Indicator
    {
    public:
        Indicator(const ObjectPtr &object, SceneManipulator *sceneManipulator)
			: mProxy(NULL)
        {
            assert(sceneManipulator);

			mUserAny = Ogre::Any();

			mProxy = new ObjectProxy(object);

            // 根据光源位置来定节点位置
            Ogre::Vector3 pos = VariantCast<Ogre::Vector3>(object->getProperty("position"));
            mIndicatorSceneNode = sceneManipulator->getIndicatorRootSceneNode()->createChildSceneNode(pos);

            Real radius = 5;
            int rings = 16;
            int segments = 16;

            Ogre::MeshPtr sphereMesh = createCommonSphere(radius, rings, segments);
            Ogre::MaterialPtr material = createPureColourMaterial(
                Ogre::ColourValue(1, 0, 0, 0.75)   );

            mIndicatorEntity = sceneManipulator->getSceneManager()->createEntity(mIndicatorSceneNode->getName(), sphereMesh->getName());
			//david-<<
            //mIndicatorEntity->setNormaliseNormals(true);
			//david->>
            mIndicatorEntity->setMaterialName(material->getName());
            setUserObject(mProxy);

            mIndicatorSceneNode->attachObject(mIndicatorEntity);

            // 选择时不考虑粒子系统的包围盒，用的是指示器的包围盒
            ParticleSystemObject *particleSystemObject = static_cast<ParticleSystemObject *> (object.get());
			Ogre::ParticleSystem *system = particleSystemObject->getParticleSystem();

            if (system)
				system->setQueryFlags(0);

            // 根据光源类型来挂接模型
            showIndicator(false);
        }

        ~Indicator()
        {
			if (mIndicatorEntity && mIndicatorSceneNode)
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

        void setPosition( const Ogre::Vector3 &pos )
        {
            assert(mIndicatorSceneNode);

            mIndicatorSceneNode->setPosition(pos);
        }

		void setUserObject(Ogre::UserDefinedObject* obj)
		{
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

    AddParticleSystemObjectPlugin::AddParticleSystemObjectPlugin(Fairy::SceneManipulator* sceneManipulator)
    {
        assert(sceneManipulator);

        mSceneManipulator = sceneManipulator;
        mSceneManipulator->addSceneListener(this);
    }

    AddParticleSystemObjectPlugin::~AddParticleSystemObjectPlugin()
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
        AddParticleSystemObjectPlugin::onSceneReset(void)
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

            // 判断，如果是能处理的类型（ParticleSystemObject），就处理
            if ( type == "ParticleSystem" )
            {
                ParticleSystemObject *particleSystemObject = static_cast<ParticleSystemObject *> ((*it).get());
                Indicator *indicator = new Indicator( *it, mSceneManipulator);

                std::pair<Indicators::iterator, bool> inserted =
                    mIndicators.insert(Indicators::value_type(*it, indicator));

                assert(inserted.second);
            }
        }
    }

    void 
        AddParticleSystemObjectPlugin::onAddObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "ParticleSystem" )
        {
            ParticleSystemObject *particleSystemObject = static_cast<ParticleSystemObject *> (object.get());
            Indicator *indicator = new Indicator(object, mSceneManipulator);

            std::pair<Indicators::iterator, bool> inserted =
                mIndicators.insert(Indicators::value_type(object, indicator));

            assert(inserted.second);
        }
    }

    void 
        AddParticleSystemObjectPlugin::onRemoveObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "ParticleSystem" )
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
    AddParticleSystemObjectPlugin::onRenameObject(const ObjectPtr& object, const String& oldName)
    {
    }*/

    void 
        AddParticleSystemObjectPlugin::onSelectObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "ParticleSystem" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                i->second->showIndicator(true);
            }
        }

    }

    void 
        AddParticleSystemObjectPlugin::onDeselectObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "ParticleSystem" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                i->second->showIndicator(false);
            }
        }
    }

    void 
        AddParticleSystemObjectPlugin::onDeselectAllObjects(void)
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
        AddParticleSystemObjectPlugin::onObjectPropertyChanged(const ObjectPtr& object, const String& name)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "ParticleSystem" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                if ( name == "position" )
                    i->second->setPosition( VariantCast<Ogre::Vector3>(object->getProperty("position")) );                
            }
        }
    }
}