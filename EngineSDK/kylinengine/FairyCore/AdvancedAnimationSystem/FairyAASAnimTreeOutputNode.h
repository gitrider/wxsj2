/********************************************************************
	created:	2008/04/09
	created:	9:4:2008   15:07
	file base:	FairyAASAnimTreeOutputNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASAnimTreeOutputNode_h__
#define FairyAASAnimTreeOutputNode_h__

#include "FairyAASNode.h"

namespace Fairy
{
	class AdvancedAnimationSystem;
	class AASPort;

	class AASAnimTreeOutputNode : public AASNode
	{
	public:
		AASAnimTreeOutputNode(AdvancedAnimationSystem* system);
		virtual ~AASAnimTreeOutputNode();

		AASPort*			getInPort() const;
		void				setInPort(AASPort* val);

		virtual	void		evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
							reachedAnimSequenceNodeCollection);
	protected:
		AASPort*			m_inPort;

	private:
	};
}
#endif // FairyAASAnimTreeOutputNode_h__