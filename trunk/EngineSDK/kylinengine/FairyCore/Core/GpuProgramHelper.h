#pragma once

//#include "fairysystem.h"
#include "ogrevector4.h"

namespace Fairy
{
	class System;
};
/// define all special, or unknown gpu shaders

/// GLOBAL PARAMS define
/// 0 - 999 
/// 
#define GPU_PROGRAM_PARAM_WIND						60
#define GPU_PROGRAM_PARAM_TERRAIN_COLOUR_MODULATE	61
#define GPU_PROGRAM_PARAM_MODEL_COLOUR_MODULATE	62

#define GPU_PROGRAM_PARAP_TEST		90

///
/// Grass effect
///  grass_vs_wind_1 ---- grass_vs_wind_2 ....

class GpuProgramHelper
{
public:
	GpuProgramHelper();
	~GpuProgramHelper(void);

	void setGlobalParam( int n, const Ogre::Vector4 &v );
	void setTerrainColourModulate(  const Ogre::Vector4 &v );
	void setModelColourModulate( const Ogre::Vector4 &v );
	void setWindforce( const Ogre::Vector4 &wind );

	void setSystem( Fairy::System*	pSystem )	{mSystem=pSystem;}

protected:
	Fairy::System*		mSystem;

};
