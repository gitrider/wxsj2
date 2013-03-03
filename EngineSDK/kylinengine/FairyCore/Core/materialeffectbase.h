#ifndef __MATERIAL_BASE_EFFECT_H__
#define __MATERIAL_BASE_EFFECT_H__

#include "OgreString.h"
#include "OgreController.h"

#define MATERIAL_EFFECT_DIFFUSE			1
#define MATERIAL_EFFECT_EMISSIVE		2
#define MATERIAL_EFFECT_SPECULAR		3

namespace Ogre
{
	class MovableObject;
};

struct __MDump
{
	struct __OneMaterialReplace
	{
		Ogre::String mOriginMaterialName;
		Ogre::String mNewMaterialName;
	};

	Ogre::MovableObject*	mpObj;
	std::vector< __OneMaterialReplace >		mArray;
	bool			mBDumpMaterials;

	__MDump( Ogre::MovableObject *pObj );
	virtual ~__MDump();

	void dumpMaterial();
	
	
	
};

struct MTimeLimitEffect : public __MDump
{
	MTimeLimitEffect( Ogre::MovableObject *pObj );
	~MTimeLimitEffect();

	void	createMaterialTimeControl();
	void	destroyMaterialTimeControl();
	virtual void applyTimeChange ( float nTime )	{}
	Ogre::Controller< Ogre::Real > *		mController;
};

struct MTransEffect : public __MDump
{
	MTransEffect( Ogre::MovableObject *pObj );
	void setTrans( float nNewAlpha );
};

#endif
