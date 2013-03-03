#include "AddEffectObjectPlugin.h"
#include "SceneManipulator.h"
#include "Core/FairyEffectObject.h"
#include "Core/FairyObjectProxy.h"
#include "Core/FairySceneInfo.h"
#include "Core/FairyUtils.h"
#include "EffectSystem/FairyEffect.h"

// ogre header
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Fairy	{

	class AddEffectObjectPlugin::Indicator
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
			// 显示带方向的箭头
			mIndicatorEntity = sceneManipulator->getSceneManager()->createEntity(mIndicatorSceneNode->getName(), "axes.mesh");
			//david-<<
			setUserObject(mProxy);
			//david->>
			mIndicatorSceneNode->attachObject(mIndicatorEntity);
			mIndicatorSceneNode->setScale(6,6,6);
			// 选择时不考虑粒子系统的包围盒，用的是指示器的包围盒
    //        EffectObject *effectObject = static_cast<EffectObject *> (object.get());
     //       effectObject->getParticleSystem()->setQueryFlags(0);

            // 根据光源类型来挂接模型
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
       
		void setOrientation( const Ogre::Quaternion &qua )
		{
			assert(mIndicatorSceneNode);

			mIndicatorSceneNode->setOrientation(qua);
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

    AddEffectObjectPlugin::AddEffectObjectPlugin(Fairy::SceneManipulator* sceneManipulator)
    {
        assert(sceneManipulator);

        mSceneManipulator = sceneManipulator;
        mSceneManipulator->addSceneListener(this);
    }

    AddEffectObjectPlugin::~AddEffectObjectPlugin()
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
        AddEffectObjectPlugin::onSceneReset(void)
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
            if ( type == "Effect" )
            {
                EffectObject *effectObject = static_cast<EffectObject *> ((*it).get());
                Indicator *indicator = new Indicator( *it, mSceneManipulator);

                std::pair<Indicators::iterator, bool> inserted =
                    mIndicators.insert(Indicators::value_type(*it, indicator));

                assert(inserted.second);
            }
        }
    }

    void 
        AddEffectObjectPlugin::onAddObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "Effect" )
        {
            EffectObject *effectObject = static_cast<EffectObject *> (object.get());
            Indicator *indicator = new Indicator(object, mSceneManipulator);

            std::pair<Indicators::iterator, bool> inserted =
                mIndicators.insert(Indicators::value_type(object, indicator));

            assert(inserted.second);
        }
    }

    void 
        AddEffectObjectPlugin::onRemoveObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "Effect" )
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
    AddEffectObjectPlugin::onRenameObject(const ObjectPtr& object, const String& oldName)
    {
    }*/

    void 
        AddEffectObjectPlugin::onSelectObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "Effect" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                i->second->showIndicator(true);
            }
        }

    }

    void 
        AddEffectObjectPlugin::onDeselectObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "Effect" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                i->second->showIndicator(false);
            }
        }
    }

    void 
        AddEffectObjectPlugin::onDeselectAllObjects(void)
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
        AddEffectObjectPlugin::onObjectPropertyChanged(const ObjectPtr& object, const String& name)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "Effect" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                if ( name == "position" )
                    i->second->setPosition( VariantCast<Ogre::Vector3>(object->getProperty("position")) );    
				else if(name == "orientation")
					i->second->setOrientation( VariantCast<Ogre::Quaternion>(object->getProperty("orientation")) );     
            }
        }
    }

	void AddEffectObjectPlugin::onSelectableChanged(const Ogre::String& objType, bool bSelectable)
	{
		if ( objType == "Effect")
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

	void AddEffectObjectPlugin::onSelectableChanged(const ObjectPtr& object, bool bSelectable)
	{
		if ( object->getType() == "Effect")
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