/********************************************************************
	created:	2008/03/31
	created:	31:3:2008   20:08
	file base:	FairyBulletCallbacks
	file ext:	h
	author:		����
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
			// ����BulletFlow�Ļص�����Skill����BulletFlowʱ���� [4/14/2008 JiangWei]
	};

	struct AfterCreatedBulletSystemCallback
	{
		virtual void	onAfterCreatedBulletSystem(BulletFlowSystem* bulletFlow,BulletSystem* bulletSystem) = 0;
		
			// ��ÿ��BulletSystem��������󣬴˻ص�����һ��,��Ҫ�����û�����userData [4/14/2008 JiangWei]
		
	};

	struct BeforeDeleteBulletSystemCallback
	{ 
		virtual void	onBeforeDeleteBulletSystem(BulletSystem* bulletSystem) = 0;
			// ��ÿ��BulletSystem��ɾ��֮ǰ���˻ص�����һ�Σ���Ҫ�����û�����userData [4/14/2008 JiangWei]
	};

	struct BulletSystemHitTargetCallback
	{
		// ��һ��Bullet����Ŀ��͵��ô˻ص�һ��,�û�����ͨ��m_userData����ȡ�û���Ϣ [4/17/2008 JiangWei]
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