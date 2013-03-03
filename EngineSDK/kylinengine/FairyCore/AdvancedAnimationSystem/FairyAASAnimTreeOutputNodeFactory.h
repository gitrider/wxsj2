/********************************************************************
	created:	2008/04/10
	created:	10:4:2008   9:38
	file base:	FairyAASAnimTreeOutputNodeFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASAnimTreeOutputNodeFactory_h__
#define FairyAASAnimTreeOutputNodeFactory_h__

#include "FairyPrerequisites.h"
#include "FairyAASAnimTreeOutputNode.h"
#include "FairyAASNodeFactory.h"

namespace Fairy
{
	class AASAnimTreeOutputNodeFactory : public AASNodeFactory
	{
	public:
		virtual String getName() const
		{
			return "AnimTree";
		}

		virtual AASNode* createNode(AdvancedAnimationSystem* system)
		{
			AASNode* tmpNode = new AASAnimTreeOutputNode(system);
			m_nodes.push_back(tmpNode);

			return tmpNode;
		}
	protected:
	private:
	};
}
#endif // FairyAASAnimTreeOutputNodeFactory_h__