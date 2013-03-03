#include "AddLightObjectPlugin.h"
#include "SceneManipulator.h"

#include "Core/FairyLightObject.h"
#include "Core/FairyObjectProxy.h"
#include "Core/FairySceneInfo.h"
#include "Core/TerrainData.h"
#include <OgreAny.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreAny.h>
namespace Fairy {

    class AddLightObjectPlugin::Indicator
    {
    public:
        Indicator(const ObjectPtr &object, Ogre::SceneManager *sceneMgr, Fairy::SceneManipulator *sceneManipulator)
			: mProxy(NULL), mOldPos(Ogre::Vector3::ZERO)
		{
            assert(sceneMgr && sceneManipulator);

			mUserAny = Ogre::Any();

			mProxy = new ObjectProxy(object);

            mSceneManipulator = sceneManipulator;

            mCurrentLightEntity = NULL;

            // ���ݹ�Դλ�������ڵ�λ��
            Ogre::Vector3 pos = VariantCast<Ogre::Vector3>(object->getProperty("position"));
            mIndicatorSceneNode = sceneManipulator->getIndicatorRootSceneNode()->createChildSceneNode(pos);
       //     mIndicatorSceneNode->scale(2.0,15.0,2.0);

            //  ��������entity�ֱ����������ֹ�Դ��ģ��
            mPointLightEntity = sceneMgr->createEntity(mIndicatorSceneNode->getName()+"point","pointLight.mesh");
            mDirectionalLightEntity = sceneMgr->createEntity(mIndicatorSceneNode->getName()+"directional","directionalLight.mesh");
            mDirectionalLightEntity->setQueryFlags(0);

            mSpotLightEntity = sceneMgr->createEntity(mIndicatorSceneNode->getName()+"spotlight","spotLight.mesh");

			setUserObject(mProxy);

            // ���ݹ�Դ�������ҽ�ģ��
            setIndicatorModel(mProxy->getObject()->getPropertyAsString("type"));

            // ���ݹ�Դ����������scenenode����
            Ogre::Vector3 &dir = VariantCast<Ogre::Vector3>(object->getProperty("direction"));
            setDirection(dir);

            showIndicator(false);
        }

