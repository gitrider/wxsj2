//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _SKY_H_
#define _SKY_H_

#ifndef _MMATH_H_
#include "math/mMath.h"
#endif
#ifndef _SCENEOBJECT_H_
#include "sceneManager/sceneObject.h"
#endif
#ifndef _SCENESTATE_H_
#include "sceneManager/sceneState.h"
#endif
#ifndef _SCENEGRAPH_H_
#include "sceneManager/sceneManager.h"
#endif
#ifndef _MPOINT3_H_
#include "math/mPoint3.h"
#endif
#ifndef _MATERIALLIST_H_
#include "materials/materialList.h"
#endif
#ifndef _GAMEBASE_H_
//#include "client/gameBase.h"
#endif
#ifndef _RENDERPASSMANAGER_H_
#include "renderManager/renderPassManager.h"
#endif

#include "graphic/gDevice.h"

#define MAX_NUM_LAYERS 3
#define MAX_BAN_POINTS 20

class SceneManager;
class SceneState;
class SceneRenderImage;

enum SkyState
{
   isDone = 0,
   comingIn = 1,
   goingOut = 2
};

typedef struct
{
   bool StormOn;
   bool FadeIn;
   bool FadeOut;
   S32 currentCloud;
   F32 stormSpeed;
   F32 stormDir;
   S32 numCloudLayers;
   F32 fadeSpeed;
   SkyState stormState;
}StormInfo;

typedef struct
{
   SkyState state;
   F32 speed;
   F32 time;
   F32 fadeSpeed;
}StormCloudData;

typedef struct
{
   bool active;
   SkyState state;
   F32 speed;
   F32 endPercentage;
   F32 lastPercentage;
}StormFogVolume;

typedef struct
{
   SkyState state;
   U32 startTime;
   F32 endPercentage;
   F32 time;
   S32 current;
   U32 lastTime;
   StormFogVolume volume[MaxFogVolumes];
}StormFogData;

//---------------------------------------------------------------------------
class Cloud
{
  private:
   Point3F mPoints[25];
   Point2F mSpeed;
   F32 mCenterHeight, mInnerHeight, mEdgeHeight;
   F32 mAlpha[25];
   S32 mDown, mOver;
   static F32 mRadius;
   U32 mLastTime;
   F32 mOffset;
   Point2F mBaseOffset, mTexCoords[25], mTextureScale;
   GTexHandle mCloudHandle;

   Point2F alphaCenter;
   Point2F stormUpdate;
   F32 stormAlpha[25];
   F32 mAlphaSave[25];

   GStateBlockRef mCloudSB;

   static StormInfo mGStormData;
  public:
   Cloud();
   ~Cloud();
   void setPoints();
   void setHeights(F32 cHeight, F32 iHeight, F32 eHeight);
   void setTexture(GTexHandle);
   void setSpeed(const Point2F &speed);
   void setTextPer(F32 cloudTextPer);
   void updateCoord();
   void calcAlpha();
   void render(U32, U32, bool, S32, PlaneF*);
   void updateStorm();
   void calcStorm(F32 speed, F32 fadeSpeed);
   void calcStormAlpha();
   static void startStorm(SkyState);
   static void setRadius(F32 rad) {mRadius = rad;}
   void setRenderPoints(Point3F* renderPoints, Point2F* renderTexPoints, F32* renderAlpha, F32* renderSAlpha, S32 index);
   void clipToPlane(Point3F* points, Point2F* texPoints, F32* alphaPoints, F32* sAlphaPoints, U32& rNumPoints, const PlaneF& rPlane);
};

//--------------------------------------------------------------------------
class Sky : public SceneObject
{
   typedef SceneObject Parent;
  private:

    StormCloudData mStormCloudData;
    StormFogData mStormFogData;
    GTexHandle mSkyHandle[6];
    F32 mCloudHeight[MAX_NUM_LAYERS];
    F32 mCloudSpeed[MAX_NUM_LAYERS];
    Cloud mCloudLayer[MAX_NUM_LAYERS];
    F32 mRadius;
    Point3F mPoints[10];
    Point2F mTexCoord[4];
    FileName mMaterialListName;
    Point3F mSkyBoxPt;
    Point3F mTopCenterPt;
    Point3F mSpherePt;
    ColorI mRealFogColor;
    ColorI mRealSkyColor;

