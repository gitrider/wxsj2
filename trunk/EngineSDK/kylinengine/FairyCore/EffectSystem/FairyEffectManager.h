/********************************************************************
filename:   FairyEffectManager.h
created:    2005/10/5
author:     Weiliang Xie (feiyurainy@163.com)

purpose:    effect manager manage all effects, skills and path files.
*********************************************************************/

#ifndef __EffectManager_H__
#define __EffectManager_H__

// ogre header
#include <OgreSingleton.h>
#include <OgreScriptLoader.h>
#include <OgreStringVector.h>
#include <OgreDataStream.h>
#include "OgreIteratorWrappers.h"

// fairy header
#include "Core/FairyPrerequisites.h"
#include "FairyBulletCallbacks.h"
//#include "FairyScriptResGet.h"
#include <OgreStringVector.h>
// stl header
#include <map>

// ǰ������
namespace Ogre	{
	class SceneNode;
}

namespace Fairy	{
	class Effect;
	class EffectElementFactory;
	class EffectElement;
	class System;
	class EffectBasic;
	class Skill;
	class AnimationEffectInfo;
	class AnimationBulletFlow;
	class AnimationRibbon;
	class AnimationSound;
    class AnimationSceneLightInfo;
	class AnimationCameraShake;
    class MainSceneLight;
}
namespace Fairy	{	


    /// ��Ч����Ĭ��ΪHIGH
    enum EffectLevel
    {
        EL_LOW,
        EL_MID,
        EL_HIGH,
    };

	class EffectManager : public Ogre::Singleton<EffectManager>, Ogre::ScriptLoader
	{
	public:

		typedef std::map<String, Effect *> EffectTemplateMap;
		//typedef std::map<String, Effect *> EffectMap;
		typedef std::list<Effect *> ActiveEffectList;

		typedef std::list<Effect *> FreeEffectList;
		typedef std::map<String, FreeEffectList *> FreeEffectMap;

		typedef std::map<String, EffectElementFactory *> ElementFactoryMap;

		typedef std::map<String,Ogre::String> EffectTemplateScriptFileMap;
		typedef std::map<String,Ogre::String> SkillTemplateScriptFileMap;
		typedef std::map<String,Ogre::String> ParticleTemplateScriptFileMap;


		typedef std::map<String, Skill *> SkillMap;

	public:

		EffectManager( System *system );
		~EffectManager();

		/// @copydoc ScriptLoader::getScriptPatterns
		const Ogre::StringVector& getScriptPatterns(void) const;
        /// @copydoc ScriptLoader::parseScript
		void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);
        /// @copydoc ScriptLoader::getLoadingOrder
        Ogre::Real getLoadingOrder(void) const;
		
        static EffectManager& getSingleton(void);       
        static EffectManager* getSingletonPtr(void);
		
		/// debug
		void dump(const Ogre::String &fileName);

		/// ������Чģ��������һ���µ���Ч
		Effect *createEffect( const Ogre::String &templateName );

		Skill *getSkill( const Ogre::String &skillName );

		/// ������ЧԪ�ص�����������һ����ЧԪ��
		EffectElement * createElement(const Ogre::String &type);

		/// ������ɾ��һ����Ч
		ActiveEffectList::iterator destroyEffect( Effect *effect );

		/** ��ʱ��ɾ��һ����Ч
			������������ɾ����Ч�����ǰ����ӻ����Ч������ɾ��������
			���е���Ч�������Ա�����
		@param removeParentNode �Ƿ�Ҫ�Ѹ�effect�Ľڵ�����ĸ��ڵ���ж������
			�������effect�Ľڵ���õ���createSceneNode���������Ϊfalse��
			�������effect�Ľڵ���õ���createSceneNode( Ogre::SceneNode *parentNode )���������Ϊtrue
        @param removeNow �Ƿ����Ͼ�ɾ�����effect�����Ϊfalse����ô���effect��������ȫ�������
                         ��ɾ��

        @remarks ���removeNowΪfalse����������ɾ��effect����ʱҪ��֤effect��SceneNode��������effectɾ����
                 ���������ҪremoveNowΪfalse������effect��scnenode���ʹ��createSceneNode()��������scenenode����
                 ��effect�ڲ������Ա�֤scenenode��ɾ��ʱ��
		*/
		void removeEffect( Effect *effect, bool removeParentNode = true, bool removeNow = true );

		/**	����һ��������������ÿ֡����effect	*/
		void createController(void);

		/// �ɿ��������ã�ÿ֡���»����Ч
		void updateActiveEffects(Ogre::Real time);

		size_t getNumEffectTemplates(void);

		typedef Ogre::MapIterator<EffectTemplateMap> EffectTemplateIterator;
		EffectTemplateIterator getEffectTemplateIterator(void);

