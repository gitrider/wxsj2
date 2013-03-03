//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _SUN_H_
#define _SUN_H_

#ifndef _COLOR_H_
#include "core/color.h"
#endif
#ifndef _SIMOBJECT_H_
#include "console/simObject.h"
#endif

class LightInfo;

/// This Sun object should support different types of lighting systems because of the
/// lighting system createLightInfo factory method.  So other lighting systems should
/// only need to derive from here to add more information to the lightinfo.
class Sun : public SimObject
{
private:
   typedef SimObject Parent;
protected:
   ColorF mLightColor;
   ColorF mLightAmbient;

   LightInfo* mLight;
   LightInfo* mRegisteredLight;

   virtual void createLightIfNeeded();
   virtual void conformLight();   
public:

   Sun();
   ~Sun();

   // SimObject
   virtual bool onAdd();
   virtual void onRemove();
   virtual void registerLights(LightManager *lm, bool lightingScene);

   //
   

   static void initPersistFields();


   F32		mSunAzimuth;
   F32		mSunElevation;

   DECLARE_CONOBJECT(Sun);
};

#endif
