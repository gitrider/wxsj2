/********************************************************************
	created:	2008/04/09
	created:	9:4:2008   11:47
	file base:	FairyAASPort
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyAASPort_h__
#define FairyAASPort_h__

#include "Core/FairyPrerequisites.h"
#include "FairyAdvancedAnimationSystem.h"

namespace Fairy
{
	class AASNode;

	class AASPort
	{
	public:
		AASPort();
		virtual ~AASPort();

		AASNode*		getParentNode() const;
		void			setParentNode(AASNode* val);

		AASNode*		getConnectedNode() const;
		void			setConnectedNode(AASNode* val);

		void			evaluateAnimation(const float elapsedTime,ReachedAnimSequenceNodeCollection& 
			reachedAnimSequenceNodeCollection);

	protected:
		AASNode* m_parentNode;
		AASNode* m_connectedNode;

	private:
	};
}
#endif // FairyAASPort_h__