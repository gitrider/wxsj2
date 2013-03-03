#pragma once

#include "effectsystem/fairyskill.h"

namespace Fairy
{

/// Skill helper
///			skillhelper		-		skill caster , implement skill callback function
///			

	enum E_ANIM_FALG
	{
		E_ANIM_BASE = 1,
		E_ANIM_ARM = (E_ANIM_BASE << 1 ),
		E_ANIM_BOTH = (E_ANIM_BASE|E_ANIM_ARM),
	};

class ISkillCaster
{
public:
	virtual Ogre::AnimationState* _getAnimationState(const String& animName)	=	0;

//	virtual void	__animEndCallback( const String &sAnim )	=	0;
	virtual void	__stopSound( int nHandle )				=	0;
	virtual bool	__handleAnimationFinish( const String &sAnimName , int nPart)			=	0;
	virtual bool	__handleSkillHitTime( const String &sAnimName , int nHitLie, int nHitCount )			=	0;
	virtual bool	__handleSkillBreakTime( const String &sAnimName, int nBreakLie, int nBreakCount )			=	0;
	virtual bool	__handleSkillShakeTime( const String &sAnimName )			=	0;
	virtual bool	__updateSkill( Skill *pSkill, float oldTime, float currTime ,float nTimePass)				=	0;
};

struct ComposeAnimStatus
{
	Ogre::AnimationState*		mBaseAnimationState;
	String						mBaseAnimName;
	String						mSubAnimName;
	Ogre::AnimationState*		mSubAnimationState;


	ComposeAnimStatus() : mBaseAnimationState(NULL), mSubAnimationState(NULL)	{}
	void EndSubAnim();
	void EndBaseAnim();
	void StartSubAnim( const String &sSubAnim, Ogre::AnimationState* pSubAnimStatus );
	void StartBaseAnim( const String &sNewAnim, Ogre::AnimationState* pSubAnimStatus );
	void addTime( float nDelta );
	void setWeight( float nWeight );

protected:
	void	__prepareForBlend(Ogre::AnimationState* pBaseAnimStatus);
	void	__stopBlendAnim( Ogre::AnimationState* pBaseAnimStatus );
};

class CSkillHelper
{
public:
	CSkillHelper(ISkillCaster * pCaster);
	~CSkillHelper(void);

///
protected:
	Fairy::Skill *			mCurrentSkill;
	ISkillCaster *			mSkillCaster;

	ComposeAnimStatus		mComposeCurrentAnimationState;
	ComposeAnimStatus		mComposeLastAnimationState;

	float					mAnimDelay[2];

	bool					mBHasUpdateSkill;

public:
	void	setOwner( ISkillCaster *p )	{mSkillCaster=p;}
	/// change current skill and do a new anim or skill
	void createSkill(const String& skillName, int nAnimFlag, bool loop, bool anim, Real delayTime);
	void delCurrentSkill(void);
	Fairy::Skill *	getCurrentSkill()	{return mCurrentSkill;}
	void _muteCurrentSkillSound(void);
	bool _changeAnimation(const String& animationName,  Real delay,   int nAnimFlag, bool bLoop);
	void setBaseAnim( const String &sBaseAnim , Real delay,  bool bLoop);
	void setSubAnim( const String &sSubAnim ,  Real delay,bool bLoop );
	const String&	getCurrentBaseAnimName();
	const String&	getCurrentSubAnimName();

///	run event processor, call some occur message
///	call advaceanim let logic model manager time pass, or call advacetime, use this handle event 
/// oldTime, newTime, time postion before anim and after anim, range 0 - 1 ..relate position
	void	runEventProcessor( int nEffectPart, bool bLoopAnim, const String &sAnimName, Real oldTime, Real newTime, Real nTimePass );
	Fairy::Skill *	getSkill()	{return mCurrentSkill;}

	Real		getCurrentBaseAnimTime();
	Real		getCurrentSubAnimTime();

	bool		getCurrentBaseAnimStatus( bool &bLoop, Real &timePos, String &sAnimName );
	bool		getCurrentSubAnimStatus( bool &bLoop, Real &timePos, String &sAnimName  );

	bool		hasAnim( int nFlag );

////	go ahead for a time,	use subanimtime calc event or use mainanimtime calc event
	void AdvanceTime( bool bUseSubAnimTime, Real nTimePass );
	void AdvanceTime(  Real nTimePass );

/// advance anim only
	void AdvanceAnim( Real nTimePass );
};

}


