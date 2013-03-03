//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "graphic/bitmap/gBitmap.h"
#include "math/mathIO.h"
#include "core/stream/bitStream.h"
#include "console/consoleTypes.h"
#include "sceneManager/sceneManager.h"
#include "terrain/terrData.h"
#include "math/mathUtils.h"
#include "terrain/sun.h"
#include "sceneManager/lightInfo.h"
#include "sceneManager/lightManager.h"

IMPLEMENT_CO_NETOBJECT_V1(Sun);

//-----------------------------------------------------------------------------

Sun::Sun()
{
   
   mTypeMask = EnvironmentObjectType;

   // Light properties
   mLightColor.set(0.7f, 0.7f, 0.7f);
   mLightAmbient.set(0.3f, 0.3f, 0.3f);
   
   mSunAzimuth = 0.0f;
   mSunElevation = 35.0f;
   mLight = NULL;
   mRegisteredLight = NULL;
}

Sun::~Sun()
{
   SAFE_DELETE(mLight);
   SAFE_DELETE(mRegisteredLight);
}

void Sun::createLightIfNeeded()
{
   if (!mLight)
   {
      LightManager* lm = gSceneManager->getLightManager();
      mLight = lm->createLightInfo();
      mRegisteredLight = lm->createLightInfo();

      mLight->mType = LightInfo::Vector;   
      mLight->mDirection.set(0.f, 0.707f, -0.707f);
      conformLight();
   }
}

//-----------------------------------------------------------------------------

void Sun::conformLight()
{
   // Update light properties from field properties
   mLight->mColor = mLightColor;
   mLight->mAmbient = mLightAmbient;

   // Normalize & clamp!
   mLight->mDirection.normalize();
   mLight->mColor.clamp();
   mLight->mAmbient.clamp();
}

//-----------------------------------------------------------------------------

bool Sun::onAdd()
{
   if(!Parent::onAdd())
      return(false);

   createLightIfNeeded();

   // Used to copy mLightColor, etc to light
   conformLight();


   Sim::getLightSet()->addObject(this);

   return(true);
}

void Sun::onRemove()
{
   Parent::onRemove();
   SAFE_DELETE(mLight);   
   SAFE_DELETE(mRegisteredLight);
}

void Sun::registerLights(LightManager *lightManager, bool relight)
{
   mRegisteredLight->set(mLight);   

	if(relight)
	{
		// static lighting not affected by this option when using the sun...		
		lightManager->registerGlobalLight(mRegisteredLight, this, true);
	}	
   // Always do this now, allows the lighting system to manipulate the sun light if needed.
	lightManager->setSpecialLight(LightManager::slSunLightType, mRegisteredLight);
}


void Sun::initPersistFields()
{
   Parent::initPersistFields();
   addGroup("Misc");	
   addField("azimuth",     TypeF32,       Offset( mSunAzimuth, Sun));
   addField("elevation",   TypeF32,       Offset( mSunElevation, Sun));
   //addField("direction", TypePoint3F,   Offset(mLight->mDirection, Sun));
   addField("color",       TypeColorF,    Offset(mLightColor, Sun));
   addField("ambient",     TypeColorF,    Offset(mLightAmbient, Sun));  
   endGroup("Misc");	
}