/********************************************************************
	created:	2008/04/10
	created:	10:4:2008   10:28
	file base:	FairyAASAttackAnimSequenceNodeFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASAttackAnimSequenceNodeFactory_h__
#define FairyAASAttackAnimSequenceNodeFactory_h__

#include "FairyPrerequisites.h"
#include "FairyAASNodeFactory.h"
#include "FairyAASAttackAnimSequenceNode.h"

namespace Fairy
{
	class AASAttackAnimSequenceNodeFactory : public AASNodeFactory
	{
	public:
		virtual String getName() const 
		{
			return "AttackAnimSequence";
		}

		virtual AASNode* createNode(AdvancedAnimationSystem* system)
		{
			AASNode* tmpNode = new AASAttackAnimSequenceNode(system);
			m_nodes.push_back(tmpNode);

			return tmpNode;
		}
	protected:
	private:
	};
}
#endif // FairyAASAttackAnimSequenceNodeFactory_h__