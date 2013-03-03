/********************************************************************
	created:	2008/04/07
	created:	7:4:2008   17:09
	file base:	FariyAASBlendByIdleNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FariyAASBlendByIdleNode_h__
#define FariyAASBlendByIdleNode_h__

#include "FairyAASCrossFadeByTimeNode.h"

namespace Fairy
{
	class AdvancedAnimationSystem;

	class AASBlendByIdleNode : public AASCrossFadeByTimeNode
	{
	public:
		AASBlendByIdleNode(AdvancedAnimationSystem* system);
		virtual ~AASBlendByIdleNode();

		virtual	void	evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);
	protected:
	private:
	};
}
#endif // FariyAASBlendByIdleNode_h__