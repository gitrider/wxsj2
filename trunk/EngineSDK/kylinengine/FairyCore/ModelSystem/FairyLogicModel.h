/********************************************************************
filename:   WXLogicModel.h

purpose:    LogicModel���������滻ԭ��DObject�ģ�����Ҫ��������Ⱦ����
			�ͻ��˵�һ���Ӻϲ㣬������Ⱦ�㣬ʵ�ֿͻ��˶Գ�����
			��̬�����һЩ�����绻װ������Ч�ȣ��������ǿͻ��˿��Ժ�
			�����ʵ�ָ���Ч���������������Ⱦ��Ĵ��롣
*********************************************************************/

#ifndef _FAIRYLOGICMODEL_H_
#define _FAIRYLOGICMODEL_H_

#include "Core/FairyPrerequisites.h"
#include "Core/FairyVariant.h"
#include "Core/TerrainData.h"

#include "FairyLogicModelManager.h"

#include "EffectSystem/FairyEffect.h"

#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreAny.h>
#include <OgreStringVector.h>
#include <OgreMaterial.h>
#include <OgreRay.h>
#include <OgreIteratorWrappers.h>

#include "core/skillhelper.h"

#define NO_ANIM_BLEND	0

// �������й����еĻص�����
typedef bool          (*OnAnimationFinish)(const char* pAnimationName, unsigned long info, int nFlag  );

typedef bool          (*OnSkillHitTime)(const char* pAnimationName, unsigned long info,
                                        unsigned short index, unsigned short totalCount);

typedef bool          (*OnSkillBreakTime)(const char* pAnimationName, unsigned long info,
                                          unsigned short index, unsigned short totalCount);

typedef bool          (*OnSkillShakeTime)(const char* pAnimationName, unsigned long info);
typedef bool          (*OnGetEffectName)(const char* pAnimationName, Ogre::String& effectName);


// ��ȡ��Ϸ������ĳһx��z����ĸ߶�ֵ
typedef bool         (__stdcall*GetHeightInWorld)(float x, float z, float& y);

// ��������ָ��
typedef int (__stdcall* OnPlaySound)(const char* szSoundFile, float* fvPos, bool isLoop);
typedef void (__stdcall* OnStopSound)(int handle);

namespace Ogre	{

	class Entity;
	class Any;
	class UserDefinedObject;
	class SkeletonInstance;
	class Node;
	class AnimationState;
	class Animation;
    class AxisAlignedBox;

    /// ���������������
    class Fabric;
    class StdCollider;
    class ColliderSet;
}

namespace Fairy	{

    const Ogre::ColourValue& _getColourFromMaterial(const Ogre::MaterialPtr& mat);

    enum ModelProjectorType
    {
        MPT_SELECTED,   // ��ѡ��ʱ��ʾ�ĵ�projector
        MPT_SHADOW,     // ���µļ���Ӱ
    };

	class System;
	class Effect;
	class Skill;
	class BulletSystem;
   // class LogicModelManager;
	class BulletFlowSystem;

	class LogicModel   //: public ISkillCaster
	{
        // ����LogicModelManagerΪ��Ԫ�ֱ࣬�Ӵ洢˽�г�Ա
        friend class LogicModelManager;
		friend class WXObjModel;
		friend class CGod;
		friend class CAnthropoMonster;
		friend class CHero;
		friend class CDataManipulator;
		friend class CWeapon;
		friend class CRider;
		friend class CModel;
	public:

		/// ��������ʱ����������
		static Ogre::String MESH_HINT;
		static Ogre::String MATERIAL_HINT;
		static Ogre::String SLMT_CREATE_HINT;

		/// ������������
		static Ogre::String MOVEMENT_TYPE_ATTRIBUTE;
        static Ogre::String DEFAULT_ANIM_NAME;
        static Ogre::String BOUNDING_BOX;
        static Ogre::String SCALE_FACTOR;

        static Ogre::String HAIR_MESH_NAME;

		/// addEffectʱ���ص���Ч���
		typedef Ogre::uint EffectHandle;

		/// ��ȡlocator��Ϣʱ�ı�־
		enum GetTransformInfoType
		{
			GTIT_POSITION,	// ֻ��ȡλ����Ϣ
			GTIT_ORIENTATION,	// ֻ��ȡ��ת��Ϣ
			GTIT_ALL,	// ��ȡλ����Ϣ����ת��Ϣ
		};

		// object���ͣ���Ҫ������object�����߷�ʽ
		enum MovementType
		{
			MT_NORMAL, // ��ͨ���ͣ��������������object�ķ�λ
			MT_ROTATE_X_AXIS,	// ֻ��ǰ��ҡ��
			MT_ROTATE_X_Z_AXIS,	// ����ǰ������ҡ��
		};

		/// �Ѵ�����effect����Ϣ������updateAllEffects
		struct CreatedEffectInfo
		{
			CreatedEffectInfo() :
			mEffect(NULL),
			mLocator(""),
            mTransformType(GTIT_POSITION),
            mEffectHandle(0)
			{
			}

			CreatedEffectInfo(Effect* effect, const Ogre::String& locatorName, 
                GetTransformInfoType transformType, EffectHandle handle) :
			mEffect(effect),
			mLocator(locatorName),
            mTransformType(transformType),
            mEffectHandle(handle)
			{
			}

			Effect* mEffect;
			Ogre::String mLocator;
            GetTransformInfoType mTransformType;
            EffectHandle mEffectHandle;
		};

        typedef std::list<CreatedEffectInfo> CreatedEffectList;

        /// slot����
        struct SlotValue
        {
            SlotValue() : mModelName(""), mModel(NULL)
            {
            }

            SlotValue( const Ogre::String& modelName ) :
            mModelName(modelName),
                mModel(NULL)
            {
            }

            ~SlotValue(void)
            {
                if (mModel)
                    LogicModelManager::getSingleton().destroyLogicModel(mModel);
            }

           Ogre::String mModelName;
            LogicModel* mModel;
        };

        typedef std::map<Ogre::String, SlotValue> SlotMap;

