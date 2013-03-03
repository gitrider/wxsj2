/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:41
	file base:	FairyBulletFlowSystem
	file ext:	h
	author:		姜威
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletFlowSystem_h__
#define FairyBulletFlowSystem_h__

#include "Core/FairyPrerequisites.h"

#include "OgreVector3.h"
#include "OgreStringInterface.h"
#include "OgreSceneNode.h"
#include "Core/FairyObject.h"
#include "Core/FairySystem.h"
#include "Core/FairyBulletObject.h"
#include "FairyEffectBasic.h"
#include "ModelSystem/FairyLogicModel.h"
#include <map>
#include <vector>

namespace Fairy
{
	class BulletEventSystem;
	class BulletSystem;

	
	class BulletFlowSystem : public Ogre::StringInterface
	{
	public:
		BulletFlowSystem(System* system);
		virtual ~BulletFlowSystem();

		bool initParamDictionary(void);

		class CmdMaxTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		Ogre::String& getTemplateName() {return m_templateName;}
		void setTemplaterName(const Ogre::String& templateName){ m_templateName = templateName;}

		BulletFlowSystem& operator = (const BulletFlowSystem& rhs);

		void update(Real time);

		BulletEventSystem*	addBulletEventSystem(const String& name);
		int					getNumBulletEventSystem() const;
		BulletEventSystem*	getBulletEventSystem(const String& name);
		void				removeBulletEventSystem(const String& eventSystemName);
		void				removeBulletEventSystem(BulletEventSystem* eventSystem);
		void				removeAllBulletEventSystem();

		const std::map<String,BulletEventSystem*>& getBulletEventSystemsMap() const;
		// 此函数调用BulletSystemManager::createBulletSystem函数,另外BulletSystem
		// 由单独的BulletSystemManager管理，而不在BulletFlowSystemManager里面管理[1/23/2008 JiangWei]
		BulletSystem*		addBulletSystem(const String& name);
		int					getNumBulletSystem() const;
		BulletSystem*		getBulletSystem(int index) const;
		void				removeBulletSystem(unsigned int index);
		void				removeBulletSystem(BulletSystem* bulletSystem);
		void				removeAllBulletSystem();

		void				setPosition(const Ogre::Vector3& position){ m_TransInfo.mPosition =position;}
		Ogre::Vector3		getPosition() const { return m_TransInfo.mPosition;}
		void				setRotation(const Ogre::Quaternion& rotation){ m_TransInfo.mRotation =rotation;}
		Ogre::Quaternion	getRotation() const { return m_TransInfo.mRotation;}

		const LogicModel*		getTargetModel() const {return m_targetModel;}
		void					setTargetModel(LogicModel* model){m_targetModel  = model;}
		const LogicModel*		getCasterModel() const {return m_casterModel;}
		void					setCasterModel(LogicModel* model) { m_casterModel = model;}

		Ogre::Vector3			getOffsetPosition() const {return m_OffsetPos;}
		void					setOffsetPosition(const Ogre::Vector3 position) {m_OffsetPos = position;}
		Ogre::Quaternion		getOffsetRotation() const {return m_OffsetRoation;}
		void					setOffsetRotation(const Ogre::Quaternion rotation) {m_OffsetRoation = rotation;}

		Ogre::String			getAttachPoint()const {return m_AttachPointName;}
		void					setAttachPoint(const Ogre::String& name) { m_AttachPointName = name;}

		Ogre::String			getTargetPoint() const {return m_TargetPointName;}
		void					setTargetPoint(const Ogre::String& name) { m_TargetPointName = name;}

		Fairy::Real				getMaxTime() const;
		void					setMaxTime(Fairy::Real val);

		Fairy::Real				getAge() const;
		void					setAge(Fairy::Real val);

		int						getBirthBulletTimesPerCluster() const;
		void					setBirthBulletTimesPerCluster(int val);

		static CmdMaxTime		ms_maxTimeCmd;

		typedef std::map<String,BulletEventSystem*> BulletEventMap;
		typedef std::vector<BulletSystem*> BulletSystemVector;
	protected:
		void					integrateBulletSystemTransform(Real time);

	protected:
		Ogre::SceneNode*					m_sceneNode;
		System*								m_system;
		// 一个FlowSystem中不可能包含多个同名的EventSystem,否则将抛异常 [1/24/2008 JiangWei]
		BulletEventMap						m_bulletEventSystemMap;
		// 一个FlowSystem中可能包含多个具有相同模板名的BulletSystem [1/24/2008 JiangWei]
		BulletSystemVector					m_bulletSystems;
		bool								m_isCreated;
		bool								m_isActiveEventSystemListInitialised;
		Ogre::Controller<Real>*				m_timeController;
		Real								m_maxTime;
		Real								m_age;
		Fairy::LogicModel*					m_targetModel;
		Fairy::LogicModel*					m_casterModel;
		int									m_birthBulletTimesPerCluster;
		TransformInfo						m_TransInfo;
		String								m_templateName;
		String								m_AttachPointName;
		String								m_TargetPointName;
		Ogre::Vector3						m_OffsetPos;
		Ogre::Quaternion					m_OffsetRoation;
	
	public:
		void*								m_userData;// 用户数据，每个BulletSystem挂接一个，
														//用来挂接客户端对象 [4/14/2008 JiangWei]
	};
}
#endif // FairyBulletFlowSystem_h__