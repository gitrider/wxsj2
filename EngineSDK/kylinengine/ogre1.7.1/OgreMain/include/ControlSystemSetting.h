#ifndef __CONTROL_SYSTEM_SETTING_H__
#define __CONTROL_SYSTEM_SETTING_H__
#include "OgrePrerequisites.h"
#include "OgreString.h"

namespace Ogre
{
	class _OgreExport SystemSetting
	{
	public:
		static void	forceMaterial( const String&sName );
		static const String&		getForceMaterialName();
		static const MaterialPtr	getForceMaterial();
		static void	EnableMaterialSameName( bool b );
		static bool	IsEnableMaterialSameName();

		static void forcePixelShader( bool b );
		static bool isForcePixelShader();

		static bool isScriptExceptionCatch();
		static void setScriptExceptionCatch( bool b);

		static void updateMaterialPixelShader( Ogre::MaterialPtr &mPtr );
		static void removeExtraMaterialPixelShader( Ogre::MaterialPtr &mPtr );
	};
}

#endif