        /// locator��Ϣ
        struct	LocatorValue
        {
            LocatorValue() : 
            mBoneName(""),
                mOffsetPos(Ogre::Vector3::ZERO),
                mOffsetOrientation(Ogre::Quaternion::IDENTITY),
                mLocatorNode(NULL),
                mTransferable(false),
                mTranslateFirst(false)
            {
                mSlotMap.clear();
            }

            LocatorValue(const Ogre::String& boneName, const Ogre::Vector3& pos, 
                const Ogre::Quaternion& orientation, bool transferable = false,
                bool translateFirst = false) :
            mBoneName(boneName),
                mOffsetPos(pos),
                mOffsetOrientation(orientation),
                mLocatorNode(NULL),
                mTransferable(transferable),
                mTranslateFirst(translateFirst)
            {
                mSlotMap.clear();
            }

            LocatorValue& operator = (const LocatorValue& rhs)
            {
                this->mBoneName = rhs.mBoneName;
                this->mOffsetPos = rhs.mOffsetPos;
                this->mOffsetOrientation= rhs.mOffsetOrientation;
                this->mLocatorNode = rhs.mLocatorNode;
                this->mTransferable = rhs.mTransferable;
                this->mTranslateFirst = rhs.mTranslateFirst;

                return *this;
            }

           Ogre::String	mBoneName;
            Ogre::Vector3		mOffsetPos;
            Ogre::Quaternion	mOffsetOrientation;
            Ogre::SceneNode*	mLocatorNode;
            SlotMap			mSlotMap;
            bool mTransferable;
            bool mTranslateFirst;   // ��update locator posʱ�Ƿ��Ƚ���translate
        };

        typedef std::map<Ogre::String, LocatorValue> LocatorMap;

        /// ������ײ������
        enum FabricColliderType
        {
            FCT_BOX,
            FCT_SPHERE,
            FCT_PLANE,
        };

        /// ������ײ���������Ϣ
        struct FabricCollider 
        {
            FabricCollider(const Ogre::String& locator, FabricColliderType type, const Ogre::String& colliderInfo) :
            mLocatorName(locator), mColliderType(type), mColliderInfo(colliderInfo),
            mLocatorNode(NULL), mCollider(NULL)
            {
            }

           Ogre::String mLocatorName;    // ��ײ������Ĺҽӵ������
            FabricColliderType mColliderType;   // ��ײ������
           Ogre::String mColliderInfo;   // ��ײ��Ķ�����Ϣ�����ݲ�ͬ��ײ���������box���͵���Ҫ���������������峤��ߡ�
            Ogre::SceneNode* mLocatorNode;  // ����locator��ָ��
            Ogre::StdCollider* mCollider;   // ������������ײ���ָ��
        };

        /// ���沼����ײ����Ϣ��map
        typedef std::map<Ogre::String, FabricCollider> FabricColliderMap;

        /// ��ɫ������ֵ
        struct AttribValue
        {
            AttribValue() : mAttribValue(""), mHint(0)
            {
            }

            AttribValue(const Ogre::String& value, const Ogre::String& hint) :
            mAttribValue(value),
                mHint(hint)
            {
            }

           Ogre::String mAttribValue;
           Ogre::String mHint;
        };
        /// ����map��key����������value������ֵ��Ϣ
        typedef std::map<Ogre::String, AttribValue> AttribMap;

	protected:

        /// ͸�������õĲ���ʵ��
        struct ModelMaterialInstance
        {
            ModelMaterialInstance(const Ogre::MaterialPtr& origin);
            ~ModelMaterialInstance();

            Ogre::MaterialPtr mOrigin;
            Ogre::MaterialPtr mDerived;
        };

        /// ͸����
        class EntityTransparentor
        {
        protected:
            Ogre::Entity* mEntity;

            typedef std::vector<ModelMaterialInstance*> MaterialInstanceList;
            typedef std::vector<size_t> IndexList;
            size_t addMaterial(const Ogre::MaterialPtr& material);

            bool applyTransparency(const Ogre::MaterialPtr& origin, const Ogre::MaterialPtr& derived, Ogre::Real transparency);

        public:
            EntityTransparentor(void);

            EntityTransparentor(Ogre::Entity* entity);

            ~EntityTransparentor();

            void clear(void);

            void init(Ogre::Entity* entity);

            void apply(Ogre::Real transparency);

        public:
            MaterialInstanceList mMaterialInstances;
            IndexList mSubEntityMaterialIndices;
        };

		/** typedef	*/
		
		/// ��ͨ��ɫ������entity���б�
		struct EntityValue
		{
			EntityValue() : mMeshName(""), mMaterialName(""), mEntity(NULL), mTransparentor(NULL),
                mEntityColour(Ogre::ColourValue::Black), mEntityOriginColour(Ogre::ColourValue::Black),
                mEntityColourChanged(false)
			{
			}

			EntityValue(const Ogre::String& meshName, const Ogre::String& matName) :
			mMeshName(meshName),
				mMaterialName(matName),
				mEntity(NULL),
                mTransparentor(NULL),
                mEntityColour(Ogre::ColourValue::Black),
                mEntityOriginColour(Ogre::ColourValue::Black),
                mEntityColourChanged(false)
			{
			}

			Ogre::String mMeshName;
			Ogre::String mMaterialName;
			Ogre::Entity* mEntity;
            EntityTransparentor* mTransparentor;
            /// ����clone���Ĳ���
            std::vector< Ogre::MaterialPtr > mClonedMaterials;
            /// ����clone֮ǰ�õĲ���
            std::vector<Ogre::String > mOriginMaterialNames;

            /// ���entity�����õ���ɫֵ
            Ogre::ColourValue mEntityColour;
            /// entity������ɫ֮ǰ����ɫֵ
            Ogre::ColourValue mEntityOriginColour;
            bool mEntityColourChanged;
		};

		typedef std::map<Ogre::String, EntityValue> EntityMap;

		/// material����
		struct MaterialValue
		{
			MaterialValue() : mEntityEntry(""), mMaterialName("")
			{
			}

			MaterialValue(const Ogre::String& entityEntry, const Ogre::String& matName) :
			mEntityEntry(entityEntry),
			mMaterialName(matName)
			{
			}

