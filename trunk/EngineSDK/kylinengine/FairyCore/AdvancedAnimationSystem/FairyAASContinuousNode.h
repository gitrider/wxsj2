/********************************************************************
	created:	2008/04/07
	created:	7:4:2008   17:16
	file base:	FairyAASContinuousNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASContinuousNode_h__
#define FairyAASContinuousNode_h__

#include "FairyAASNode.h"

namespace Fairy
{
	class AdvancedAnimationSystem;

	class AASContinuousNode : public AASNode
	{
	public:
		AASContinuousNode(AdvancedAnimationSystem* system);
		virtual ~AASContinuousNode();

		virtual	void	evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);
	protected:
	private:
	};
}
#endif // FairyAASContinuousNode_h__