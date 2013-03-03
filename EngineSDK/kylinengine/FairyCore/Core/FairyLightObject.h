#ifndef __FAIRYLIGHTOBJECT_H__
#define __FAIRYLIGHTOBJECT_H__

#include "FairyObject.h"
#include <OgreLight.h>

namespace Fairy
{
    class ObjectFactory;

    class LightObject : public Object
    {
    protected:
        class Factory;

    public:
        LightObject(void);
        virtual ~LightObject();

        const Ogre::String& getType(void) const;
        const Ogre::String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);
        void updateRenderInstance(void);

        Ogre::uint getPropertyFlags(const Ogre::String& name) const;

        //////////////////////////////////////////////////////////////////////////

        static const Ogre::String msType;
        static const Ogre::String msCategory;

        static ObjectFactory* getFactory(void);

        //////////////////////////////////////////////////////////////////////////

        Ogre::Light* getLight(void) const
        {
            return mLight;
        }    

    protected:
        Ogre::SceneNode* mBaseSceneNode;
        Ogre::SceneNode* mSceneNode;
        Ogre::Light* mLight;

        void createLight(void);
        void destroyLight(void);
        void updateRenderInstanceImpl(void);

    public:     // Intend for direct access by property commands only, DO NOT access outside
        Ogre::Light::LightTypes mLightType;
        Ogre::ColourValue mDiffuse;
        Ogre::ColourValue mSpecular;
        Ogre::Vector3 mPosition;
        Ogre::Vector3 mDirection;
        Ogre::Real mRange;
        Ogre::Real mAttenuationConstant;
        Ogre::Real mAttenuationLinear;
        Ogre::Real mAttenuationQuadric;
        Ogre::Real mSpotOuterAngle;
        Ogre::Real mSpotInnerAngle;
        Ogre::Real mSpotFalloff;
        bool mCastShadows;
        bool mStaticLight;
		//cky-add 20080421<<<<<<<<
		bool mIsAreaLightFlag;
		//cky-add 20080421<<<<<<<<
    };

}

#endif 