/********************************************************************
	created:	2008/04/07
	created:	7:4:2008   16:40
	file base:	FairyAASNode
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASNode_h__
#define FairyAASNode_h__

#include "Core/FairyPrerequisites.h"
#include "FairyAdvancedAnimationSystem.h"

#include <map>

namespace Fairy
{
	class AASPort;
	class AdvancedAnimationSystem;

	class AASNode
	{
	public:
		AASNode(AdvancedAnimationSystem* system);
		virtual ~AASNode();

		AASPort*		createPort(const String& name);

		AASPort*		getPort(const String& name);

		virtual	void	evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);

	protected:
		std::map<String,AASPort*>	m_portDictionary;
		AdvancedAnimationSystem*	m_parent;
	private:
	};
}
#endif // FairyAASNode_h__