			Ogre::String mEntityEntry;
			Ogre::String mMaterialName;
		};

		typedef std::map<Ogre::String, MaterialValue> MaterialMap;

		/// ��¼obj�ļ���Effects�����Ϣ
		struct EffectValue
		{
            EffectValue() :	mEffectName(""), mLocator(""), mTranslateAll(false), mEffectColour(Ogre::ColourValue::White)
			{
			}

            EffectValue(const Ogre::String& effectName, const Ogre::String& locatorName, bool translateAll, const Ogre::ColourValue& colour) :
			mEffectName(effectName),
				mLocator(locatorName),
                mTranslateAll(translateAll),
                mEffectColour(colour)
			{
			}

			Ogre::String mEffectName;
			Ogre::String mLocator;
            bool mTranslateAll;
            Ogre::ColourValue mEffectColour;
		};
		typedef std::map<Ogre::String, EffectValue> EffectMap;			

		/// ���ҽ��ϵ�����ԭ����node
		typedef std::map<LogicModel*, Ogre::Node*> AttachedModelOriginSceneNodes;

        /// ����model�ϵ�projector
        struct ModelProjector
        {
            ModelProjector() : mProjectorName(""), mType(MPT_SELECTED), mProjectorEffect(NULL), mProjectorNode(NULL)
            {
            }

            ModelProjector(const Ogre::String& projectorName, ModelProjectorType type) :
            mProjectorName(projectorName),
            mType(type),
            mProjectorEffect(NULL),
            mProjectorNode(NULL)
            {
            }

           Ogre::String mProjectorName;  // ������Ч����
            ModelProjectorType mType;   // ͶӰ����
            Effect* mProjectorEffect;   // ��Чָ��
            Ogre::SceneNode* mProjectorNode;
        };
        typedef std::list<ModelProjector*> ModelProjectors;        

        /// ��ҪͶ��Ӱ��mesh����
        typedef std::list<Ogre::String> ShadowUncastableMeshs;
        
		/// ������붯�����б�
        typedef std::list<Ogre::AnimationState*> PlayingAnimationList;
        PlayingAnimationList mPlayingAnimationList;

        /// ����ר��͸����
        class FabricTransparentor
        {
        protected:
            Ogre::Fabric* mFabric;
            ModelMaterialInstance* mMaterialInstance;

            bool applyTransparency(const Ogre::MaterialPtr& origin, const Ogre::MaterialPtr& derived, Ogre::Real transparency);

        public:
            FabricTransparentor(Ogre::Fabric* fabric);
            ~FabricTransparentor();

            void clear(void);
            void init(void);
            void apply(Ogre::Real transparency);
        };

        struct FabricInfo
        {
            FabricInfo(Ogre::Fabric* fabric)
                : mFabric(fabric)
                , mTransparentor(NULL)
            {
            }

            Ogre::Fabric* mFabric;
            FabricTransparentor* mTransparentor;
        };

        typedef std::map<Ogre::String, FabricInfo> FabricInfoMap;

		struct AreaEffect
		{
			AreaEffect() :	mEffect(NULL), mTemplateName(""), mBaseNode(NULL)
			{
			}

			AreaEffect(Fairy::Effect* effect, const Ogre::String& templateName, Ogre::SceneNode* baseNode) :
			mEffect(effect),
			mTemplateName(templateName),
			mBaseNode(baseNode)
			{
			}

			Fairy::Effect* mEffect;
			Ogre::String mTemplateName;
			Ogre::SceneNode* mBaseNode;
		};

		typedef std::vector<AreaEffect> AreaEffectMap;

	public:
		LogicModel( const Ogre::String& name, System *system );
		~LogicModel();

		/// Model����л��գ��ڻ���ǰҪ��reset
		void reset(void);

		/// obj�ļ������Ե�����
		bool addAttribute( const Ogre::String &name, const Ogre::String &value, const Ogre::String &hint );
		bool changeAttribute( const Ogre::String &name, const Ogre::String &value );
		// �ı�Attribute��Hints���� add by zhuwenchao 2010.03.05
		bool changeAttributeHints(const Ogre::String &name,const Ogre::String &hints);
        Variant getAttribute( const Ogre::String& attributeName );

        const AttribMap& getAttribMap(void)
        {
            return mAttribMap;
        }
        /** ��ȡ��ǰ����ֵ
        @param value ��ǰ����ֵ
        @return 
            ���û��Ҫ�ҵ����ԣ��ͷ���false
        */
        bool getAttribute( const Ogre::String& name,Ogre::String& value);

        /** ����Ƿ���ָ��������
        */
        bool hasAttribute( const Ogre::String& name );

		bool addEntity( const Ogre::String &name, const Ogre::String &meshName, const Ogre::String &matName );

        /** �ı�entity
        @param name ��������
        @param value mesh���ƣ����Ϊ""����ʾɾ��ԭ����entity���������µ�entity
        */
		bool changeEntity( const Ogre::String &name, const Ogre::String &value );

		/**add by zhuwenchao 2010.1.28
		//��ȡָ��ʵ����(���༭��AVATAR��װ����ʹ��)
		//@param hints ��Ҫ��ȡ�Ĳ��ֵ�ʵ������
		 */
		Ogre::String GetEntityName(const Ogre::String &hints);

		bool addMaterial( const Ogre::String &name, const Ogre::String &entityEntry, const Ogre::String &matName );
		bool changeMaterial( const Ogre::String &name, const Ogre::String &value );

		bool addLocator( const Ogre::String &name, const Ogre::String &boneName, 
			const Ogre::Vector3 &pos, const Ogre::Quaternion &orientation, bool transferable = false, bool translateFirst = false);

		//add by zhuwenchao 2010.2.25
		//ɾ��һ���ҵ�
		bool DelLocator(const Ogre::String &name);

        /// �ж��������locator�Ƿ�Ϊtransferable
        bool isLocatorTransferable(const Ogre::String& name);

        bool isLocatorExist(const Ogre::String& locator);

        /// ��ȡlocator��Ϣ
        bool getLocatorInfo(const Ogre::String& locator, LocatorValue& locatorInfo);
        bool setLocatorInfo(const Ogre::String& locator, const LocatorValue& info);

