#ifndef __MATERIAL_NORMAL_EFFECT_H__
#define __MATERIAL_NORMAL_EFFECT_H__
#include "materialeffectbase.h"
#include "ogrecolourvalue.h"

struct MEffectTest : public __MDump
{
	MEffectTest( Ogre::MovableObject *pObj );

	void apply();
};

struct MEffectColourChange : public MTimeLimitEffect
{
	MEffectColourChange ( Ogre::MovableObject *pObj );
	void apply( const Ogre::ColourValue &start, const Ogre::ColourValue &end, float nTimeChange );

	void applyTimeChange( float nTime );
	Ogre::ColourValue __getColourValueByTime( float nTime );
protected:
	Ogre::ColourValue			mColourStart;
	Ogre::ColourValue			mColourEnd;
	Ogre::ColourValue			mColourDelta1;
	Ogre::ColourValue			mColourDelta2;
	int							mEffectFlag;
	float						mTimePass;
	float						mTimeChange;
};

struct MEffectExtendPass : public __MDump
{
	MEffectExtendPass( Ogre::MovableObject *pObj );
	bool apply( const Ogre::String &sMaterialName, int nPass );
};

#endif
