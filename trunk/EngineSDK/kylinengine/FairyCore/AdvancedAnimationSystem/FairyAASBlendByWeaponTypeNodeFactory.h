/********************************************************************
	created:	2008/04/10
	created:	10:4:2008   9:49
	file base:	FairyAASBlendByWeaponTypeNodeFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASBlendByWeaponTypeNodeFactory_h__
#define FairyAASBlendByWeaponTypeNodeFactory_h__

#include "FairyPrerequisites.h"
#include "FairyAASNodeFactory.h"
#include "FairyAASBlendByWeaponTypeNode.h"

namespace Fairy
{
	class AASBlendByWeaponTypeNodeFactory : public AASNodeFactory
	{
	public:
		virtual String getName() const
		{
			return "BlendByWeaponType";
		}

		virtual AASNode* createNode(AdvancedAnimationSystem* system)
		{
			AASNode* tmpNode = new AASBlendByWeaponTypeNode(system);
			m_nodes.push_back(tmpNode);

			return tmpNode;
		}
	protected:
	private:
	};
}
#endif // FairyAASBlendByWeaponTypeNodeFactory_h__