		bool addSlot( const Ogre::String &locatorName, const Ogre::String &slotName, const Ogre::String &modelName, const Ogre::String &attrib );
		bool changeSlotAttrib( const Ogre::String &name, const Ogre::String &value );
		//LogicModel& operator = (const LogicModel& rhs);

        /** �����Ч��Ϣ����obj�ļ��л�ȡ����Ϣ��
        */
		bool addEffectInfo( const Ogre::String& name, const Ogre::String& effectName,
            const Ogre::String& locatorName, bool translateAll = false, const Ogre::ColourValue& colour = Ogre::ColourValue::White );

        /** ��Ӳ�����͸����Ӱ��mesh����
        */
        bool addShadowUncastable(const Ogre::String& meshName);

        /// �жϸ�mesh�Ƿ�涨Ϊ����ͶӰ
        bool isShadowUncastable(const Ogre::String& meshName);

		void setUserAny(const Ogre::Any& anything);
		const Ogre::Any& getUserAny(void) const;

		void setUserObject(Ogre::UserDefinedObject* obj);
		Ogre::UserDefinedObject* getUserObject(void) const;

		void setParentSceneNode( Ogre::Node* parentNode );		
		Ogre::SceneNode* getSceneNode(void)
		{
			return mModelMainNode;
		}

		void destroySceneNode(void);

		void destroyEntities(void);

		const Ogre::AxisAlignedBox&     getBoundingBox(void);

		void initModel(void);

		/// ÿ֡�н��б�Ҫ�ĸ��£��綯������Ч��skill�ĸ���
		void execute(Ogre::Real delta);

		/// ����ȫ�ֵĶ������ʣ�1.0��ʾ1���٣�2.0��ʾ2����
		void setGlobalAnimationRate(Ogre::Real rate)
		{
			mGlobalAnimationRate = rate;
		}
		Ogre::Real getGlobalAnimationRate(void)
		{
			return mGlobalAnimationRate;
		}

		/// �������model������entity��visible flag
		void setVisibleFlag(Ogre::uint32 flags);
        Ogre::uint32 getVisibleFlag(void)
        {
            return mVisibleFlag;
        }

		void setAnimationLoop(bool loop);
		bool getAnimationLoop(void)
		{
			return mAnimationLoop;
		}

        /** ��LogicModel�Ϲ�һ��effect
        @param effectName ��Ч����
        @param locatorName �ҽӵ�����
        @param transformType ��Ч���˶����ͣ�Ĭ��ΪGTIT_POSITION����ʾ
               ֻ����model��λ����Ϣ
        @param colour ������Чʱ��ָ������Ч��ɫֵ
        */
		EffectHandle addEffect( const Ogre::String& effectName, const Ogre::String& locatorName, 
            GetTransformInfoType transformType = GTIT_POSITION, const Ogre::ColourValue& colour = Ogre::ColourValue::White );

        /** ɾ��effect
        @param handle ��Чhandle
        @remarks �����������û�ҽ���ָ����effect�����쳣
        */
        CreatedEffectList::iterator delEffect( EffectHandle handle );

        /** ɾ��effect
        @param effectIterator ָ��Ҫɾ������Ч��iterator
        */
        CreatedEffectList::iterator delEffect( CreatedEffectList::iterator effectIterator );

        /// ɾ��������Ч
		void delAllEffect(void);

        /** ��ȡָ����Ч
        @param handle ������Ч��handle
        @remarks �����ǰ��������û�����effect���ͷ���NULL
        */
        Effect* getEffect(EffectHandle handle);

		bool getLocatorWorldTransform(const Ogre::String& name, Fairy::TransformInfo& info, const Ogre::Vector3& offsetPos = Ogre::Vector3::ZERO,
			const Ogre::Quaternion& offsetRotation = Ogre::Quaternion::IDENTITY, GetTransformInfoType type = GTIT_ALL );
		bool getBoneWorldTransform(const Ogre::String& name, Fairy::TransformInfo& info, const Ogre::Vector3& offsetPos = Ogre::Vector3::ZERO,
			const Ogre::Quaternion& offsetRotation = Ogre::Quaternion::IDENTITY, GetTransformInfoType type = GTIT_ALL );

        bool getLocatorWorldTransform(const Ogre::String& name, Ogre::Matrix4& mtx);

		void createSkill(const String& skillName,  bool loop = true, bool anim = false, Ogre::Real delayTime = 0.0f);
		void stopCurrentAction();
			
		void delCurrentSkill(void);

		Skill* getCurrentSkill(void)
		{	
			return mCurrentSkill;
		}

		void setName(const Ogre::String& name)
		{
			mName = name;
		}
		const Ogre::String& getName(void)
		{
			return mName;
		}		

		bool attachModel(const Ogre::String& locatorName, LogicModel* model);
		bool detachModel( LogicModel* model );
        /// ����attach�Լ���LogicModel
        void notifyAttach(LogicModel* parentModel)
        {
            mAttachParent = parentModel;
        }

        /// �жϵ�ǰ�Ƿ�ҽ��ڱ��model��
        bool isAttached(void)
        {
            return (mAttachParent != NULL);
        }

		/// ��ȡ��ǰ�ҽ�model�ĸ�model
        LogicModel* getAttachModel(void)
        {
            return mAttachParent;
        }

		bool isChildModel(void)
		{
			return mChildModel;
		}

		void setChildModel(bool child)
		{
			mChildModel = child;
		}

		void setPosition(Ogre::Vector3& pos);

		Ogre::Vector3& getPosition(void)
		{
			return mModelPosition;
		}

		void setOrientation(Ogre::Quaternion& orientation);

		Ogre::Quaternion& getOrientation(void)
		{
			return mModelOrientation;
		}

        bool isMainNodeNeedUpdate(void) const
        {
            return mMainNodeNeedUpdate;
        }

		// ������ݵ�������ת�ĽǶ�
		void calZAxisRotation(float deltaX, float deltaZ, float deltaHeight);
		void calXAxisRotation(float deltaX, float deltaZ, float deltaHeight);

        const Ogre::String& getCurrentAnimName(void) const
        {
            return mCurrentAnimationName;
        }

