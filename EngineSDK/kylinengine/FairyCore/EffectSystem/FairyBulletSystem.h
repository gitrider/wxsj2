/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:42
	file base:	FairyBulletSystem
	file ext:	h
	author:		姜威
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletSystem_h__
#define FairyBulletSystem_h__

#include "OgreStringInterface.h"
#include "OgreStringInterface.h"
#include "OgreSceneNode.h"

// fairy header
#include "Core/FairyPrerequisites.h"
#include "Core/TerrainData.h"
#include "FairyBullet.h"
#include "FairyEffectBasic.h"
#include "FairyBulletOperator.h"
#include "ModelSystem/FairyLogicModel.h"
// 声音函数指针
typedef int (__stdcall* OnPlaySound)(const char* szSoundFile, float* fvPos, bool isLoop);
typedef void (__stdcall* OnStopSound)(int handle);

namespace Fairy
{
	class AnimationEffectInfo;
	class AnimationRibbon;
	class AnimationSound;
	class AnimationSceneLightInfo;
	class AnimationCameraShake;
	class AnimationLine;
	class System;
	class TerrainData;

	
	class BulletSystem : public Ogre::StringInterface
	{
	public:
		enum PursuitType
		{
			PT_POINTING,
			PT_BEELINE,
			PT_BIZZARD
		};

		// 子弹轨迹类型
		enum ENUM_BULLET_CONTRAIL_TYPE
		{
			BULLET_CONTRAIL_TYPE_INVALID = -1,
			BULLET_CONTRAIL_TYPE_BEELINE,			// 直线
			BULLET_CONTRAIL_TYPE_PARABOLA,			// 抛物线
			BULLET_CONTRAIL_TYPE_NONE,				// 无轨迹，直接爆炸
			BULLET_CONTRAIL_TYPE_ROTATE,			// 旋转
			BULLET_CONTRAIL_TYPE_EXP,

			BULLET_CONTRAIL_TYPE_NUMBERS
		};


		BulletSystem(const Ogre::String& templateName,System* system);
		virtual ~BulletSystem();

		

		class CmdForce : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};


		unsigned int getClusterIndex() const;
		void setClusterIndex(unsigned int val);

		const Ogre::Vector3& getPosition() const;
		void setPosition(const Ogre::Vector3& val);

		const Ogre::Quaternion& getOrientation() const;
		void setOrientation(const Ogre::Quaternion& val);

		const Ogre::Vector3& getVelocityWorld() const;
		void setVelocityWorld(const Ogre::Vector3& val);

		const Ogre::Vector3& getAccelerationWorld() const;
		void setAccelerationWorld(const Ogre::Vector3& val);

		

		BulletSystem& operator = (const BulletSystem& rhs);

		void initParameters();

		void createSceneNode();

		void createSceneNode(Ogre::SceneNode* parentNode);

		void removeSceneNode();

		void setTransformInfo(const TransformInfo& info);
		const TransformInfo& getTransformInfo();

		void update(Real time);

		const Fairy::String&	getTemplateName() const { return m_templateName; }

		//////////////////////////////////////////////////////////////////////////		
		AnimationEffectInfo* addAnimationEffectInfo(void);

		AnimationEffectInfo* getAnimationEffectInfo(unsigned short index) const;

		unsigned short getNumAnimationEffectInfos(void) const;

		void removeAnimationEffectInfo(unsigned short index);

		void removeAllAnimationEffectInfos(void);

		//////////////////////////////////////////////////////////////////////////
		AnimationRibbon* addAnimationRibbon(void);

		AnimationRibbon* getAnimationRibbon(unsigned short index) const;

		unsigned short getNumAnimationRibbons(void) const;

		void removeAnimationRibbon(unsigned short index);

		void removeAllAnimationRibbons(void);

		//////////////////////////////////////////////////////////////////////////		
		AnimationCameraShake* addAnimationCameraShake(void);

		AnimationCameraShake* getAnimationCameraShake(unsigned short index) const;

		unsigned short getNumAnimationCameraShake(void) const;

		void removeAnimationCameraShake(unsigned short index);

		void removeAllAnimationCameraShakes(void);

		//////////////////////////////////////////////////////////////////////////
		AnimationSound* addAnimationSound(void);

		AnimationSound* getAnimationSound(unsigned short index) const;

		unsigned short getNumAnimationSounds(void) const;

		void removeAnimationSound(unsigned short index);

		void removeAllAnimationSounds(void);

		void Initial(LogicModel* targetModel,LogicModel* casterModel,const Ogre::String attachPoint,
			const Ogre::String targetPoint,const Ogre::Vector3	ffsetPos,const Ogre::Quaternion	offsetRotation);

		void shutdown(void);

		//////////////////////////////////////////////////////////////////////////		
		AnimationSceneLightInfo* addAnimationSceneLightInfo(void);

		AnimationSceneLightInfo* getAnimationSceneLightInfo(unsigned short index) const;

		unsigned short getNumAnimationSceneLightInfos(void) const;

		void removeAnimationSceneLightInfo(unsigned short index);

		void removeAllAnimationSceneLightInfos(void);

		//////////////////////////////////////////////////////////////////////////		
		AnimationLine* addAnimationLine(void);

		AnimationLine* getAnimationLine(unsigned short index) const;

		unsigned short getNumAnimationLines(void) const;

