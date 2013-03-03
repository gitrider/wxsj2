/********************************************************************
	created:	2008/04/07
	created:	7:4:2008   16:59
	file base:	FairyAASCrossFadeByValNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASCrossFadeByValNode_h__
#define FairyAASCrossFadeByValNode_h__

#include "FairyAASCrossFadeNode.h"

namespace Fairy
{
	class AdvancedAnimationSystem;

	class AASCrossFadeByValNode : public AASCrossFadeNode
	{
	public:
		AASCrossFadeByValNode(AdvancedAnimationSystem* system);
		virtual ~AASCrossFadeByValNode();

		virtual	void	evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);
	protected:
	private:
	};
		
}
#endif // FairyAASCrossFadeByValNode_h__