        /// ����λ��
        void resetPosition(void);

        /// ����model�Ƿ���ʾ�������ҽ���locator�ϵ������Լ�attachModel����ȥ������
        void setVisible(bool visible);
        
        /** ��model�����projector
        @remarks �ͻ��˵��ã����ڹҽ�projector���������µ���Ӱ��ѡ�л���
        @param type projector����
        @param projectorEffectName ��Ч����
        @param show �Ƿ��ڹ���ʱ����ʾ
        @param projectorSize ��ʼ��͸�䷶Χ
        */
        void addProjector(ModelProjectorType type, const Ogre::String& projectorEffectName,
            bool show = true, float projectorSize = 100.0f);

        /** ɾ������model�ϵ�projector
        @param type projector����
        @param projectorEffectName ��Ч����
        @return ���ɾ���ɹ�,��return true,��Ȼ,��return false
        */
        bool removeProjector(ModelProjectorType type, const Ogre::String& projectorEffectName);

		void addAreaEffect(const Ogre::String& templateName);
		bool removeAreaEffect(const Ogre::String& templateName);

        /** ��������model��͸����
        @remarks ����������Ըı�model��͸���ȣ����������Ϲҵ����壩
        @param time �ӵ�ǰ͸���ȹ��ɵ�transparent����������͸���ȵĹ���ʱ��
        @param transparency ��Ҫ��͸���ȣ�0��ʾ��͸����1��ʾȫ͸����Ҳ���ǲ��ɼ���
        */
        void setTransparent(Ogre::Real time, Ogre::Real transparency);

        /// ����model������entity��͸����
        void updateTransparency(Ogre::Real deltaTime);

        /** ����͸������Ϣ
        @remarks ���ø�model��͸������Ϣ�����transparentTimeΪ0�����������õ�entity�У����򣬾���Ҫ��execute�н��й���
        @param currTranparency ��ǰʱ�̵�͸����
        @param destTranparency Ҫ�ﵽ��͸����
        @param transparentTime ��ǰ��ʣ�µ�͸������ʱ��
        */
        void setTransparencyInfo(Ogre::Real currTransparency, Ogre::Real destTransparency, Ogre::Real transparentTime);

        void setHairColour(const Ogre::ColourValue& colour);

        Ogre::ColourValue& getHairColour(void)
        {
            return mHairColour;
        }

        /// �����Ƿ����ͶӰ
        void setShadowCastable(bool castable);

        /** ���������Ƿ�ѡ��
        @param selected ���ѡ�У�Ĭ�ϵ�����Ǹ�������Է������0.5
        */
        void setSelected(bool selected);

        /// ��ǰ�����Ƿ���ѡ��״̬
        bool isSelected(void)
        {
            return mSelected;
        }

        /** ����������ɫ��ͷ�����⣩
        @param colour Ҫ���õ���ɫֵ
        @param method ��ɫ���÷�ʽ
        */
        void setBodyColour( const Ogre::ColourValue& colour );

        /// ����������ɫ��ԭʼ����
        void resetBodyColour(void);

        /** �ѵ�ǰ��bounding box��ray�����ཻ�Լ��
        @param ray ���м�������
        @return �������ray�͵�ǰ��bounding box�ཻ���ͷ���true�����򣬷���false
        */
        bool rayIntersect(const Ogre::Ray& ray);

        /// ��ǰ��bounding box�Ƿ����ⲿ�����
        bool isUseExternalBoundingBox(void)
        {
            return NULL != mExternalBoundingBox;
        }

        /** ���һ������
        @param fabricName ���ϵ����ƣ������ڸ�model����Ψһ�ģ����֮ǰ�Ѿ�������
                          ͬ���Ĳ��ϣ����֮ǰ�Ĳ���ɾ��
        @param headLocatorName ��һ�����ҽӵ������
        @param tailLocatorName ���һ���ҽӵ������
        @remarks
        */
        void addFabric(
            const Ogre::String& fabricName,
            int width, int height,
            Ogre::Real mass, Ogre::Real naturalLength,
            Ogre::Real structuralStiffness,
            Ogre::Real shearStiffness,
            Ogre::Real flexionStiffness,
            Ogre::Real deformationRate,
            const Ogre::Vector3& windVelocity,
            const Ogre::String& materialName,
            const Ogre::String& headLocatorName,
            const Ogre::String& tailLocatorName);

        /// ɾ��ָ�����ƵĲ���
        void removeFabric(const Ogre::String& fabricName);

        /// ɾ����ǰ���ҽӵ����в���
        void removeAllFabrics(void);

        /** ���һ��������ײ��
        @param colliderName ��ײ�������ƣ�model��Ψһ����obj�ļ��ж���
        @param locatorName ��ײ�����ҽӵ�locator������
        @param colliderType ��ײ��������
        @param colliderInfo ������ײ������Ķ�����Ϣ����box�ĳ����
        */
        bool addFabricCollider( const Ogre::String& colliderName, const Ogre::String& locatorName,
                                FabricColliderType colliderType, const Ogre::String& colliderInfo = "" ); 

        /** �Ѳ�����ײ���������collider set��remove��
        @param colliderName ��ײ�������ƣ����Ϊ��TerrainCollider������remove������collider��
        ����ָ��������obj�ļ���FabricCollider��������� 
        */
        void removeFabricCollider( const Ogre::String& colliderName );

        void enableAnimationState(bool enable = true);

        void setEnableWalkingEffect(bool enable)
        {
            mEnableWalkingEffect = enable;
        }
        bool getEnableWalkingEffect(void)
        {
            return mEnableWalkingEffect;
        }

        const Ogre::AxisAlignedBox& _getFullBoundingBox(void)
        {
            if (!mFullBoundingBox)
                mFullBoundingBox = new Ogre::AxisAlignedBox;

            return *mFullBoundingBox;
        }

        /// ��ǰ������Ƿ��ܿ�����model
        bool isCameraVisible(void);
        
        /// ��ȡentitymap�е�mesh����
        Ogre::StringVector getOriginMeshNames(void);

