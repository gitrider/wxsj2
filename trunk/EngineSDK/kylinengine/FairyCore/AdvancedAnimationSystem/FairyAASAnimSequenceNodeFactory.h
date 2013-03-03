/********************************************************************
	created:	2008/04/10
	created:	10:4:2008   9:52
	file base:	FairyAASAnimSequenceNodeFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASAnimSequenceNodeFactory_h__
#define FairyAASAnimSequenceNodeFactory_h__

#include "FairyPrerequisites.h"
#include "FairyAASNodeFactory.h"
#include "FairyAASAnimSequenceNode.h"

namespace Fairy
{
	class AASAnimSequenceNodeFactory : public AASNodeFactory
	{
	public:
		virtual String getName() const
		{
			return "AnimSequence";
		}

		virtual AASNode* createNode(AdvancedAnimationSystem* system)
		{
			AASNode* tmpNode = new AASAnimSequenceNode(system);
			m_nodes.push_back(tmpNode);

			return tmpNode;
		}
	protected:
	private:
	};
}
#endif // FairyAASAnimSequenceNodeFactory_h__