#ifndef __FAIRYENVIROMENTOBJECT_H__
#define __FAIRYENVIROMENTOBJECT_H__

#include "FairyObject.h"

#include <OgreColourValue.h>
#include <OgreCommon.h>
#include <OgreVector3.h>

namespace Fairy
{
    class ObjectFactory;

    class EnviromentObject : public Object
    {
    protected:
        class Factory;

    public:
        EnviromentObject(void);
        virtual ~EnviromentObject();

        const Ogre::String& getType(void) const;
        const Ogre::String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);
        void updateRenderInstance(void);

        Ogre::uint getPropertyFlags(const Ogre::String& name) const;

		void setWeatherParticleSystemPosition(const Ogre::Vector3& pos);

		void setRenderWeather(bool render);
		bool getRenderWeather(void)
		{
			return mRenderWeather;
		}

		void initialize(void);

		void setRenderFloodlight(bool render);
		bool getRenderFloodlight(void)
		{
			return mRenderFloodlight;
		}
        //////////////////////////////////////////////////////////////////////////

        static const Ogre::String msType;
        static const Ogre::String msCategory;

        static ObjectFactory* getFactory(void);

		void setSystem(Fairy::System* system)
		{
			mSystem = system;
		}

/* laim ∞Ê±æ≤Ó“Ï¥˙¬Î
		void	setTerrainBrightness( float nNewBrightness );
		void	setTerrainFocusColour( const Ogre::ColourValue &clr );
		void	setTerrainLightmapThrehold( float nNewLightmapThrehold );
		void	setTerrainSpeculareExtend( const Ogre::Vector3 &clr );
		void	setWindDirChange( const Ogre::Vector3 &vNewDirChange );
		void	setWindDirForceMax( const Ogre::Real &newForce );
*/
    protected:
        Ogre::SceneManager* mSceneManager;
        Ogre::Viewport* mViewport;
	    Fairy::System* mSystem;
	

		Ogre::ParticleSystem* mWeatherParticleSystem;

        void updateRenderInstanceImpl(void);

		void createWeatherParticleSystemNode(System* system);
		void destroyWeatherParticleSystemNode(void);

		void createWeatherParticleSystem(void);
		void destroyWeatherParticleSystem(void);
        
    public:     // Intend for direct access by property commands only, DO NOT access outside
        Ogre::ColourValue mAmbientLight;
        Ogre::ColourValue mBackgroundColour;

		bool mRenderFloodlight;
		Ogre::ColourValue mModulateColour;
		Ogre::Real mBrightnessScale;
		Ogre::Real mShineAmount;

        Ogre::FogMode mFogMode;
        Ogre::ColourValue mFogColour;
        Ogre::Real mFogExpDensity;
        Ogre::Real mFogLinearStart;
        Ogre::Real mFogLinearEnd; 
/* laim ∞Ê±æ≤Ó“Ï¥˙¬Î
		Ogre::Real	mLightmapThrehold;
		Ogre::Vector3	mSpecularExtendAdded;

		Ogre::Vector3	mWindDirChange;
		Ogre::Real		mWindForceMax;
*/	
		/// weather particle system
		Ogre::String mWeatherParticleSystemName;
		Ogre::SceneNode* mWeatherParticleSystemNode;
		bool mRenderWeather;
/* laim ∞Ê±æ≤Ó“Ï¥˙¬Î
		float	mTerrainBrightness;
		Ogre::ColourValue	mTerrainFocusLight;
*/
    };

}

#endif 