        /** ���ϲ���һ������
        @param animName ��������
        @param startTime ������ʼ���ŵ�ʱ��
        @param weight �ö�����Ȩ��
        @remarks ����ö��������ڣ�����log��д�������Ϣ
        */
        void playAnimation(const Ogre::String& animName, Ogre::Real startTime = 0.0f, Ogre::Real weight = 1.0f);

        /// ��ȡָ��������length
        float getAnimationLength(const Ogre::String& animName) const;

        /// ���ø�model��active״̬,������Ǵ���active״̬,�Ͳ�ˢ��
        void setActive(bool active = true)
        {
            mIsActive = active;
        }
        bool isActive(void)
        {
            return mIsActive;
        }

        /** ���õ�ǰ�����ŵĶ�����ָ����ʱ�俪ʼ����
        @param range ������Χ��0��1��0��ʾ���Ŷ����ĵ�һ֡��1��ʾ���������һ֡
        */
        void setCurrentAnimTime(Ogre::Real range);

        /// ���õ�ǰģ�͵�����ϵ��
        void setScaleFactor(const Ogre::Vector3& scaleFactor);

        /// ���µ�ǰ���е�entity��normalize normal״̬
        void setEntityNormalizeNormal(bool normal);

		//////////////////////////////////////////////////////////////////////////
		void SetAnimationFinishListener(OnAnimationFinish func,Ogre::ulong info)
		{
			mOnAnimationFinish = func;
            mCallbackFuncInfo = info;
		}
		OnAnimationFinish GetAnimationFinishListener(void)
		{
			return mOnAnimationFinish;
		}

		void SetSkillHitTimeListener(OnSkillHitTime func,Ogre::ulong info)
		{
			mOnSkillHitTime = func;
            mCallbackFuncInfo = info;
		}
		OnSkillHitTime GetSkillHitTimeListener(void)
		{
			return mOnSkillHitTime;
		}

		void SetSkillBreakTimeListener(OnSkillBreakTime func,Ogre::ulong info)
		{
			mOnSkillBreakTime = func;
            mCallbackFuncInfo = info;
		}
		OnSkillBreakTime GetSkillBreakTimeListener(void)
		{
			return mOnSkillBreakTime;
		}

        void SetSkillShakeTimeListener(OnSkillShakeTime func,Ogre::ulong info)
        {
            mOnSkillShakeTime = func;
            mCallbackFuncInfo = info;
        }
        OnSkillShakeTime GetSkillShakeTimeListener(void)
        {
            return mOnSkillShakeTime;
        }

        void SetGetEffectNameListener(OnGetEffectName func,Ogre::ulong info)
        {
            mOnGetEffectName = func;
            mCallbackFuncInfo = info;
        }
        OnGetEffectName GetGetEffectNameListener(void)
        {
            return mOnGetEffectName;
        }

        //////////////////////////////////////////////////////////////////////////
		static void SetGetHeightInWorldListener(GetHeightInWorld func)
		{
			mGetHeightInWorld = func;
		}
		static GetHeightInWorld GetGetHeightInWorldListener(void)
		{
			return mGetHeightInWorld;
		}

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

		/////////////////// �༭���� ////////////////////////
		Ogre::ushort getNumBones(void);
		const Ogre::String& getBoneName( Ogre::ushort index );
		const Ogre::Bone* getBone( Ogre::ushort index );
		const Ogre::Bone* getBone( const Ogre::String& name );

		Ogre::Real getCurrentAnimationLength(void);

		Ogre::ushort getSkeletonAnimationCount(void);

		Ogre::Animation* getSkeletonAnimation( Ogre::ushort index );

		void getLocatorNames( Ogre::StringVector& names );

		bool getAnimEditing(void)
		{
			return mAnimEditing;
		}
		void setAnimEditing(bool edit = true)
		{	
			mAnimEditing = edit;
		}

		void setAnimTimePos(Ogre::Real sliderValue)
		{
			mAnimTimePos = sliderValue;
		}
		Ogre::Real getAnimTimePos(void)	
		{
			return mAnimTimePos;
		}

		Ogre::Real getCurrentTimePos(void);	

        typedef Ogre::MapIterator<LocatorMap> LocatorMapIterator;
        LocatorMapIterator getLocatorMapIterator(void)
        {
            return LocatorMapIterator(
                mLocatorMap.begin(), mLocatorMap.end());
        }

		typedef std::map<Ogre::String, EffectHandle> EffectHandleMap;
/*
		/// offer iterator to visit property direct
		typedef Ogre::MapIterator<EntityMap> EntityMapIterator;
		EntityMapIterator getEntityIterator()	{return EntityMapIterator(mEntityMap.begin(),mEntityMap.end());}

		typedef Ogre::MapIterator<AttribMap> AttribMapIterator;
		AttribMapIterator getAttribIterator()	{return AttribMapIterator(mAttribMap.begin(),mAttribMap.end());}

		typedef Ogre::MapIterator<MaterialMap>	MaterialMapIterator;
		MaterialMapIterator	getMaterialIterator()	{return MaterialMapIterator(mMaterialMap.begin(),mMaterialMap.end());}

		typedef Ogre::MapIterator<EffectMap>	EffectMapIterator;
		EffectMapIterator	getEffectIterator()	{return EffectMapIterator(mEffectMap.begin(),mEffectMap.end());}

		const CreatedEffectList&	GetCreatedEffectList() const	{return mCreatedEffectList;}

		Ogre::Entity*	GetSkeletonEntity()							{return mSkeletonEntity;}
*/
		int _getTerrainSoundType(void);
/*
	//	void	setAnimBoneWeight( const std::string &sAnimName, const std::string &sBoneName, float nNewWeight, bool bSetSub  );
	//	void	setBaseAnim( const String &sBaseAnim,   bool bLoop);
	//	void	setSubAnim( const String &sSubAnim,   bool bLoop);

		virtual void	__stopSound( int nHandle )				;
		virtual bool	__handleAnimationFinish( const String &sAnimName , int nPart)			;
		virtual bool	__handleSkillHitTime( const String &sAnimName , int nHitLie, int nHitCount )			;
		virtual bool	__handleSkillBreakTime( const String &sAnimName, int nBreakLie, int nBreakCount )		;
		virtual bool	__handleSkillShakeTime( const String &sAnimName )			;
		virtual bool	__updateSkill( Skill *pSkill, float oldTime, float currTime, float nTimePass)				;

////	
		CSkillHelper&	GetSkillHelper()	{return	mSkillHelper;}	///extend skill helper not implement with current
*/				
	protected:

