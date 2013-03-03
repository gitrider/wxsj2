#ifndef __FAIRYSTATICENTITYOBJECT_H__
#define __FAIRYSTATICENTITYOBJECT_H__

#include "FairyPlacementObject.h"
#include "OgreExt/OgreUserDefinedObject.h"
#include "OgreColourValue.h"
#include <OgreAny.h>
#include <map>

namespace Fairy
{
    class ObjectFactory;
    class ObjectProxy;

    class EntityTransparentor;

    class StaticEntityObject : public PlacementObject
    {
    protected:
        class Factory;
		static Ogre::uint ms_uGenPrefixCount;

    public:
        StaticEntityObject(void);
        virtual ~StaticEntityObject();

        const Ogre::String& getType(void) const;
        const Ogre::String& getCategory(void) const;

        void queryReferenceResources(ResourceCollection* resourceCollection);

        void queryBakableEntities(EntityList& entities);
        void bakeStaticGeometry(Ogre::StaticGeometry* staticGeometry, const EntityList& entities);

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);

        void processMessage(const Ogre::String& msg, const Variant& params);

        bool setProperty(const Ogre::String& name, const Variant& value);
        bool setPropertyAsString(const Ogre::String& name, const Ogre::String& value);
        Variant getProperty(const Ogre::String& name) const;
       Ogre::String getPropertyAsString(const Ogre::String& name) const;

        //////////////////////////////////////////////////////////////////////////

        static const Ogre::String msType;
        static const Ogre::String msCategory;

        static ObjectFactory* getFactory(void);

        //////////////////////////////////////////////////////////////////////////

        Ogre::Entity* getEntity(void) const
        {
            return mEntity;
        }

        const Ogre::String& getMeshName(void) const
        {
            return mMeshName;
        }

        void setMeshName(const Ogre::String& meshName);

        Ogre::Real getAnimationLevel(void) const
        {
            return mAnimationLevel;
        }

        void setAnimationLevel(Ogre::Real level);

        void setTransparency(Ogre::Real transparency);

        Ogre::Real getTransparency(void) const
        {
			return mCurrentTransparency;
        }

        bool getCastShadows(void) const
        {
            return mCastShadows;
        }

        void setCastShadows(bool castShadows);

        bool getReceiveShadows(void) const
        {
            return mReceiveShadows;
        }

        void setReceiveShadows(bool receiveShadows);

        bool getReceiveDecals(void) const
        {
            return mReceiveDecals;
        }

        void setReceiveDecals(bool receiveDecals);

		// 获取是否允许透明
		bool getEnableTransparency(void) const
		{
			return mEnableTransparency;
		}
        // 设置是否允许透明
		void setEnableTransparency(bool enableTransparency);

		void setAdditionalDiffuse(Ogre::ColourValue diffuse);

		Ogre::ColourValue getAdditionalDiffuse() const
		{
			return mAdditionalDiffuse;
		};

		void setDiffuseAdded(bool bAdded);		
		
		bool getDiffuseAdded() const
		{
			return mDiffuseAdded;
		};

		void setUserObject(Ogre::UserDefinedObject* userObject);
		Ogre::UserDefinedObject* getUserObject(void) const;
		
    protected:
        System* mSystem;
        ObjectProxy* mProxy;
        Ogre::Entity* mEntity;
        Ogre::String mMeshName;
        Ogre::Real mAnimationLevel;
		Ogre::Real mCurrentTransparency;		// 当前model的透明度
		Ogre::Real mDestTransparency;		    // 目标透明度

        bool mCastShadows;
		bool mReceiveShadows;
        bool mReceiveDecals;
        bool mEnableTransparency;               // 是否允许透明

		Ogre::ColourValue mAdditionalDiffuse;	// 附加diffuse颜色
		bool mDiffuseAdded;						// 是否采用相加算法

        EntityTransparentor* mEntityTransparentor;
        AnimationType mAnimationType;
		typedef std::map<Ogre::String, Ogre::String> MaterialMap;

		MaterialMap mClonedMaterials;

		Ogre::String mPrefixName;

		void clearClonedMaterials();

        void createEntity(void);
        void destroyEntity(void);
        void updateAnimationType(void);
        void updateTransparency(void);
        void updateNormaliseNormals(void);
		void cloneMaterialsManually();

		Ogre::Any mUserAny;
    };

}

#endif 
