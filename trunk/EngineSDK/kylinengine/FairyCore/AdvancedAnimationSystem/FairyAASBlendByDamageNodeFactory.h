/********************************************************************
	created:	2008/04/10
	created:	10:4:2008   9:45
	file base:	FairyAASBlendByDamageNodeFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASBlendByDamageNodeFactory_h__
#define FairyAASBlendByDamageNodeFactory_h__

#include "FairyPrerequisites.h"
#include "FairyAASNodeFactory.h"
#include "FairyAASBlendByDamageNode.h"

namespace Fairy
{
	class AASBlendByDamageNodeFactory : public AASNodeFactory
	{
	public:
		virtual String getName() const
		{
			return "BlendByDamage";
		}

		virtual AASNode* createNode(AdvancedAnimationSystem* system)
		{
			AASNode* tmpNode = new AASBlendByDamageNode(system);
			m_nodes.push_back(tmpNode);

			return tmpNode;
		}
	protected:
	private:
	};
}
#endif // FairyAASBlendByDamageNodeFactory_h__