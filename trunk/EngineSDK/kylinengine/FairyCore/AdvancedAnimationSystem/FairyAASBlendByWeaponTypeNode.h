/********************************************************************
	created:	2008/04/07
	created:	7:4:2008   17:20
	file base:	FairyAASBlendByWeaponTypeNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASBlendByWeaponTypeNode_h__
#define FairyAASBlendByWeaponTypeNode_h__

#include "FairyAASContinuousNode.h"

namespace Fairy
{
	class AdvancedAnimationSystem;

	class AASBlendByWeaponTypeNode : public AASContinuousNode
	{
	public:
		AASBlendByWeaponTypeNode(AdvancedAnimationSystem* system);
		virtual ~AASBlendByWeaponTypeNode();

		virtual	void	evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);
	protected:
	private:
	};
}
#endif // FairyAASBlendByWeaponTypeNode_h__