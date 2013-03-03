/********************************************************************
	created:	2008/04/07
	created:	7:4:2008   20:19
	file base:	FairyAASBlendByLifeNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASBlendByLifeNode_h__
#define FairyAASBlendByLifeNode_h__

#include "FairyAASCrossFadeByTimeNode.h"

namespace Fairy
{
	class AdvancedAnimationSystem;

	class AASBlendByLifeNode : public AASCrossFadeByTimeNode
	{
	public:
		AASBlendByLifeNode(AdvancedAnimationSystem* system);
		virtual ~AASBlendByLifeNode();

		virtual	void	evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);
	protected:
	private:
	};
}
#endif // FairyAASBlendByLifeNode_h__