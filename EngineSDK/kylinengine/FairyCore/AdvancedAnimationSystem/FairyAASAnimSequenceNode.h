/********************************************************************
	created:	2008/04/08
	created:	8:4:2008   16:02
	file base:	FairyAASAnimSequenceNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	 
*********************************************************************/
#ifndef FairyAASAnimSequenceNode_h__
#define FairyAASAnimSequenceNode_h__

#include "FairyAASNode.h"

#include "OgreAnimationState.h"

namespace Fairy
{
	class AdvancedAnimationSystem;
	class AASPort;

	class AASAnimSequenceNode : public AASNode
	{
	public:
		AASAnimSequenceNode(AdvancedAnimationSystem* system,Ogre::AnimationState* animationState);
		virtual ~AASAnimSequenceNode();

		Ogre::AnimationState* getAnimState() const;
		void setAnimState(Ogre::AnimationState* val);

		virtual	void	evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);
	protected:
		Ogre::AnimationState*	m_animState;

	private:
	};
}
#endif // FairyAASAnimSequenceNode_h__