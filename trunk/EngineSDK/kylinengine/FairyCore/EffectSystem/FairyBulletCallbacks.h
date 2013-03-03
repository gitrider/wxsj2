/********************************************************************
	created:	2008/03/31
	created:	31:3:2008   20:08
	file base:	FairyBulletCallbacks
	file ext:	h
	author:		姜威
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletCallbacks_h__
#define FairyBulletCallbacks_h__

#include "Core/FairyPrerequisites.h"
#include "EffectSystem/FairyEffectBasic.h"
namespace Fairy
{
	class BulletFlowSystem;
	class BulletSystem;
	class Skill;

	struct CreateBulletFlowCallback
	{
		virtual void	onCreateBulletFlow(const String& bulletFlowTemplateName,
			const TransformInfo& transInfo,void* skillUserData) = 0;
			// 创建BulletFlow的回调，供Skill创建BulletFlow时调用 [4/14/2008 JiangWei]
	};

	struct AfterCreatedBulletSystemCallback
	{
		virtual void	onAfterCreatedBulletSystem(BulletFlowSystem* bulletFlow,BulletSystem* bulletSystem) = 0;
		
			// 当每个BulletSystem被创建完后，此回调调用一次,主要是让用户处理userData [4/14/2008 JiangWei]
		
	};

	struct BeforeDeleteBulletSystemCallback
	{ 
		virtual void	onBeforeDeleteBulletSystem(BulletSystem* bulletSystem) = 0;
			// 当每个BulletSystem被删除之前，此回调调用一次，主要是让用户处理userData [4/14/2008 JiangWei]
	};

	struct BulletSystemHitTargetCallback
	{
		// 当一个Bullet打中目标就调用此回调一次,用户可以通过m_userData来获取用户信息 [4/17/2008 JiangWei]
		virtual void	onHitTargetCallback(BulletFlowSystem* bulletFlow,BulletSystem* bulletSystem) = 0;
	};

	struct DestroySkillCallback
	{
		virtual void onDestroySkill(Skill* pSkill) = 0;
	};

	struct CameraShakeCallback
	{
		virtual void onShakeCallBack(float amplitude,float duration,float requency) = 0;
	};
}

#endif // FairyBulletCallbacks_h__