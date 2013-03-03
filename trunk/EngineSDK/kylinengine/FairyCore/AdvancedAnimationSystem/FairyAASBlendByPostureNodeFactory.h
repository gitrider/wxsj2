/********************************************************************
	created:	2008/04/09
	created:	9:4:2008   17:42
	file base:	FairyAASBlendByPostureNodeFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASBlendByPostureNodeFactory_h__
#define FairyAASBlendByPostureNodeFactory_h__

#include "FairyPrerequisites.h"
#include "FairyAASNodeFactory.h"

namespace Fairy
{
	class AASBlendByPostureNodeFactory : public AASNodeFactory
	{
	public:
		virtual String getName() const
		{
			return "BlendByPosture";
		}

		virtual AASNode* createNode(AdvancedAnimationSystem* system)
		{
			AASNode* tmpNode = new AASBlendByPostureNode(system);
			m_nodes.push_back(tmpNode);

			return tmpNode;
		}
	protected:
	private:
	};
}
#endif // FairyAASBlendByPostureNodeFactory_h__