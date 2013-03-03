/********************************************************************
	created:	2008/04/07
	created:	7:4:2008   17:19
	file base:	FairyAASBlendByDamageNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASBlendByDamageNode_h__
#define FairyAASBlendByDamageNode_h__

#include "FairyAASContinuousNode.h"

namespace Fairy
{
	class AdvancedAnimationSystem;

	class AASBlendByDamageNode : public AASContinuousNode
	{
	public:
		AASBlendByDamageNode(AdvancedAnimationSystem* system);
		virtual ~AASBlendByDamageNode();

		virtual	void	evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);
	protected:
	private:
	};
}
#endif // FairyAASBlendByDamageNode_h__