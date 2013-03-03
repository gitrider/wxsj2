/********************************************************************
	created:	2008/04/09
	created:	9:4:2008   15:38
	file base:	FairyAASNodeFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASNodeFactory_h__
#define FairyAASNodeFactory_h__

#include "FairyPrerequisites.h"

#include <vector>

namespace Fairy
{

	class AdvancedAnimationSystem;
	class AASNode;

	class AASNodeFactory
	{
	public:
		AASNodeFactory() {}
		virtual ~AASNodeFactory();
		
		virtual	String			getName() const = 0;

		virtual AASNode*		createNode(AdvancedAnimationSystem* system) = 0;

		virtual	void			destroyNode(AASNode* node);
	protected:
		std::vector<AASNode*>	m_nodes;
	private:
	};
}
#endif // FairyAASNodeFactory_h__