/********************************************************************
	created:	2008/04/09
	created:	9:4:2008   16:59
	file base:	FairyAASBlendByIdleNodeFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASBlendByIdleNodeFactory_h__
#define FairyAASBlendByIdleNodeFactory_h__

#include "FairyPrerequisites.h"
#include "FairyAASNodeFactory.h"
#include "FairyAASBlendByIdleNode.h"
#include "FairyAdvancedAnimationSystem.h"

namespace Fairy
{
	class AASBlendByIdleNodeFactory : public AASNodeFactory
	{
	public:

		virtual String getName() const 
		{
			return "BlendByIdle";
		}

		virtual AASNode* createNode(AdvancedAnimationSystem* system)
		{
			AASNode* tmpNode = new AASBlendByIdleNode(system);
			m_nodes.push_back(tmpNode);

			return tmpNode;
		}
	protected:
	private:
	};
}
#endif // FairyAASBlendByIdleNodeFactory_h__