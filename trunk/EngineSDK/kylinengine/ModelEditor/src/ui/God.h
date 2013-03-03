//����ģ�����ݵĻ���
#ifndef __GOD_H__
#define __GOD_H__
#include "SceneManipulator.h"
#include "Modelsystem\FairyLogicModel.h"
#include "Modelsystem\FairyLogicModelManager.h"
#include "EditDobject_NT.h"

#include <OgreLogManager.h>
#include <OgreStringConverter.h>

namespace Fairy {

	enum E_MODEL_TYPE
	{
		EMT_NONE			= -1,
		EMT_ANTHROPOMONSTER		,				//���ι�
		EMT_HERO				,				//����
		EMT_MODEL			,					//ģ��(����,��Ч��)
		EMT_RIDER				,				//����
		EMT_WEAPON				,				//����
		EMT_MONSTER				,				//���ι�
		EMT_NUM					
	};

	struct BoneLocatorList
	{
		BoneLocatorList()
		{
			m_locatorname = "";					//�ҵ�����
			m_Bonename = "";					//��ͷ����
		}
		BoneLocatorList(DString locatorname,DString bonename)
		{
			m_locatorname = locatorname;
			m_Bonename = bonename;
		}
		DString FindMatching(DString name)		//��ѯƥ����
		{
			if(name.compare(m_locatorname) == 0)
				return m_Bonename;
			else if(name.compare(m_Bonename) == 0)
				return m_locatorname;
			else
				return "";
		}
		DString m_locatorname;
		DString m_Bonename;
	};

	typedef	Ogre::String				DString;
	typedef	std::vector<DString>		DStringList;

	class CGod: public CEditDobject_NT
	{
	public:
		CGod(SceneManipulator* sceneManipulator);

		virtual ~CGod();
	public:
		virtual bool CreateModel(const DString& entfile,						//����ģ��
			const DString& group = 
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);						

		virtual bool AddModelLocator(const DString& name,						//����ģ�Ͱ��						  		
			const DString& bonename,
			const Ogre::Vector3& offpos, 
			const Ogre::Quaternion& offrot){return true;}

		virtual bool AddModelSlot(const DString& locatorname,					//����ģ�Ͱ�Ĳ��(���ڰ�ģ��)						   
			const DString& slotname,
			const DString& slotobj, 
			const DStringList& initattribs){return true;}

		virtual bool AddModelEffect(const DString& effectname,					//����ģ�Ͱ���Ч						  
			const DString& locatorname){return true;}

		virtual void UpdateModelLocatorPos(const DString& locatorname,			//����ģ�͹ҵ��λ�á�����
			const Ogre::Vector3& position,
			const Ogre::Quaternion& quaternion){}

		virtual void PlayAnimation(const DString& aniname);						//���Ŷ���

		virtual void ModelConversion(const DString& name,						//ģ��ת������
			const DString& modelname);

		void InitShareSkeletonIni();											//��ʼ����������ļ�

		void LoadGeneralLocatorListIni();										//��ʼ��ͨ�ð󶨵��ļ�

		//objcet
		virtual DString InitObject();											//��ʼ��objectѡ��

		//attribute name
		virtual DStringList InitAttributename();								//��ʼ��attribute name

		//attribute hints
		virtual DStringList InitAttributehints();								//��ʼ��attribute hints

		//entitylist name
		virtual DStringList InitEntityListname();								//��ʼ��entity name

		//entitylist file
		virtual DString	InitEntityListFile();									//��ʼ��entity file

		virtual void InsertEntity(const DString& name,							//����һ��entity
			const DString& file,const DString& material);

		//materiallist name
		virtual DStringList InitMaterialListname();								//��ʼ��material name

		//materiallist
		virtual DStringList InitMaterialTable();								//��ʼ��materialtable

		virtual void  InsertMaterial(const DString& name,						//����һ��material
			const DString& file,const DString& material);
		
		//locator name
		virtual DStringList InitLocatorName();									//��ʼ��locator name

		//locator attachmodel
		virtual DStringList InitAttachModel();									//��ʼ��locator attachmodel

		//locator slotname
		virtual DStringList InitSlotName();										//��ʼ��locator slotname

		//effect name
		virtual DStringList InitEffectName();									//��ʼ��effect name

		//bone name
		virtual DStringList InitBoneName();										//��ʼ��bone name

