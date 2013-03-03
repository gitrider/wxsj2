/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:41
	file base:	FairyBulletFlowSystemManager
	file ext:	h
	author:		姜威
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletFlowSystemManager_h__
#define FairyBulletFlowSystemManager_h__

#include "OgreScriptLoader.h"
#include "OgreSingleton.h"
#include "Core/FairySystem.h"
#include "Core/FairyPrerequisites.h"
#include "Core/FairyResourceSolver.h"
#include "EffectSystem/FairyBulletCallbacks.h"
#include "OgreIteratorWrappers.h"
#include <map>
#include <list>
namespace Fairy
{
	class BulletEventSystem;
	class BulletFlowSystem;
	class BulletOperator;
	class BulletSystem;
	class BulletOperatorFactory;
	
	class BulletFlowSystemManager : public Ogre::ScriptLoader,
		public Ogre::Singleton<BulletFlowSystemManager>
	{
	public:
		BulletFlowSystemManager(System* system);
		virtual ~BulletFlowSystemManager();
	    static	BulletFlowSystemManager&	getSingleton(void);

		/*
		*	重载ScriptLoader纯虚函数
		*/
		virtual const StringVector& getScriptPatterns(void) const;
		virtual void				parseScript(Ogre::DataStreamPtr& stream, const String& groupName);
		virtual Real				getLoadingOrder(void) const;

		void						parseBulletFlowTemplateSegment(const String& name,
			Ogre::DataStreamPtr& stream, const String& groupName);

		BulletFlowSystem* createBulletFlowSystem(const String& templateName);
 
		void destroyBulletFlowSystem(BulletFlowSystem* bulletFlowSystem);

		void updateBulletFlowSystem(Ogre::Real time);

		BulletFlowSystem*			createBulletFlowSystemTemplate(const String& name);
		void						destroyBulletFlowSystemTemplate(const String& name);
		void						addBulletFlowSystemTemplate(const String& name,
			BulletFlowSystem* bulletFlowSystem);
		BulletFlowSystem*			getBulletFlowSystemTemplate(const String& name);

		// 此函数由BulletFlowSystem::addBulletEventSystem函数调用 [1/23/2008 JiangWei]
		BulletEventSystem*			createBulletEventSystem(const String& name,BulletFlowSystem* flowSystem);

		void						destroyBulletEventSystem(BulletEventSystem* bulletEventSystem);
		/*
		 *	放弃BulletEventSystemManager，因为EventSystem不需要模板文件，所以原本
		 *	BulletEventSystemManeger的功能全部放在此Manager内实现
		 */
		// 此函数由BulletEventSystem::addBulletOperator函数调用 [1/23/2008 JiangWei]
		BulletOperator*				createBulletOperator(const String& typeName,BulletEventSystem* eventSystem);

		void						destroyBulletOperator(BulletOperator* bulletOperator);

		void						addBulletOperatorFactory(BulletOperatorFactory* operatorFactory);

		const Ogre::String & getFileNameByTemplateName( const Ogre::String &templateName );
		void getTemplatesFromScriptFile( const Ogre::String &fileName, StringVector &templates );
		void addToBulletFlowTemplateScriptFileMap( const Ogre::String &templateName, const Ogre::String &fileName );
		void removeBulletFlowTemplateScriptFileMap( const Ogre::String &templateName);


		CreateBulletFlowCallback* getCreateBulletFlowCallback() const {return m_createBulletFlowCallback;}
		void setCreateBulletFlowCallback(CreateBulletFlowCallback* val) {m_createBulletFlowCallback = val;}

		AfterCreatedBulletSystemCallback* getAfterCreatedBulletSystemCallback() const { return m_afterCreatedBulletSystemCallback;}
		void setAfterCreatedBulletSystemCallback(AfterCreatedBulletSystemCallback* val) { m_afterCreatedBulletSystemCallback = val;}

		BeforeDeleteBulletSystemCallback* getBeforeDeleteBulletSystemCallback() const { return m_beforeDeleteBulletSystemCallback;}
		void setBeforeDeleteBulletSystemCallback(BeforeDeleteBulletSystemCallback* val) {m_beforeDeleteBulletSystemCallback = val;}

		BulletSystemHitTargetCallback* getBulletSystemHitTargetCallback() const { return m_bulletSystemHitTargetCallback;}
		void setBulletSystemHitTargetCallback(BulletSystemHitTargetCallback* val) {m_bulletSystemHitTargetCallback = val;}

		CameraShakeCallback* getBulletCameraShakeCallback() const { return m_CameraShakeCallback;}
		void setBulletCameraShakeCallback(CameraShakeCallback* val) {m_CameraShakeCallback = val;}

		const std::map<String,BulletOperatorFactory*>& getOperatorFactorys(){return m_operatorFactories;}

	protected:
		void registerOperatorFactories();

		void clearOperatorFactories();
		// 解析BulletFlowSystem属性 [1/19/2008 JiangWei]
		void parseAttrib(const String& line,BulletFlowSystem* bulletFlowSystem);
		// 解析新BulletEventSystem [1/19/2008 JiangWei]
		void parseNewBulletEventSystem(const String& name,Ogre::DataStreamPtr& stream,
			BulletFlowSystem* bulletFlowSystem);
		void parseBulletEventSystemAttrib(const String& line,BulletEventSystem* bulletEventSystem);
		// 解析行BulletOperator [1/19/2008 JiangWei]
		void parseNewBulletOperator(const String& name,Ogre::DataStreamPtr& stream,BulletEventSystem* eventSystem);
		void parseBulletOperatorAttrib(const String& line,BulletOperator* bulletOperator);

		void						skipToNextCloseBrace(Ogre::DataStreamPtr& chunk);
		void						skipToNextOpenBrace(Ogre::DataStreamPtr& chunk);
		/// 输出错误信息
		void						_logErrorInfo(const String& errorInfo, const String& lineContent, 
									const String& functionName);
	public:
		typedef std::map<String,BulletFlowSystem*> BulletFlowSystemMap;
		typedef std::map<String,BulletOperatorFactory*> BulletOperatorFactoryMap;
		typedef std::list<BulletFlowSystem *> ActiveFlowSystemList;
		typedef std::map<String,Ogre::String> BulletTemplateScriptFileMap;

		typedef Ogre::MapIterator<BulletFlowSystemMap> BulletFlowTemplateIterator;
		BulletFlowTemplateIterator getBulletFlowTemplateIterator(void);


	protected:

		System*						  m_system;
		/// 每次进行getLine的时候就递增行号，作为错误的输出信息
		int mWrongLineNum;

		StringVector								m_scriptPatterns;
		/// 每次进行getLine的时候就递增行号，作为错误的输出信息
		int											m_WrongLineNum;
		/// 当前解析的文件名称
		String										m_ParsingFileName;
		BulletFlowSystemMap							m_bulletFlowSystemTemplate;
		BulletOperatorFactoryMap					m_operatorFactories;
		BulletTemplateScriptFileMap					m_BulletTemplateScriptFileMap;
		ActiveFlowSystemList						m_ActiveFlowSystemList;

		CreateBulletFlowCallback*					m_createBulletFlowCallback;
		AfterCreatedBulletSystemCallback*			m_afterCreatedBulletSystemCallback;
		BeforeDeleteBulletSystemCallback*			m_beforeDeleteBulletSystemCallback;
		BulletSystemHitTargetCallback*				m_bulletSystemHitTargetCallback;
		CameraShakeCallback*						m_CameraShakeCallback;
	private:
	};
}
#endif // FairyBulletFlowSystemManager_h__