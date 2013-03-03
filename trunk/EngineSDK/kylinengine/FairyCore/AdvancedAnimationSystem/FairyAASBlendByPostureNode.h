/********************************************************************
	created:	2008/04/07
	created:	7:4:2008   17:09
	file base:	FairyAASBlendByPostureNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASBlendByPostureNode_h__
#define FairyAASBlendByPostureNode_h__

#include "FairyAASCrossFadeByTimeNode.h"

namespace Fairy
{
	class AdvancedAnimationSystem;

	class AASBlendByPostureNode : public AASCrossFadeByTimeNode
	{
	public:
		AASBlendByPostureNode(AdvancedAnimationSystem* system);
		virtual ~AASBlendByPostureNode();

		virtual	void	evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);
	protected:
	private:
	};
}
#endif // FairyAASBlendByPostureNode_h__