/********************************************************************
	created:	2008/04/07
	created:	7:4:2008   16:50
	file base:	FairyAASCrossFadeNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASCrossFadeNode_h__
#define FairyAASCrossFadeNode_h__

#include "FairyAASNode.h"

namespace Fairy
{
	class AdvancedAnimationSystem;

	class AASCrossFadeNode : public AASNode
	{
	public:
		AASCrossFadeNode(AdvancedAnimationSystem* system);
		virtual ~AASCrossFadeNode();

		virtual	void	evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);
	protected:
	private:
	};
}
#endif // FairyAASCrossFadeNode_h__