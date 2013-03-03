/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:42
	file base:	FairyBulletSystemManager
	file ext:	h
	author:		姜威
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletSystemManager_h__
#define FairyBulletSystemManager_h__

#include "OgreSingleton.h"
#include "OgreStringInterface.h"
#include <OgreDataStream.h>
#include "OgreString.h"
#include "OgreScriptLoader.h"
#include "OgreIteratorWrappers.h"

// fairy header
#include "Core/FairyPrerequisites.h"
#include "Core/FairySystem.h"
#include "Core/FairyResourceSolver.h"

#include <map>

namespace Fairy
{
	class BulletSystem;
	class AnimationEffectInfo;
	class AnimationRibbon;
	class AnimationSound;
	class AnimationSceneLightInfo;
	class AnimationCameraShake;
	class AnimationLine;

	class BulletSystemManager : public Ogre::Singleton<BulletSystemManager>
	{
	public:
		BulletSystemManager(System* system);
		virtual ~BulletSystemManager();
	    static	BulletSystemManager&	getSingleton(void);

		/*
		 *	重载ScriptLoader纯虚函数
		 */
		virtual const StringVector& getScriptPatterns(void) const;
		virtual void parseScript(Ogre::DataStreamPtr& stream, const String& groupName);
		virtual Real getLoadingOrder(void) const;

		void			parseBulletSystemTemplateSegment(const String& templateName,
			Ogre::DataStreamPtr& stream, const String& groupName);

		// 此函数由BulletFlowSystem::addBulletSystem函数调用 [1/23/2008 JiangWei]
		BulletSystem*	createBulletSystem(const Ogre::String& name);
		void			destroyBulletSystem(BulletSystem* bulletSystem);

		BulletSystem*	createBulletSystemTemplate(const String& name);
		void			destroyBulletSystemTemplate(BulletSystem* bulletSystem);
		void			addBulletSystemTemplate(const String& name,BulletSystem* bulletSystem);	
		BulletSystem*	getBulletSystemTemplate(const String& name);

		typedef Ogre::MapIterator<std::map<String,BulletSystem*> > BulletSystemTemplateIterator;
		BulletSystemTemplateIterator getBulletSystemTemplateIterator();

		const Ogre::String & getFileNameByTemplateName( const Ogre::String &templateName );
		void getTemplatesFromScriptFile( const Ogre::String &fileName, StringVector &templates );
		void addToBulletSystemTemplateScriptFileMap( const Ogre::String &templateName, const Ogre::String &fileName );
		void removeBulletSystemTemplateScriptFileMap( const Ogre::String &templateName);

	protected:
		void skipToNextCloseBrace(Ogre::DataStreamPtr& chunk);
		void skipToNextOpenBrace(Ogre::DataStreamPtr& chunk);
		/// 解析.bullet文件
		void parseAttrib(const String& line, BulletSystem* bulletSystem);
		void parseAnimEffectInfo(Ogre::DataStreamPtr &stream, BulletSystem *bulletSystem);	
		void parseAnimEffectInfoAttrib(const String& line, AnimationEffectInfo *effectInfo);
		void parseAnimRibbon(Ogre::DataStreamPtr &stream, BulletSystem *bulletSystem);	
		void parseAnimRibbonAttrib(const String& line, AnimationRibbon *ribbon);
		void parseAnimSound(Ogre::DataStreamPtr &stream, BulletSystem *bulletSystem);	
		void parseAnimSoundAttrib(const String& line, AnimationSound *sound);
		void parseAnimSceneLightInfo(Ogre::DataStreamPtr &stream, BulletSystem *bulletSystem);	
		void parseAnimSceneLightInfoAttrib(const String& line, AnimationSceneLightInfo *sceneLightInfo);
		void parseAnimSceneCamerShake(Ogre::DataStreamPtr &stream, BulletSystem *bulletSystem);	
		void parseAnimSceneCamerShakeAttrib(const String& line, AnimationCameraShake *cameraShake);
		void parseAnimLine(Ogre::DataStreamPtr &stream, BulletSystem *bulletSystem);	
		void parseAnimLineAttrib(const String& line, AnimationLine *animationLine);
		/// 输出错误信息
		void _logErrorInfo(const String& errorInfo, const String& lineContent, 
			const String& functionName);
	public:
		typedef std::map<String,BulletSystem*> BulletSystemMap;
		typedef std::map<String,Ogre::String> BulletSytemTemplateScriptFileMap;


	protected:
		System*						  m_system;
		/// 每次进行getLine的时候就递增行号，作为错误的输出信息
		int mWrongLineNum;
		/// 当前解析的文件名称
		String mParsingFileName;

		BulletSystemMap m_systemTemplate;
		BulletSytemTemplateScriptFileMap m_BulletSytemTemplateScriptFileMap;
		
		StringVector	m_scriptPatterns;
	private:
	};
}

#endif // FairyBulletSystemManager_h__