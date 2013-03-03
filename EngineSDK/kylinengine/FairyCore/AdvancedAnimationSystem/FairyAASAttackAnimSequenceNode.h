/********************************************************************
	created:	2008/04/08
	created:	8:4:2008   16:07
	file base:	FairyAASAttackAnimSequenceNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASAttackAnimSequenceNode_h__
#define FairyAASAttackAnimSequenceNode_h__

#include "FairyAASAnimSequenceNode.h"

#include "OgreAnimationState.h"

namespace Fairy
{
	class AdvancedAnimationSystem;

	class AASAttackAnimSequenceNode : public AASAnimSequenceNode
	{
	public:
		AASAttackAnimSequenceNode(AdvancedAnimationSystem* system,Ogre::AnimationState* animationState);
		virtual ~AASAttackAnimSequenceNode();

		virtual	void	evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);
	protected:
	private:
	};
}
#endif // FairyAASAttackAnimSequenceNode_h__