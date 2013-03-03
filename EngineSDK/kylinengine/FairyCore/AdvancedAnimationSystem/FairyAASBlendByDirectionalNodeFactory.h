/********************************************************************
	created:	2008/04/09
	created:	9:4:2008   16:37
	file base:	FairyAASBlendByDirectionalNodeFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASBlendByDirectionalNodeFactory_h__
#define FairyAASBlendByDirectionalNodeFactory_h__

#include "FairyPrerequisites.h"
#include "FairyAASNodeFactory.h"
#include "FairyAASBlendByDirectionalNode.h"
#include "FairyAdvancedAnimationSystem.h"

namespace Fairy
{
	class AASBlendByDirectionalNodeFactory : public AASNodeFactory
	{
	public:

		virtual	String		getName() const
		{
			return "BlendByDirectional";
		}
		virtual AASNode*	createNode(AdvancedAnimationSystem* system)
		{
			AASNode* tmpNode = new AASBlendByDirectionalNodeFactory(system);
			m_nodes.push_back(tmpNode);

			return tmpNode;
		}
	protected:
	private:
	};
}
#endif // FairyAASBlendByDirectionalNodeFactory_h__