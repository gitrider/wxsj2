
#ifndef __FairyBulletEventSystem_h__
#define __FairyBulletEventSystem_h__

#include "OgreStringInterface.h"
#include <vector>
#include "Core/FairyPrerequisites.h"

namespace Fairy
{
	class BulletOperator;
	class BulletSystem;
	class BulletFlowSystem;
	class BulletFlowSystemManager;

	struct BulletSystemCollection 
	{
		std::vector<BulletSystem*> m_bulletSystemCollectition;
	};

	class BulletEventSystem : public Ogre::StringInterface
	{
	public:
		BulletEventSystem(const String& templateName,BulletFlowSystem* parent);
		virtual ~BulletEventSystem();

		BulletEventSystem& operator = (const BulletEventSystem& rhs);

		void addActiveBulletSystem(BulletSystem* bulletSystem);
		
		void addNewTransferredBulletSystem(BulletSystem* bulletSystem);
		//void				clearNewTransferredBulletSystemList();

		void removeActiveBulletSystem(BulletSystem* bulletSystem);

		void clearActiveBulletSystem(BulletSystem* bulletSystem);

		void removeAllActiveBulletSystem();

		enum BulletOperatorAddMode
		{
			BOAM_APPEND,
			BOAM_INSERT
		};
		BulletOperator*		addBulletOperator(const String& typeName,
			BulletOperatorAddMode addMode = BOAM_APPEND,
			unsigned int insertIndex = 0);
		void				removeBulletOperator(unsigned int index);
		void				removeBulletOperator(BulletOperator* pOperator);
		void				removeAllBulletOperators();
		int					getNumBulletOperator() const;
		BulletOperator*		getBulletOperator(int index) const;

		void				update(Real time);

		// ���¼�Operator�� [1/22/2008 JiangWei]
		void addTransition(const String& eventName, BulletSystem* bulletSystem);

		BulletFlowSystem*	getParent();

		const Fairy::String& getTemplateName() const ;

		std::vector<BulletSystem*>&		getActiveBulletSystem();

		std::vector<BulletSystem*>&		getNewTransferredBulletSystem();

		std::map<String,BulletSystemCollection*>& getTransitionCache();

		typedef std::map<String,BulletSystemCollection*> TransitionMap;
		typedef std::vector<BulletSystem*> BulletSystemVecotor;

	protected:
		String							m_templateName;
		// Ϊ����ȫ�֣�BulletFlowSystem����Operator��׼�� [1/23/2008 JiangWei]
		BulletFlowSystem*				m_parent;
		std::vector<BulletOperator*>	m_operators;
		BulletSystemVecotor				m_activeBulletSystem;
		// BulletSystemǨ�ƻ��棬ÿ֡��ˢ�¡���� [1/22/2008 JiangWei]
		TransitionMap					m_transitionCache;
		std::vector<BulletSystem*>		m_newTransferredBulletSystem;
	private:
	};
}
#endif // FairyBulletEventSystem_h__