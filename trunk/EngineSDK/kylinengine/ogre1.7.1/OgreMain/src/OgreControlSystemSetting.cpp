#include "OgreStableHeaders.h"
#include "OgreControlSystemSetting.h"
#include "OgreMaterialManager.h"

 namespace Ogre
{
	static String szSetting("ForceMaterialName");
	void	SystemSetting::forceMaterial( const String&sName )
	{

	}

	const String&	SystemSetting::getForceMaterialName()
	{
		return szSetting;
	}

	const MaterialPtr	SystemSetting::getForceMaterial()
	{
		 MaterialPtr m = MaterialManager::getSingleton().getByName( getForceMaterialName() );
		 return m;
	}

	void	SystemSetting::EnableMaterialSameName( bool b )
	{

	}
	bool	SystemSetting::IsEnableMaterialSameName()
	{
		return false;
	}
	void SystemSetting::forcePixelShader( bool b )
	{

	}
	bool SystemSetting::isForcePixelShader()
	{
		return false;
	}

	bool SystemSetting::isScriptExceptionCatch()
	{
		return true;

	}
	void SystemSetting::setScriptExceptionCatch( bool b)
	{

	}

	void SystemSetting::updateMaterialPixelShader( Ogre::MaterialPtr &mPtr )
	{

	}
	void SystemSetting::removeExtraMaterialPixelShader( Ogre::MaterialPtr &mPtr )
	{

	}
};