    MaterialList mMaterialList;
    ColorF mFogColor;
    bool mSkyTexturesOn;
    bool mRenderBoxBottom;
    ColorF mSolidFillColor;
    F32 mFogDistance;
    F32 mVisibleDistance;
    U32 mNumFogVolumes;
    FogVolume mFogVolumes[MaxFogVolumes];
    F32 mFogLine;
    F32 mFogTime;
    F32 mFogPercentage;
    S32 mFogVolume;
    S32 mRealFog;
    F32 mRealFogMax;
    F32 mRealFogMin;
    F32 mRealFogSpeed;
    bool mNoRenderBans;
    F32 mBanOffsetHeight;

    SkyState mFogState;

    S32 mNumCloudLayers;
    Point3F mWindVelocity;

    F32 mLastVisDisMod;

    GVertexBufferHandle<GVertexPCT> mSkyVB;

    static bool smCloudsOn;
    static bool smCloudOutlineOn;
    static bool smSkyOn;
    static S32  smNumCloudsOn;

    bool mStormCloudsOn;
    bool mStormFogOn;
    bool mSetFog;

    bool mSkyGlow;
    ColorF mSkyGlowColor;

    GStateBlockRef mClearSB;
    GStateBlockRef mSkyBoxSB;
    GStateBlockRef mRenderBansSB;

    void calcPoints();
    void loadVBPoints();
    void setupStateBlocks();
  protected:
    bool onAdd();
    void onRemove();

    ObjectRenderInst::RenderDelegate mRenderDelegate;

    bool prepRenderImage  ( SceneState *state, const U32 stateKey, const U32 startZone, const bool modifyBaseZoneState=false);
    void renderObject(ObjectRenderInst *ri, BaseMatInstance* );
    void render(SceneState *state);
    void calcAlphas_Heights(F32 zCamPos, F32 *banHeights, F32 *alphaBan, F32 DepthInFog);
    void renderSkyBox(F32 lowerBanHeight, F32 alphaIn);
    void calcBans(F32 *banHeights, Point3F banPoints[][MAX_BAN_POINTS], Point3F *cornerPoints);
    void renderBans(const F32 *alphaBan, const F32 *banHeights, const Point3F banPoints[][MAX_BAN_POINTS], const Point3F *cornerPoints);
    
    void startStorm();
    void setVisibility();
    void initSkyData();
    bool loadDml();
    void updateFog();
    void updateRealFog();
    void startStormFog();
    void setRenderPoints(Point3F* renderPoints, S32 index);
    void calcTexCoords(Point2F* texCoords, Point3F* renderPoints, S32 index, F32 lowerBanHeight);
  public:
    Point2F mWindDir;
    enum NetMaskBits {
        InitMask = BIT(0),
        VisibilityMask = BIT(1),
        StormCloudMask = BIT(2),
        StormFogMask = BIT(3),
        StormRealFogMask = BIT(4),
        WindMask = BIT(5),
        StormCloudsOnMask = BIT(6),
        StormFogOnMask = BIT(7),
        SkyGlowMask = BIT(8)
   };
   enum Constants {
        EnvMapMaterialOffset = 6,
        CloudMaterialOffset  = 7
   };

   Sky();
   ~Sky();

   F32 getVisibleDistance() const { return mVisibleDistance; }

   /// @name Storm management.
   /// @{
   void stormCloudsShow(bool);
   void stormFogShow(bool);
   void stormCloudsOn(S32 state, F32 time);
   void stormFogOn(F32 percentage, F32 time);
   void stormRealFog(S32 value, F32 max, F32 min, F32 speed);
   /// @}

   /// @name Wind velocity
   /// @{

   void setWindVelocity(const Point3F &);
   const Point3F &getWindVelocity() const;
   /// @}

   /// @name Environment mapping
   /// @{

//   TextureHandle getEnvironmentMap() { return mMaterialList.getMaterial(EnvMapMaterialOffset); }
   /// @}

   /// Torque infrastructure
   DECLARE_CONOBJECT(Sky);
   static void initPersistFields();
   static void consoleInit();
   void updateVisibility();

};




#endif
