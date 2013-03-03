//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _WATERBLOCK_H_
#define _WATERBLOCK_H_

#ifndef _GAMEBASE_H_
#include "client/gameBase.h"
#endif
#ifndef _GDEVICE_H_
#include "graphic/gDevice.h"
#endif
#ifndef _SCENEDATA_H_
#include "materials/sceneData.h"
#endif
#ifndef _REFLECTPLANE_H_
#include "graphic/reflectPlane.h"
#endif
#ifndef _MATINSTANCE_H_
#include "materials/matInstance.h"
#endif
#ifndef _GPRIMITIVEBUFFER_H_
#include "graphic/gPrimitiveBuffer.h"
#endif
#ifndef _RENDERPASSMANAGER_H_
#include "renderManager/renderPassManager.h"
#endif

class AudioEnvironment;

//*****************************************************************************
// WaterBlock
//*****************************************************************************
class WaterBlock : public SceneObject
{
   typedef SceneObject Parent;

public:

   // LEGACY support
   enum EWaterType
   {
      eWater            = 0,
      eOceanWater       = 1,
      eRiverWater       = 2,
      eStagnantWater    = 3,
      eLava             = 4,
      eHotLava          = 5,
      eCrustyLava       = 6,
      eQuicksand        = 7,
   };

   enum MaterialType
   {
      BASE_PASS = 0,
      UNDERWATER_PASS = 1,
      FOG_PASS = 2,
      BLEND = 3,
      NO_REFLECT = 4,
      NUM_MAT_TYPES
   };

private:

   enum MaskBits {
      InitialUpdateMask = Parent::NextFreeMask,
      UpdateMask =   InitialUpdateMask << 1,
      NextFreeMask = UpdateMask << 1
   };

   enum consts
   {
      MAX_WAVES = 3,
      NUM_ANIM_FRAMES = 32,
   };

   struct WaterMatParams
   {
      MaterialParameterHandle* mWaveDataSC;
      MaterialParameterHandle* mTimeDataSC;
      MaterialParameterHandle* mWaveTexScaleSC;
      MaterialParameterHandle* mReflectTexSizeSC;
      MaterialParameterHandle* mBaseColorSC;
      MaterialParameterHandle* mMiscParamsSC;     

      void init(BaseMatInstance* matInst);
   };

   // wave parameters   
   Point2F  mWaveDir[ MAX_WAVES ];
   F32      mWaveSpeed[ MAX_WAVES ];
   Point2F  mWaveTexScale[ MAX_WAVES ];
   
   // vertex / index buffers
   Vector< GVertexBufferHandle<GVertexPCT> * > mVertBuffList;
   Vector< GPrimitiveBufferHandle * >          mPrimBuffList;
   GVertexBufferHandle<GVertexPC>  mRadialVertBuff;
   GPrimitiveBufferHandle           mRadialPrimBuff;

   // misc
   bool           mRender11;
   bool           mFullReflect;
   F32            mGridElementSize;
   U32            mWidth;
   U32            mHeight;
   F32            mElapsedTime;
   ColorI         mBaseColor;
   ColorI         mUnderwaterColor;
   F32            mClarity;
   F32            mFresnelBias;
   F32            mFresnelPower;
   F32            mVisibilityDepth;
   bool           mRenderFogMesh;
   Point3F        mPrevScale;
   GTexHandle   mBumpTex;
   EWaterType     mLiquidType;            ///< Type of liquid: Water?  Lava?  What?

   // reflect plane
   ReflectPlane   mReflectPlane;
   U32            mReflectTexSize;

   // materials
   const char     *  mSurfMatName[NUM_MAT_TYPES];
   BaseMatInstance *  mMatInstances[NUM_MAT_TYPES];
   WaterMatParams mMatParamHandles[NUM_MAT_TYPES];
   AlignedArray<Point2F> mConstArray;

   // Stateblocks
   GStateBlockRef mUnderwaterSB;

   ObjectRenderInst::RenderDelegate mRenderDelegate;

   // for reflection update interval
   U32 mRenderUpdateCount;
   U32 mReflectUpdateCount;
   U32 mReflectUpdateTicks;



   SceneGraphData setupSceneGraphInfo( SceneState *state );
   void setShaderParams(BaseMatInstance* mat, const WaterMatParams& paramHandles);
   void setShaderXForms(BaseMatInstance* mat);
   void setupVBIB();
   void setupVertexBlock( U32 width, U32 height, U32 rowOffset );
   void setupPrimitiveBlock( U32 width, U32 height );
   void drawUnderwaterFilter();
   void render1_1( SceneState *, SceneGraphData &sgData, const Point3F &camPosition );
   void render2_0( SceneState *, SceneGraphData &sgData, const Point3F &camPosition );
   void animBumpTex( SceneState *state );
   void setupRadialVBIB();
   void setMultiPassProjection();
   void clearVertBuffers();

   void initMaterials();   
   void cleanupMaterials();
protected:

   //-------------------------------------------------------
   // Standard engine functions
   //-------------------------------------------------------
   bool onAdd();
   void onRemove();
   bool prepRenderImage  ( SceneState *state, const U32 stateKey, const U32 startZone, const bool modifyBaseZoneState=false);
   void renderObject(ObjectRenderInst *ri, BaseMatInstance* overrideMat);



   bool castRay(const Point3F &start, const Point3F &end, RayInfo* info);

public:
   WaterBlock();
   virtual ~WaterBlock();

   bool isPointSubmerged ( const Point3F &pos, bool worldSpace = true ) const{ return true; }
   AudioEnvironment * getAudioEnvironment(){ return NULL; }

   static void initPersistFields();

   EWaterType getLiquidType() const                 { return mLiquidType; }
   bool isUnderwater( const Point3F &pnt );

   virtual void updateReflection();

   DECLARE_CONOBJECT(WaterBlock);

};







#endif