		void _createEntity(void);

		void _setEntityMaterials(void);

		Ogre::Entity* _createEntityImpl( const Ogre::String& meshName );

		void _destroyEntityImpl(Ogre::Entity* entity);

		void _createMainSceneNode(void);

		void _createLocatorNode(void);

		bool _changeAnimation(const Ogre::String& animationName, Ogre::Real delay);

		void _updateLocatorPos(void);

		void _animationBlend(Ogre::Real delta);

		void _executeLocatorModel(Ogre::Real delta);

		void _muteCurrentSkillSound(void);

		bool _handleAnimationFinish(Ogre::Real oldTime, Ogre::Real currTime);

		void _handleSkillHitTime(Ogre::Real oldTime, Ogre::Real currTime);

		void _handleSkillBreakTime(Ogre::Real oldTime, Ogre::Real currTime);

        void _handleSkillShakeTime(Ogre::Real oldTime, Ogre::Real currTime);

		void _createAnimationEffect(Ogre::Real oldTime, Ogre::Real currTime);

		void _createAnimationCameraShake(Ogre::Real oldTime, Ogre::Real currTime);

		void _updateAnimationCameraShake(Ogre::Real delta);

		void _createAnimationBulletFlow(Ogre::Real oldTime, Ogre::Real currTime);

		void _updateAttachedAnimationEffect(Ogre::Real oldTime, Ogre::Real currTime);

		void _createAnimationRibbon(Ogre::Real oldTime, Ogre::Real currTime);

		void _updateAnimationRibbon(Ogre::Real delta);

		void _updateAnimationSound(Ogre::Real oldTime, Ogre::Real currTime);

		void _updateAnimationBulletFlow(Ogre::Real oldTime, Ogre::Real currTime);

        void _updateAnimationLight(Ogre::Real oldTime, Ogre::Real currTime);

		bool _setEntityMaterial(const Ogre::String& entityEntry, const Ogre::String& matName);

		void _createEffects(void);

		void _updateAllEffects(void);

        void _updateModelProjectors(void);

		void _updateBoundingBox(void);

		void _updateAreaEffects(void);


		// ������Ҫ��ת����
		void _rotate(void);

		/// �����ַ��������ò�ͬ���ƶ�����
		void _setMovementType( const Ogre::String& value );

        /// ɾ������model�ϵ�����projector effect
        void _delAllProjector(void);

		void _delAllAreaEffect(void);
        /// ʵ����modelʱ����slot�ϵ�model
        bool _createModelInSlot(SlotValue& slotValue, LocatorValue& locatorValue);

        /// ����Ĭ�϶�������
        void _setDefaultAnimationName(const Ogre::String& name)
        {
            mDefaultAnimationName = name;
        }

        /// �����������ԣ�û��hint�����ԣ�
        void _dealWithExtraAttributes(const Ogre::String& name, const Ogre::String& value);

        /** ��ȡָ�����Ƶ�entity
        @param name Ҫ��ȡ��entity�����ƣ���HairMesh�ȣ�
        */
        Ogre::Entity* _getEntityByName(const Ogre::String& name);

        /** ������entity
        @remarks ÿ��model����һ����entity����Ϊskeleton entity������entity����master
                 entity���������
                 master entityֻ����LogicModel����ʱ�Żᱻɾ�������Կ��Ա�������������ֵ�
                 entityʱ����������Ĺ�ϵ����ȷ�ġ�
        @param skeleton ���ڴ���skeleton entity��skeleton
        */
        void _createSkeletonEntity(const Ogre::SkeletonPtr& skeleton);

        /// ɾ��master entity
        void _delSkeletonEntity(void);

        /// ����һ��entity����ɫ
        void _setEntityColour( EntityValue& entityValue, const Ogre::ColourValue& entityColour );
        /// ���һ��entity��clone�Ĳ���
        void _clearEntityMaterials( EntityValue& entityValue );

        void _resetEntityMaterial( EntityValue& entityValue );

        void _updateAllEntitiesTransparency(void);

        /// �����ⲿ��������ݣ�������һ��bounding box
        void _createBoundingBoxFromExternal(void);

        /// �����ⲿ��������ݣ�������������
        void _setScaleFactor(const Ogre::String& value);

        /// ���ݵ�ǰλ�ã���ȡ����Ӧ��ģ����Ч������
        bool _getTemplateEffectName(Ogre::String& effectName, const Ogre::String& templateName, float& height);

        /** ���ݶ������ƣ���ȡAnimationState
        @par �����ǰskeleton entity��û������������Ͷ�ȡ����������ڵĹ����ļ���
             ��link����ǰ��skeleton entity
        */
		Ogre::AnimationState* _getAnimationState(const Ogre::String& animName);

        /// ���û�ȡ���θ߶ȵĻص�����
        void _getWorldHeight(float x, float z, float& y);

        /// ����ͨ��attachModel����attach�������model
        void _executeAttachedModel(Ogre::Real delta);

        /// ɾ����������collider������colliderset
        void _destroyColliders(void);

        /// ��execute�и���collider��λ�úͷ�λ
        void _updateFabricColliders(void);

        /// ʵ�������е�collider
        void _initFabricColliders(void);

        /** ����һ��locator node
        @param offsetPos ��ʼƫ����
        @param offsetOri ��ʼƫ����ת��
        */
        Ogre::SceneNode* _createLocatorSceneNode(const Ogre::Vector3& offsetPos, const Ogre::Quaternion& offsetOri);

        /** ��ȡ��locator����Ӧ��scene nodeָ��
        @remarks �����ǰ���locator��scene node��δ�����������ͻ�����������е���_createLocatorSceneNode���д���
        */
        Ogre::SceneNode* _getLocatorSceneNode(LocatorValue& locatorValue);