        ~Indicator()
        {
            
            if (mPointLightEntity)
            {
                mIndicatorSceneNode->getCreator()->destroyEntity(mPointLightEntity);
				mPointLightEntity = NULL;
            }
            if (mDirectionalLightEntity)
            {
                mIndicatorSceneNode->getCreator()->destroyEntity(mDirectionalLightEntity);
				mDirectionalLightEntity = NULL;
            }
            if (mSpotLightEntity)
            {
                mIndicatorSceneNode->getCreator()->destroyEntity(mSpotLightEntity);
				mSpotLightEntity = NULL;
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

            assert( mCurrentLightEntity && mDirectionalLightEntity );

            // ����Ƿ����
            if ( mCurrentLightEntity == mDirectionalLightEntity )
            {
                mIndicatorSceneNode->setVisible(show);
                mIndicatorSceneNode->showBoundingBox(false);

                // ��ȡ�����������εĽ���
                Ogre::Vector3 pos;
                Ogre::Ray cameraRay( mSceneManipulator->getCamera()->getPosition(), mSceneManipulator->getCamera()->getDirection() );
                bool hit = mSceneManipulator->getTerrainIntersects(cameraRay, pos);

                if (hit)
                {
                    // �ڵ����ϵ�һ�״����ֹ�Դָʾ��
                    float height = mSceneManipulator->getTerrainData()->getHeightAt(pos.x, pos.z) + 100.0f;
                    mIndicatorSceneNode->setPosition(pos.x, height, pos.z);
                }
            }
            else
            {
                mIndicatorSceneNode->showBoundingBox(show);
            }
        }

        void setPosition( const Ogre::Vector3 &pos )
        {
            assert(mIndicatorSceneNode);
			mOldPos = pos;
            if ( mCurrentLightEntity != mDirectionalLightEntity )
                mIndicatorSceneNode->setPosition(pos);
        }

		void setVisible( bool show )
		{
			assert(mIndicatorSceneNode);           

			mIndicatorSceneNode->setVisible(show);
		}

        void setDirection( const Ogre::Vector3 &dir )
        {
            assert(mIndicatorSceneNode);

            // �ֱ������ڵ��������ϵķ���
            Ogre::Vector3 yAdjustVec = -dir;
            yAdjustVec.normalise();
           
            Ogre::Vector3 xVec = mIndicatorSceneNode->getOrientation().zAxis().crossProduct( yAdjustVec );
            xVec.normalise();

            Ogre::Vector3 zVec = xVec.crossProduct( yAdjustVec );
            zVec.normalise();        
            mIndicatorSceneNode->setOrientation(Ogre::Quaternion( xVec, yAdjustVec, zVec ));     
        }

        void setIndicatorModel( const Ogre::String &lightType )
        {
            assert( !lightType.empty() );

            assert ( mIndicatorSceneNode );
            assert ( mPointLightEntity );
            assert ( mDirectionalLightEntity );
            assert ( mSpotLightEntity );

            mIndicatorSceneNode->detachAllObjects();

            if ( lightType == "point" )
            {
              //  mIndicatorSceneNode->attachObject(mPointLightEntity);
                mCurrentLightEntity = mPointLightEntity;
				if ( mOldPos != Ogre::Vector3::ZERO )
					mIndicatorSceneNode->setPosition(mOldPos);
            }	
            else if ( lightType == "directional")
            {
             //   mIndicatorSceneNode->attachObject(mDirectionalLightEntity);
				// ��Ϊ����Ȿ��û��λ�����ԣ�����ָʾ��������ڵ������ģ���ı�scene node��λ��
				// ���������ȰѾ�λ�ñ�������
                mOldPos = mIndicatorSceneNode->getPosition();
                mCurrentLightEntity = mDirectionalLightEntity;
            //    mIndicatorSceneNode->setPosition(0, 0 ,0);

                // ��ȡ���������ĵ�ĸ߶�
                float height = mSceneManipulator->getTerrainData()->getHeightAt(0,0);
                mIndicatorSceneNode->setPosition(0, height ,0);
            }
            else if ( lightType == "spotlight" )
            {
             //   mIndicatorSceneNode->attachObject(mSpotLightEntity);
                mCurrentLightEntity = mSpotLightEntity;
				if ( mOldPos != Ogre::Vector3::ZERO )
					mIndicatorSceneNode->setPosition(mOldPos);
            }

            mIndicatorSceneNode->attachObject(mCurrentLightEntity);
        }

		void setUserObject(Ogre::UserDefinedObject* obj)
		{
			mUserAny = Ogre::Any(obj);
			mPointLightEntity->setUserAny(Ogre::Any(obj));
			mDirectionalLightEntity->setUserAny(Ogre::Any(obj));
			mSpotLightEntity->setUserAny(Ogre::Any(obj));
		}

		Ogre::UserDefinedObject* getUserObject(void) const
		{
			return mUserAny.isEmpty() ? 0 : Ogre::any_cast<Ogre::UserDefinedObject*>(mUserAny);
		}

	protected:

		Ogre::Any  mUserAny;

        Ogre::Entity *mPointLightEntity;
        Ogre::Entity *mDirectionalLightEntity;
        Ogre::Entity *mSpotLightEntity;

        Ogre::Entity *mCurrentLightEntity;

        Ogre::SceneNode *mIndicatorSceneNode;

        Fairy::SceneManipulator *mSceneManipulator;
		ObjectProxy* mProxy;

		Ogre::Vector3 mOldPos;
    };

    AddLightObjectPlugin::AddLightObjectPlugin(Fairy::SceneManipulator* sceneManipulator)
    {
        assert(sceneManipulator);
    
        mSceneManipulator = sceneManipulator;
        mSceneManipulator->addSceneListener(this);
    }

    AddLightObjectPlugin::~AddLightObjectPlugin()
    {
        mSceneManipulator->removeSceneListener(this);

        clearIndicators();
    }

