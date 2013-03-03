/********************************************************************
	created:	2008/04/09
	created:	9:4:2008   17:33
	file base:	FairyAASBlendByLifeNodeFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASBlendByLifeNodeFactory_h__
#define FairyAASBlendByLifeNodeFactory_h__

#include "FairyPrerequisites.h"
#include "FairyAASBlendByLifeNode.h"
#include "FairyAASNodeFactory.h"

namespace Fairy
{
	class AASBlendByLifeNodeFactory : public AASNodeFactory
	{
	public:
		virtual String	getName() const
		{
			return "BlendByLife";
		}

		virtual AASNode* createNode(AdvancedAnimationSystem* system)
		{
			AASNode* tmpNode = new AASBlendByLifeNode(system);
			m_nodes.push_back( tmpNode);

			return tmpNode;
		}

	protected:
	private:
	};
}
#endif // FairyAASBlendByLifeNodeFactory_h__