        /** ���²���Ķ����б�
        @remarks ����Ķ������������һ�𲥷ţ�ֻ����һ�Σ������һ��������������ˣ��Ͱ��������
        �Ӷ����б���ɾ��
        */
        void _updatePlayingAnimations(Ogre::Real deltaTime);

        /** ���������entity�Ĳ���ת����hardware skinning����
        @remarks ���entity�е�ԭ���ʲ���������Ҫ��ת����ʧ�ܣ���ʹ��ԭ���ʣ�
                ����technique������pass������texture unit
                ����û�й���������entity��Ҳ��ֱ�ӷ���false
        */
        bool _convertEntityToCharacterMaterial(Ogre::Entity* entity);

        Ogre::MaterialPtr _deriveHardwareSkinningMaterial(const Ogre::MaterialPtr& origin,
            const Ogre::String& prefix, const Ogre::String& postfix, bool useSpecular);

        /// �жϵ�ǰϵͳ�Ƿ�֧��Ӳ��Ƥ��
        bool _checkForHardwareSkinningMaterialSupport(void);

	protected:

		/// ���ݳ�Ա
		AttribMap mAttribMap;
		EntityMap mEntityMap;
		MaterialMap mMaterialMap;
		LocatorMap mLocatorMap;
		EffectMap mEffectMap;

		EffectHandleMap mEffectHandleMap; // hold the handle from obj

		LogicModelManager::AnimationSkeletonLinks* mlinks;

        ShadowUncastableMeshs mShadowUncastableMeshs;

        FabricInfoMap mFabricInfoMap;

        FabricColliderMap mFabricColliderMap;

        /// ����model�ϵ�����projector effect���п�����ѡ�б�־��������Ӱ��
        ModelProjectors mModelProjectors;

		
		AreaEffectMap mAreaEffects;

		System* mSystem;

		Ogre::Any mUserAny;

		Ogre::SceneNode* mModelMainNode;
        Ogre::Node* mParentNode;

		Ogre::Entity* mSkeletonEntity;

		Ogre::ulong mCreatedEntityCount;

		Ogre::SkeletonInstance* mSkeleton;

		//����BoundingBox
		Ogre::AxisAlignedBox* mFullBoundingBox;

		//�Ƿ����BoundingBox
		bool                    mIsPrepareBoundBox;


		

		Ogre::AnimationState* mCurrentAnimationState;
		Ogre::AnimationState* mFadeOutAnimationState;

		Ogre::Real mGlobalAnimationRate;

		Ogre::Real mFadeOutDelay;

		Ogre::String mCurrentAnimationName;

		bool mAnimationLoop;

        CreatedEffectList mCreatedEffectList;

		Ogre::uint mEffectHandleCount;

		Skill* mCurrentSkill;

		Ogre::Real mAnimTimePos;
		bool mAnimEditing;

		Ogre::String mName;

		bool mChildModel;

		Ogre::Vector3 mModelPosition;
		Ogre::Quaternion mModelOrientation;

		AttachedModelOriginSceneNodes mAttachedModelOriginSceneNodes;

		/// ǰ���ŵص�������Orientation
		Ogre::Quaternion mFrontBackOrientation;
		/// �����ŵص�������Orientation
		Ogre::Quaternion mLeftRightOrientation;

		bool mMainNodeNeedUpdate;

		/// ��dobject�����ͣ���ͬ�������ڸ���ʱ���в�ͬ����Ϊ
		MovementType mMovementType;

        /// ��ǰmodel��͸����
        Ogre::Real mCurrentTransparency;

        /// Ŀ��͸����
        Ogre::Real mDestTransparency;

        /// ͸����delta
        Ogre::Real mDeltaTransparency;

        /// ͸�����Ƿ�Ҫ��ı�
        bool mNeedUpdateTransparency;

        /// ͸���ȹ���ʱ��
        Ogre::Real mTransparencyTime;

        /// ��ʼ�������ƣ���ʼ���������modelһ���������Ϳ�ʼѭ�����ŵ�
       Ogre::String mDefaultAnimationName;

        /// ͷ������ɫ
        Ogre::ColourValue mHairColour;
        
        /// �Ƿ����ͶӰ
        bool mShadowCastable;

        /** attach��LogicModel��LogicModel
        @remarks ������attachModel��detachModelʱ��Ҫ��this��ɱ�attach��model��mAttachParent��
                 �������Լ��Ǳ��ĸ�LogicModel��attach��
        */
        LogicModel* mAttachParent;

        /// ��ǰ�Ƿ�ѡ��
        bool mSelected;

        Ogre::AxisAlignedBox* mExternalBoundingBox;
       Ogre::String mExternalBoundingBoxValue;

        /// �ⲿ�������������
        Ogre::Vector3 mExternalScaleFactor;

        /// ��ײ������
        Ogre::ColliderSet* mColliderSet;

        Ogre::Real mTerrainHeight;

        Ogre::uint32 mVisibleFlag;

        bool mEnableWalkingEffect;
        
		// ������ػص�����
		OnAnimationFinish					mOnAnimationFinish;
		OnSkillBreakTime               mOnSkillBreakTime;
		OnSkillHitTime                 mOnSkillHitTime;
        OnSkillShakeTime               mOnSkillShakeTime;
        OnGetEffectName                mOnGetEffectName;

       Ogre::ulong mCallbackFuncInfo;

        /// �Ƿ�Ϊactive��ֻ��active��model�Ż�ִ��execute
        bool mIsActive;
        /// �Ƿ���Ϊ�ɼ�
        bool mVisible;

		//CSkillHelper	mSkillHelper;

		/// ��ȡ��Ϸ����ĸ߶�ֵ����
		/** ��������Ϸ�����У�ĳһ����ĵ���߶�ֵ�����ǵ��εĸ߶�ֵ��Ҳ����
			��������ĸ߶�ֵ����������ĸ߶�ֵ�ڵײ��޷���ȡ�����Ծ�ͨ���ص�
			�������ɸ߲���ݲ�ͬ��������߶�ֵ
 		*/
		static GetHeightInWorld          mGetHeightInWorld;

		// �����ĺ���ָ��
		static OnPlaySound                   mOnPlaySound;
		static OnStopSound                   mOnStopSound;
	};
}




#endif
