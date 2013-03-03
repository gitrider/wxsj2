#ifndef __FAIRYMODELOBJECT_H__
#define __FAIRYMODELOBJECT_H__

#include "FairyPlacementObject.h"

namespace Fairy
{
    class Model;
    class ObjectFactory;
    class ObjectProxy;

    class ModelObject : public PlacementObject
    {
    protected:
        class Factory;

    public:
        ModelObject(void);
        virtual ~ModelObject();

        const Ogre::String& getType(void) const;
        const Ogre::String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);

        void processMessage(const Ogre::String& msg, const Variant& params);

        Variant getProperty(const Ogre::String& name) const;
       Ogre::String getPropertyAsString(const Ogre::String& name) const;


//laim È±µÃº¯Êý
		void  setTransparency( Real &newTrans ){return;};
		Real  getTransparency() const{return 1;};
        //////////////////////////////////////////////////////////////////////////

        static const Ogre::String msType;
        static const Ogre::String msCategory;

        static ObjectFactory* getFactory(void);

        //////////////////////////////////////////////////////////////////////////

        Model* getModel(void) const
        {
            return mModel;
        }

        const Ogre::String& getModelName(void) const
        {
            return mModelName;
        }

        void setModelName(const Ogre::String& actorName);

        Ogre::Real getAnimationLevel(void) const
        {
            return mAnimationLevel;
        }

        void setAnimationLevel(Ogre::Real level);

        bool getCastShadows(void) const
        {
            return mCastShadows;
        }

        void setCastShadows(bool castShadows);

    protected:
        ObjectProxy* mProxy;
        Model* mModel;
        Ogre::String mModelName;
        Ogre::Real mAnimationLevel;
        bool mCastShadows;
        System* mSystem;

        void createModel(void);
        void destroyModel(void);

	public:
		Real mTransparentTime;
		bool mBEnableTransparent;
    };

}

#endif 
