/********************************************************************
	created:	2008/04/10
	created:	10:4:2008   10:31
	file base:	FairyAASWeaponAnimSequenceNodeFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASWeaponAnimSequenceNodeFactory_h__
#define FairyAASWeaponAnimSequenceNodeFactory_h__

#include "FairyPrerequisites.h"
#include "FairyAASNodeFactory.h"
#include "FairyAASWeaponAnimSequenceNode.h"

namespace Fairy
{
	class AASWeaponAnimSequenceNodeFactory : public AASNodeFactory
	{
	public:
		virtual String getName() const
		{
			return "WeaponAnimSequence";
		}

		virtual AASNode* createNode(AdvancedAnimationSystem* system)
		{
			AASNode* tmpNode = new AASWeaponAnimSequenceNode(system);
			m_nodes.push_back(tmpNode);

			return tmpNode;
		}
	protected:
	private:
	};
}
#endif // FairyAASWeaponAnimSequenceNodeFactory_h__