		//skeleton name
		virtual DStringList InitSkeletonName();									//��ʼ��skeleton name

		//skeleton table
		virtual DStringList InitSkeletonTable();								//��ʼ��skeleton table

		virtual void InsertLocator(const DString& name,							//����һ��locator
			const DString& bonename,
			const DString& slotname,
			const Ogre::Vector3& pos = Ogre::Vector3::ZERO,
			const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY,
			bool transferable = false,
			bool translateFirst = false);

		virtual bool AddLocator(const DString& name,							//���һ��locator
			const DString& bonename,
			const DString& slotname,
			const Ogre::Vector3& pos = Ogre::Vector3::ZERO,
			const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY,
			bool transferable = false,
			bool translateFirst = false);

		virtual bool DelLocator(const DString& name);							//ɾ��һ��locator

		virtual int ChangeSlot(const DString& slotname,							//�ı�ҽ���Ʒ
		   const DString& modelname);

		virtual bool ChangeEffect(const DString& effectname,					//�ı�ҽ���Ч
		   const DString& locatorname,const DString& bone);

		virtual bool SaveToObj(Ogre::String StrFileName);						//����obj�ļ�

		virtual void LoadObjTemplateRes(DString templatename);					//����objģ����Դ

		virtual std::vector<bool> GetGetUIShow();								//��ȡҪ��ʾ��UI

		bool Avatar(const Fairy::DString &name,const DString& modelname);		//

		void Assembled(const DString& entfile,bool FirstPart);					//��װģ��

		void UpdateObjTemplateData(DString templatename);						//����obj�ļ�ģ������

		void ClearButtonShow();

		void UpdateLocatorPosAndOriData(const DString& locatorname);
		
		void ChangeMat(const Ogre::String &name, const Ogre::String &value);

		bool ChangeHints(const Ogre::String &name, const Ogre::String &hitns);	//�ı�������ʾ

		void SetTempEntity(Fairy::LogicModel* tempmap);

		void SaveTempEntity();

		void GetTempntity();
	public:
		Ogre::SceneNode* m_rootNode;											//�������ڵ�

		Ogre::SceneNode* m_pObjNode;											//����ڵ�	

		Fairy::SceneManipulator* m_pSceneManipulator;							//����������

		DStringList m_ShareSkeletonIni;											//������������ļ��б�

		DString m_Identifier;													//Ψһ��ʶ��

		E_MODEL_TYPE m_ModelType;												//ģ������

		DStringList m_ActorAnimationList;										//�����б�	

		Fairy::LogicModel* m_pObjModel;											//objģ��ʵ��

		Fairy::LogicModel* m_pObjTemplate;										//obj�ļ�ģ��

		DString m_Objectname;													//obj�ļ���
		
		DStringList m_Meshnmamelist;											//mesh�ļ���

		DStringList m_Modelnamelist;											//ģ����

		Fairy::LogicModel::AttribMap m_objAttribute;							//attribute	

		Fairy::LogicModel::EntityMap m_objEntitylist;							//entitylist

		Fairy::LogicModel::MaterialMap m_objMateriallist;						//materiallsit

		Fairy::LogicModel::LocatorMap m_objLocatorlist;							//locator

		DStringList m_AniSkelist;												//AnimationSkeletonLinks

		typedef std::map<DString,DStringList> AniSkelistLink;
		AniSkelistLink m_AniSkelistLink;

		typedef std::list<BoneLocatorList>	LocatorBoneList;
		LocatorBoneList	m_LocatorBoneList;										//��ͷ��ҵ�����ƥ���

		typedef std::map<DString,DString>	AllGeneralLocatname;
		AllGeneralLocatname m_AllGeneralLocatname;								//����ͨ�ð󶨵�����

		int m_nLocatorNum;														//�󶨵����

		bool m_Conversion;														//�Ƿ���Ҫת��������ʽ

		bool m_IsAvatarTemplate;												//�Ƿ�ΪAVATARģ��

		std::vector<bool> m_IsButtonShow;

		bool m_IsInsertModelMap;												//�Ƿ����ģ�ͱ�

		bool m_IsCanSelectTemplate;												//�Ƿ����ѡ��objģ��
	
		Fairy::LogicModel::EntityMap m_AvatarTempEntity;
};
}
#endif