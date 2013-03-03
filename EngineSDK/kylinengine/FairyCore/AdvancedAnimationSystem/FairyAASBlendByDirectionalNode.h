/********************************************************************
	created:	2008/04/08
	created:	8:4:2008   16:13
	file base:	FairyAASBlendByDirectionalNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASBlendByDirectionalNode_h__
#define FairyAASBlendByDirectionalNode_h__

#include "FairyAASCrossFadeByValNode.h"

namespace Fairy
{
	class AdvancedAnimationSystem;

	class AASBlendByDirectionalNode : public AASCrossFadeByValNode
	{
	public:
		AASBlendByDirectionalNode(AdvancedAnimationSystem* system);
		virtual ~AASBlendByDirectionalNode();

		virtual	void	evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);
	protected:
	private:
	};
}
#endif // FairyAASBlendByDirectionalNode_h__