    //////////////////////////////////////////////////////////////////////////
    void
    AddLightObjectPlugin::onSceneReset(void)
    {
        clearIndicators();

        typedef Fairy::SceneInfo::Objects Objects;
        const Fairy::SceneInfo* sceneInfo = mSceneManipulator->getSceneInfo();
        const Objects& objects = sceneInfo->getObjects();
        for (Objects::const_iterator it = objects.begin(); it != objects.end(); ++it)
        {
            Ogre::String type = (*it)->getType();

            // �жϣ�������ܴ�������ͣ�LightObject�����ʹ���
            if ( type == "Light" )
            {
                LightObject *lightObject = static_cast<LightObject *> ((*it).get());
                Indicator *indicator = new Indicator( *it, mSceneManipulator->getSceneManager(),
                    mSceneManipulator );

                std::pair<Indicators::iterator, bool> inserted =
                    mIndicators.insert(Indicators::value_type(*it, indicator));

                assert(inserted.second);
            }
        }
    }

    void 
    AddLightObjectPlugin::onAddObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // �жϣ�������ܴ�������ͣ�LightObject�����ʹ���
        if ( type == "Light" )
        {
            LightObject *lightObject = static_cast<LightObject *> (object.get());
            Indicator *indicator = new Indicator(object,mSceneManipulator->getSceneManager(),
                mSceneManipulator );

            std::pair<Indicators::iterator, bool> inserted =
                mIndicators.insert(Indicators::value_type(object, indicator));

            assert(inserted.second);
        }
    }

    void 
    AddLightObjectPlugin::onRemoveObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // �жϣ�������ܴ�������ͣ�LightObject�����ʹ���
        if ( type == "Light" )
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
        AddLightObjectPlugin::onRenameObject(const ObjectPtr& object, const String& oldName)
    {
    }*/

    void 
    AddLightObjectPlugin::onSelectObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // �жϣ�������ܴ�������ͣ�LightObject�����ʹ���
        if ( type == "Light" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                i->second->showIndicator(true);
            }
        }

    }

    void 
    AddLightObjectPlugin::onDeselectObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // �жϣ�������ܴ�������ͣ�LightObject�����ʹ���
        if ( type == "Light" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                i->second->showIndicator(false);
            }
        }
    }

    void 
    AddLightObjectPlugin::onDeselectAllObjects(void)
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
    AddLightObjectPlugin::onObjectPropertyChanged(const ObjectPtr& object, const String& name)
    {
        Ogre::String type = object->getType();

        // �жϣ�������ܴ�������ͣ�LightObject�����ʹ���
        if ( type == "Light" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                if ( name == "position" )
                   i->second->setPosition( VariantCast<Ogre::Vector3>(object->getProperty("position")) );
                else if ( name == "type" )
                {
                    Ogre::String lightType = object->getPropertyAsString("type");
                    i->second->setIndicatorModel(lightType);
                }
                else if ( name == "direction" )
                {
                    i->second->setDirection( VariantCast<Ogre::Vector3>(object->getProperty("direction")) );
                }
            }
        }

    }

    void 
	AddLightObjectPlugin::clearIndicators(void)
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
	}

	void AddLightObjectPlugin::onSelectableChanged(const Ogre::String& objType, bool bSelectable)
	{
		if ( objType == "Light")
		{
			for (Indicators::iterator i = mIndicators.begin(); i != mIndicators.end(); ++i )
			{
				const Fairy::ObjectPtr& object = i->first;
				if (object->getName() == Fairy::FairyAutoMainLightOneObjectName 
					||object->getName() == Fairy::FairyAutoMainLightTwoObjectName)
				{
					continue;
				}

				if ( i->second )
				{
					i->second->setVisible(bSelectable);
					i->second->showIndicator(false);
				}
			}
		}
	}

	void AddLightObjectPlugin::onSelectableChanged(const ObjectPtr& object, bool bSelectable)
	{
		if ( object->getType() == "Light")
		{
			if (object->getName() == Fairy::FairyAutoMainLightOneObjectName 
				||object->getName() == Fairy::FairyAutoMainLightTwoObjectName)
			{
				return;
			}

			Indicators::iterator i = mIndicators.find(object);
			if ( i != mIndicators.end() )
			{
				i->second->setVisible(bSelectable);
				i->second->showIndicator(false);
			}
		}
	}
}