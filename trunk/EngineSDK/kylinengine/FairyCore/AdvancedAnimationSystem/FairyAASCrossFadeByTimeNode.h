/********************************************************************
	created:	2008/04/07
	created:	7:4:2008   16:55
	file base:	FairyAASCrossFadeByTimeNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASCrossFadeByTimeNode_h__
#define FairyAASCrossFadeByTimeNode_h__

#include "FairyAASCrossFadeNode.h"

namespace Fairy
{
	class AdvancedAnimationSystem;

	class AASCrossFadeByTimeNode : AASCrossFadeNode
	{
	public:
		AASCrossFadeByTimeNode(AdvancedAnimationSystem* system);
		virtual ~AASCrossFadeByTimeNode();

		virtual	void	evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);
	protected:
	private:
	};
}
#endif // FairyAASCrossFadeByTimeNode_h__