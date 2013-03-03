#ifndef __FAIRYSKYDOMEOBJECT_H__
#define __FAIRYSKYDOMEOBJECT_H__

#include "FairyObject.h"

#include <OgreQuaternion.h>

#include "FairyObjectFactory.h"

namespace Fairy
{
    class ObjectFactory;

    class SkyDomeObject : public Object
    {
    protected:
        class Factory;

    public:
        SkyDomeObject(void);
        virtual ~SkyDomeObject();

        const Ogre::String& getType(void) const;
        const Ogre::String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);
        void updateRenderInstance(void);

        //////////////////////////////////////////////////////////////////////////

        static const Ogre::String msType;
        static const Ogre::String msCategory;

        static ObjectFactory* getFactory(void);
        //////////////////////////////////////////////////////////////////////////
   

    protected:
        Ogre::SceneManager* mSceneManager;

        void updateRenderInstanceImpl(void);

    public:     // Intend for direct access by property commands only, DO NOT access outside
        Ogre::String mMaterialName;
        Ogre::Real mCurvature;
        Ogre::Real mTiling;
        Ogre::Real mDistance;
        bool mDrawFirst;
        Ogre::Quaternion mOrientation;
        int mXsegments;
        int mYsegments;
        int mYsegments_keep;
    };

//------------------------------------------------------------------------------------------------
	class RegionObject : public Object
	{
	protected:

		class Factory : public Fairy::ObjectFactory
		{
		public:
		
			const Ogre::String& getType(void) const
			{
				return  msType;
			}

			Object* createInstance(void)
			{
				return new Fairy::RegionObject;
			}
		};


	public:
		RegionObject(void){};
	    static const Ogre::String msType;
		const Ogre::String& getType(void)   const { return msType ;};
		const Ogre::String& getCategory(void)const{return  msType;}  ;

		void createRenderInstance(System* system){return;};
		void destroyRenderInstance(void){return ;};

		static ObjectFactory* getFactory(void){
			static  Factory factory;
			return &factory;
		}

		int  mRegion_id;
	};




}

#endif 
