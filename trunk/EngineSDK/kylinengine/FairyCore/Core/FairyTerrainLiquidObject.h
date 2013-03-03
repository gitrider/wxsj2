#ifndef __FairyTerrainLiquidObject_H__
#define __FairyTerrainLiquidObject_H__

#include "FairyObject.h"

#include <OgreVector3.h>
#include <OgreRenderTargetListener.h>
#include <OgreTexture.h>
#include <OgreMaterial.h>

class Ogre::Camera;
class Ogre::Plane;
class Ogre::RenderTexture;

namespace Fairy {

    class ObjectFactory;
    class System;
    class TerrainLiquid;

    class LiquidRenderTargetListener : public Ogre::RenderTargetListener
    {
    public:
        void setCamera(Ogre::Camera *camera) { assert(camera); mCamera = camera; }
        void setPlane(Ogre::Plane *plane) { assert(plane); mPlane = plane; }
	/*  laim 
	void setSceneManager( Ogre::SceneManager *pS )	{mSceneManager=pS;}
		void saveParams();
        */
        void setTerrainLiquid(TerrainLiquid *liquid) { assert(liquid); mTerrainLiquid = liquid; }

/*  laim
		LiquidRenderTargetListener() : mUpdateNumber(0){}

		void disableUpdate();
		void recoverUpdate();
		bool checkNeedUpdate();
*/
    protected:
        TerrainLiquid *mTerrainLiquid;
        Ogre::Camera *mCamera;
        Ogre::Plane *mPlane;
/*  laim
		Ogre::SceneManager*	mSceneManager;

		Ogre::FogMode		mOldFog;
		Ogre::ColourValue	mOldFogColour;
		Ogre::Real			mRealParam1;
		Ogre::Real			mRealParam2;
		Ogre::Real			mRealParam3;

	public:
		Ogre::RenderTarget*	mRenderTarget;
		int			mUpdateNumber;
		int			mUpdateMinSep;
*/  

  };

    class ReflectRenderTargetListener : public LiquidRenderTargetListener
    {
    public:
        virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
        virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
    };

    class RefractRenderTargetListener : public LiquidRenderTargetListener
    {
    public:
        virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
        virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

	//laim	float		mLastCameraNear;
    };

    class TerrainLiquidObject : public Object
    {
    protected:
        class Factory;

    public:
        TerrainLiquidObject(void);
        virtual ~TerrainLiquidObject();

        const Ogre::String& getType(void) const;
        const Ogre::String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);
        void updateRenderInstance(void);
/* laim
		void setWaterColour( const Ogre::ColourValue &clr );
		const Ogre::ColourValue& getWaterColour() const;
		const Ogre::Real&	getWaterAlpha() const;
		const void          setWaterAlpha( Ogre::Real alphaNew );
*/
        //////////////////////////////////////////////////////////////////////////

        static const Ogre::String msType;
        static const Ogre::String msCategory;

        static ObjectFactory* getFactory(void);

        //////////////////////////////////////////////////////////////////////////

        TerrainLiquid * getTerrainLiquid(void)
        {
            assert(mTerrainLiquid);
            return mTerrainLiquid;
        }

    protected:
        System* mSystem;
        TerrainLiquid *mTerrainLiquid;
        Ogre::MaterialPtr mProjectionMaterial;
        Ogre::Camera *mProjectionCamera;
//laim		Ogre::ColourValue	mWaterColour;

        void createTerrainLiquid(void);
        void destroyTerrainLiquid(void);

    public:     // Intend for direct access by property commands only, DO NOT access outside
        Ogre::String mMaterialName;
//laim		String mOriginMaterialName;
        Ogre::Vector3 mPosition;
        size_t mSubdivision;
        Ogre::Real mTexcoordScale;
        Ogre::Real mTexcoordRotate;
        bool mDepthTexLayerEnabled;
        Ogre::Real mDepthTexLayerScale;
        Ogre::Real mDepthTexLayerAdjust;
       Ogre::String mProjectorName;
        Ogre::Real mProjectorSize;
		Ogre::ColourValue mDiffuse;
		Ogre::Real mAdjustDepth;
		Ogre::ColourValue mMatSpecular;
		Ogre::Real mShininess;

        /////////////////////////////
    protected:

        Ogre::Plane *mReflectPlane;
        Ogre::Plane *mRefractPlane;
        ReflectRenderTargetListener *mReflectRenderTargetListener;
        RefractRenderTargetListener *mRefractRenderTargetListener;

#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
        Ogre::TexturePtr mReflectTexture;
        Ogre::TexturePtr mRefractTexture;
#else
        Ogre::RenderTexture *mReflectRenderTexture;
        Ogre::RenderTexture *mRefractRenderTexture;
#endif

       Ogre::String mReflectTextureName;
       Ogre::String mRefractTextureName;

    protected:
        void _prepareProjector(void);

        void _createRenderTexture(void);
        void _setReflectPlane(void);

        void _deleteRenderTextureRelative(void);
    };
}

#endif // __FairyTerrainLiquidObject_H__