		typedef Ogre::MapIterator<ElementFactoryMap> ElementFactoryIterator;
		ElementFactoryIterator getElementFactoryIterator(void);

		typedef Ogre::MapIterator<SkillMap> SkillIterator;
		SkillIterator getSkillIterator(void);

		const Ogre::String & getFileNameByTemplateName( const Ogre::String &templateName );
		const Ogre::String & getFileNameBySkillTemplateName( const Ogre::String &templateName );
		const Ogre::String & getFileNameByParticleTemplateName( const Ogre::String &templateName );

		void getTemplatesFromScriptFile( const Ogre::String &fileName, Ogre::StringVector &templates );
		void getSkillTemplatesFromScriptFile( const Ogre::String &fileName, Ogre::StringVector &templates );
		void getParticleTemplatesFromScriptFile( const Ogre::String &fileName, Ogre::StringVector &templates );

		void getLoadedScripts( Ogre::StringVector &files )	{	files = mLoadedScripts;	}
		void getLoadedSkillScripts( Ogre::StringVector &files )	{	files = mLoadedSkillScripts;	}

		/// ����ģ����������ȡģ��
		Effect * getTemplate(const Ogre::String& name);

		Skill * createSkillTemplate(const Ogre::String &name);

		/** ����һ���µ�effectģ��
		@param name ��Ч����
		*/
		Effect * createEffectTemplate(const Ogre::String &name);

		void addToEffectTemplateScriptFileMap( const Ogre::String &templateName, const Ogre::String &fileName );
		void addToSkillTemplateScriptFileMap( const Ogre::String &templateName, const Ogre::String &fileName );
		void addToParticleTemplateScriptFileMap( const Ogre::String &templateName, const Ogre::String &fileName );
		void removeParticleTemplateScriptFileMap( const Ogre::String &templateName);

		Skill * createSkill(const Ogre::String &skillName);
		void removeSkill(Skill *skill);

		/** ����ÿ�����е�effect�������ɱ���Ŀ���effect����
		@remarks effect��removeEffectʱ�ᱻ���գ�����һ��map�У�map��key��effect��ģ�����ƣ�
				 value��һ�����е�effect��������������õľ�������������������Ҳ����˵����
				 ����effect���У�ͬ��ģ�������Ա���mMaxNumFreeEffectPerList�����е�effect
		*/
		void setMaxNumFreeEffectPerList(unsigned short num)
		{
			mMaxNumFreeEffectPerList = num;
		}
		unsigned short getMaxNumFreeEffectPerList(void)
		{
			return mMaxNumFreeEffectPerList;
		}

        MainSceneLight* getMainSceneLight(void)
        {
            return mMainSceneLight;
        }

        /// �����Ч���е���Ч
        void clearAllFreeEffects(void);

        /// ������Ч����
        void setEffectLevel(EffectLevel level);
        EffectLevel getEffectLevel(void)
        {
            return mEffectLevel;
        }

        void _destroyEffectTemplates(void);

        void _destroySkillTemplates(void);

		//void setDestroySkillCallback( DestroySkillCallback *cback );
	protected:

		/** ע���µ���ЧԪ�ع���
			ÿ����һ�������͵���ЧԪ�أ���������������������Ӧ�Ĵ���
		*/
		void registerElementFactory(void);

		void addElementFactory( EffectElementFactory *factory );

		void skipToNextCloseBrace(Ogre::DataStreamPtr& chunk);
        void skipToNextOpenBrace(Ogre::DataStreamPtr& chunk);

		/// �����ű��е��µ���ЧԪ��
		void parseNewElement(const Ogre::String &type, Ogre::DataStreamPtr &stream, Effect *effectParent);

		/// ������ЧԪ�ص����� 
		void parseElementAttrib(const Ogre::String& line, EffectElement *element);

		/// ����.effect�ļ�
		void parseEffectFile( Ogre::DataStreamPtr &stream );
		void parseEffectAttrib(const Ogre::String& line, Effect *effect);
/*
		void parseEffectFile(Ogre::DataStreamPtr &stream , const String& groupName);
		void parseEffectTemlpateSegment( const String& templateName,Ogre::DataStreamPtr &stream , const String& groupName);
		void parseEffectAttrib(const String& line, Effect *effect);
*/
		/// ����.skill�ļ�
		void parseSkillFile( Ogre::DataStreamPtr &stream );
		void parseSkillAttrib(const Ogre::String& line, Skill *skill);
        void parseAnimEffectInfo(Ogre::DataStreamPtr &stream, Skill *skill);	
		void parseAnimEffectInfoAttrib(const Ogre::String& line, AnimationEffectInfo *effectInfo);

