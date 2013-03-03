/********************************************************************
	created:	2008/04/07
	created:	7:4:2008   16:33
	file base:	FairyAdvancedAnimationSystem
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAdvancedAnimationSystem_h__
#define FairyAdvancedAnimationSystem_h__

#include <vector>

namespace Fairy
{
	class AASAnimSequenceNode;
	class AASNode;

	struct	ReachedAnimSequenceNodeCollection
	{
		std::vector<AASAnimSequenceNode*> m_reachedAnimSequenceNodeVec;		
	};

	class AdvancedAnimationSystem
	{
	public:
						AdvancedAnimationSystem();
		virtual			~AdvancedAnimationSystem();

		void			update(float time);

		AASNode*		getAnimTreeOutputNode() const;
		void			setAnimTreeOutputNode(AASNode* val);
	protected:
		AASNode*		m_animTreeOutputNode;
		ReachedAnimSequenceNodeCollection	m_reachedAnimSequenceNodeCollection;

	private:
	};
}
#endif // FairyAdvancedAnimationSystem_h__