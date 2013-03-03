#pragma once
#include "fairyeffectelement.h"

namespace Ogre
{
	class RibbonTrail;
};

namespace Fairy
{

class RibbonEffectElement :
	public EffectElement
{
public:
	RibbonEffectElement(const String &type, System *system);
	~RibbonEffectElement(void);

public:
	bool createRenderInstance(void);
	void destroyRenderInstance(void);
	void updateRenderInstance( Real time, TransformInfo &info );

	bool _createRibbonTrail();
	bool _destroyRibbonTrail();
	bool _updateRibbonTrail();

	void	setMaterialName( const String &m );

	const String	getMaterialName() const	{return mMaterialName;}

	 virtual void setVisibleFlag(uint32 flags) ;

	 Real					mWidthStart;
	 Real					mWidthChange;
	 Ogre::ColourValue		mColourChange;
	 Ogre::ColourValue		mColourStart;
	 Real					mTrailLength;
	 Real					mWidthMin;	 
	 int					mTrailSepNumber;					

protected:
	Ogre::RibbonTrail*		m_pRibbonTrail;
	Ogre::SceneNode*		m_pRibbonSceneNode;

	Ogre::String			mMaterialName;	
};

}