		void parseAnimBulletFlow(Ogre::DataStreamPtr &stream,Skill* skill);
		void parseAnimBulletFlowAttrib(const String& line,AnimationBulletFlow* animationBulletFlow);

		void parseAnimRibbon(Ogre::DataStreamPtr &stream, Skill *skill);	
		void parseAnimRibbonAttrib(const Ogre::String& line, AnimationRibbon *ribbon);
		void parseAnimSound(Ogre::DataStreamPtr &stream, Skill *skill);	
		void parseAnimSoundAttrib(const Ogre::String& line, AnimationSound *sound);
        void parseAnimSceneLightInfo(Ogre::DataStreamPtr &stream, Skill *skill);	
        void parseAnimSceneLightInfoAttrib(const Ogre::String& line, AnimationSceneLightInfo *sceneLightInfo);

		void parseAnimCameraShake(Ogre::DataStreamPtr &stream, Skill *skill);	
		void parseAnimCameraShakeAttrib(const Ogre::String& line, AnimationCameraShake *cameraShake);

		/** �ӿ��е���Ч�����л�ȡһ����ָ��ģ�����Ч
			һ����˵������Ϸ����ʱ���Ҫɾ��һ����Ч������˵һ���ҽ�
			��Ч�Ķ�������ˣ���������ֱ��ɾ����Ч�ģ����ǰ�������һЩ�����
			�����³�ʼ�������뵽һ�����е������У��´����Ҫ�󴴽�ͬ�����͵���Ч��
			���ȴӿ����������ң����û�У������´���һ��ȫ�µ�
		*/
		Effect * getFromFreeMap( const Ogre::String &templateName );

		/// ��һ����Ч���뵽����������
		void addToFreeEffectMap( Effect *effect );	

		void addToLoadedScripts( const Ogre::String &fileName )	{	mLoadedScripts.push_back(fileName);	}
		void addToLoadedSkillScripts( const Ogre::String &fileName )	{	mLoadedSkillScripts.push_back(fileName);	}

        /// ���������Ϣ
        void _logErrorInfo(const Ogre::String& errorInfo, const Ogre::String& lineContent, 
            const Ogre::String& functionName);

		//���������ļ�
		/** Internal script parsing method. */
		void parseNewParticleEmitter(const Ogre::String& type, Ogre::DataStreamPtr& chunk, Ogre::ParticleSystem* sys);
		/** Internal script parsing method. */
		void parseNewParticleAffector(const Ogre::String& type, Ogre::DataStreamPtr& chunk, Ogre::ParticleSystem* sys);
		/** Internal script parsing method. */
		void parseParticleAttrib(const Ogre::String& line, Ogre::ParticleSystem* sys);
		/** Internal script parsing method. */
		void parseParticleEmitterAttrib(const Ogre::String& line, Ogre::ParticleEmitter* sys);
		/** Internal script parsing method. */
		void parseParticleAffectorAttrib(const Ogre::String& line, Ogre::ParticleAffector* sys);
		
	private:

		ElementFactoryMap mElementFactoryMap;
		EffectTemplateMap mEffectTemplateMap;
		FreeEffectMap mFreeEffectMap;
		SkillMap mSkillMap;

		EffectTemplateScriptFileMap mEffectTemplateScriptFileMap;
        SkillTemplateScriptFileMap mSkillTemplateScriptFileMap;
		ParticleTemplateScriptFileMap mParticleTemplateScriptFileMap;

		Ogre::StringVector mLoadedScripts;
		Ogre::StringVector mLoadedSkillScripts;

		ActiveEffectList mActiveEffectList;

		Ogre::StringVector mScriptPatterns;

		System *mSystem;

		Ogre::Controller<Ogre::Real> *mController;

		unsigned short mMaxNumFreeEffectPerList;

        /// ÿ�ν���getLine��ʱ��͵����кţ���Ϊ����������Ϣ
        int mWrongLineNum;

        /// ��ǰ�������ļ�����
       Ogre::String mParsingFileName;

        /// ��ǰ�����ϵĵƹ�仯��Ϣ
        MainSceneLight* mMainSceneLight;

        /// ��Ч����
        EffectLevel mEffectLevel;

		
	public:
		DestroySkillCallback* getDestroySkillCallback() const;
		void setDestroySkillCallback(DestroySkillCallback* val);
		CameraShakeCallback* getEffectCameraShakeCallback() const { return m_CameraShakeCallback;}
		void setEffectCameraShakeCallback(CameraShakeCallback* val) {m_CameraShakeCallback = val;}
	protected:
		DestroySkillCallback*	m_destroySkillCallback;
		CameraShakeCallback*	m_CameraShakeCallback;

		
		Ogre::String mGroupName;
	};
}

#endif