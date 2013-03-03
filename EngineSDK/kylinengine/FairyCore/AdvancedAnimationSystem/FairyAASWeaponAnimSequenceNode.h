/********************************************************************
	created:	2008/04/08
	created:	8:4:2008   16:05
	file base:	FairyAASWeaponAnimSequenceNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASWeaponAnimSequenceNode_h__
#define FairyAASWeaponAnimSequenceNode_h__

#include "FairyAASAnimSequenceNode.h"

#include "OgreAnimationState.h"

namespace Fairy
{
	class AdvancedAnimationSystem;

	class AASWeaponAnimSequenceNode : public AASAnimSequenceNode
	{
	public:
		AASWeaponAnimSequenceNode(AdvancedAnimationSystem* system,Ogre::AnimationState* animationState);
		virtual ~AASWeaponAnimSequenceNode();

		virtual	void	evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);
	protected:
	private:
	};
}
#endif // FairyAASWeaponAnimSequenceNode_h__