		void removeAnimationLine(unsigned short index);

		void removeAllAnimationLines(void);


		void addBulletOperator(BulletOperator* pOperator);

		std::vector<BulletOperator*>& getBulletOperators();

		bool findBulletOperator(BulletOperator* pOperator);


		static void SetPlaySoundFuncton(OnPlaySound func)
		{
			mOnPlaySound = func;
		}
		static OnPlaySound GetPlaySoundFuncton(void)
		{
			return mOnPlaySound;
		}

		static void SetStopSoundFunction(OnStopSound func)
		{
			mOnStopSound = func;
		}
		static OnStopSound GetStopSoundFunction(void)
		{
			return mOnStopSound;
		}
		const Real getHeightToTerrain();


		const Fairy::Real& getAge() { return m_fCumulateTime; }
		void setAge(Fairy::Real& val) { m_fCumulateTime = val;}

		const Fairy::Real& getDelta() { return m_fDelta; }
		void setDelta(Fairy::Real& val) { m_fDelta = val;}

		const bool& getAlreadyHit() const { return m_bAlreadyHit; }
		void setAlreadyHit(const bool& val) { m_bAlreadyHit = val;} 

		const bool& getIsCreated() const { return m_isCreated; }
		void setIsCreated(const bool& val) { m_isCreated = val;} 

		const Fairy::Real& getCumulateDistance() { return m_fCumulateDistance; }
		void setCumulateDistance(Fairy::Real& val) { m_fCumulateDistance = val;} 



		const Ogre::Vector3& getTargetPosition() const { return m_vTargetPoint;}
		void setTargetPosition(const Ogre::Vector3& val) { m_vTargetPoint = val; }


		const Ogre::Vector3& getCurrentPosition() const { return m_vCurrentPoint;}
		void setCurrentPosition(const Ogre::Vector3& val) { m_vCurrentPoint = val; }

	
		// 设置 人物受击点名称
		void SetHitEffectLocator( const String& locatorName )	{ m_strHitEffectLocator = locatorName; }

		Real getArriveDistance() const;
		
		void copyParameters(BulletSystem* dest) const;

		typedef std::vector<AnimationEffectInfo *> AnimationEffectInfos;
		typedef std::vector<AnimationRibbon *> AnimationRibbons;
		typedef std::vector<AnimationSound *> AnimationSounds;
		typedef std::vector<AnimationSceneLightInfo *> AnimationSceneLightInfos;
		typedef std::vector<AnimationCameraShake*> AnimationCameraShakes;
		typedef std::vector<AnimationLine*> AnimationLines;
		typedef std::vector<BulletOperator*> BulletOperators;

	protected:
		void _createAnimationEffect(Ogre::Real oldTime,Ogre::Real currTime);
		void _updateAttachedAnimationEffect(Ogre::Real oldTime, Ogre::Real currTime);
		void _createAnimationCameraShake(Ogre::Real oldTime, Ogre::Real currTime);
		void _updateAnimationCameraShake(Ogre::Real delta);
		void _createAnimationRibbon(Ogre::Real oldTime, Ogre::Real currTime);
		void _updateAnimationRibbon(Ogre::Real oldTime, Ogre::Real currTime);
		void _updateAnimationSound(Ogre::Real oldTime, Ogre::Real currTime);
		void _updateAnimationLight(Ogre::Real oldTime, Ogre::Real currTime);
		void _updateAnimationLine(Ogre::Real oldTime,Ogre::Real currTime);

		float m_fOldTime;


	protected:
		AnimationEffectInfos mAnimationEffectInfos;
		AnimationRibbons mAnimationRibbons;
		AnimationSounds mAnimationSounds;
		AnimationSceneLightInfos mAnimationSceneLightInfos;
		AnimationCameraShakes mAnimationCameraShakes;
		AnimationLines mAnimationLines;
		BulletOperators mBulletOperators;

		System*				m_system;
		TransformInfo		m_transInfo;
		bool				m_isCreated;
		Ogre::String		m_templateName;

		Ogre::Vector3		m_vCurrentPoint;
		Ogre::Vector3		m_vTargetPoint;
		Ogre::Vector3		m_vCasterPoint;

		LogicModel*			m_TargetModel;
		LogicModel*			m_CasterModel;

		String								m_AttachPointName;
		String								m_TargetPointName;
		Ogre::Vector3						m_OffsetPos;
		Ogre::Quaternion					m_OffsetRoation;

		Real				m_fCumulateDistance;
		Real				m_fCumulateTime;		// 子弹飞行累计时间
		Real				m_fDelta;
		Ogre::Vector3		m_vRotation;
		String				m_strHitEffectLocator;	// 受击绑点名称
		bool				m_bAlreadyHit;
		bool				m_bActionAgain;

		// 声音的函数指针
		static OnPlaySound                   mOnPlaySound;
		static OnStopSound                   mOnStopSound;

	

		Ogre::Vector3		m_velocityWorld;
		Ogre::Vector3		m_accelerationWorld;
		Ogre::SceneNode*	m_sceneNode;
		unsigned int		m_clusterIndex;
	
	public:
		void*				m_userData;// 用户数据，每个BulletSystem挂接一个，用来挂接客户端对象 [4/14/2008 JiangWei]
	};
}

#endif // FairyBulletSystem_h__