//所有模型数据的基类
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
		EMT_ANTHROPOMONSTER		,				//人形怪
		EMT_HERO				,				//主角
		EMT_MODEL			,					//模型(建筑,特效等)
		EMT_RIDER				,				//坐骑
		EMT_WEAPON				,				//武器
		EMT_MONSTER				,				//异形怪
		EMT_NUM					
	};

	struct BoneLocatorList
	{
		BoneLocatorList()
		{
			m_locatorname = "";					//挂点名称
			m_Bonename = "";					//骨头名称
		}
		BoneLocatorList(DString locatorname,DString bonename)
		{
			m_locatorname = locatorname;
			m_Bonename = bonename;
		}
		DString FindMatching(DString name)		//查询匹配项
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
		virtual bool CreateModel(const DString& entfile,						//创建模型
			const DString& group = 
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);						

		virtual bool AddModelLocator(const DString& name,						//增加模型绑点						  		
			const DString& bonename,
			const Ogre::Vector3& offpos, 
			const Ogre::Quaternion& offrot){return true;}

		virtual bool AddModelSlot(const DString& locatorname,					//增加模型绑的插槽(用于绑定模型)						   
			const DString& slotname,
			const DString& slotobj, 
			const DStringList& initattribs){return true;}

		virtual bool AddModelEffect(const DString& effectname,					//增加模型绑定特效						  
			const DString& locatorname){return true;}

		virtual void UpdateModelLocatorPos(const DString& locatorname,			//更新模型挂点的位置、方向
			const Ogre::Vector3& position,
			const Ogre::Quaternion& quaternion){}

		virtual void PlayAnimation(const DString& aniname);						//播放动画

		virtual void ModelConversion(const DString& name,						//模型转换函数
			const DString& modelname);

		void InitShareSkeletonIni();											//初始化共享骨骼文件

		void LoadGeneralLocatorListIni();										//初始化通用绑定点文件

		//objcet
		virtual DString InitObject();											//初始化object选项

		//attribute name
		virtual DStringList InitAttributename();								//初始化attribute name

		//attribute hints
		virtual DStringList InitAttributehints();								//初始化attribute hints

		//entitylist name
		virtual DStringList InitEntityListname();								//初始化entity name

		//entitylist file
		virtual DString	InitEntityListFile();									//初始化entity file

		virtual void InsertEntity(const DString& name,							//插入一个entity
			const DString& file,const DString& material);

		//materiallist name
		virtual DStringList InitMaterialListname();								//初始化material name

		//materiallist
		virtual DStringList InitMaterialTable();								//初始化materialtable

		virtual void  InsertMaterial(const DString& name,						//插入一个material
			const DString& file,const DString& material);
		
		//locator name
		virtual DStringList InitLocatorName();									//初始化locator name

		//locator attachmodel
		virtual DStringList InitAttachModel();									//初始化locator attachmodel

		//locator slotname
		virtual DStringList InitSlotName();										//初始化locator slotname

		//effect name
		virtual DStringList InitEffectName();									//初始化effect name

		//bone name
		virtual DStringList InitBoneName();										//初始化bone name

		//skeleton name
		virtual DStringList InitSkeletonName();									//初始化skeleton name

		//skeleton table
		virtual DStringList InitSkeletonTable();								//初始化skeleton table

		virtual void InsertLocator(const DString& name,							//插入一个locator
			const DString& bonename,
			const DString& slotname,
			const Ogre::Vector3& pos = Ogre::Vector3::ZERO,
			const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY,
			bool transferable = false,
			bool translateFirst = false);

		virtual bool AddLocator(const DString& name,							//添加一个locator
			const DString& bonename,
			const DString& slotname,
			const Ogre::Vector3& pos = Ogre::Vector3::ZERO,
			const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY,
			bool transferable = false,
			bool translateFirst = false);

		virtual bool DelLocator(const DString& name);							//删除一个locator

		virtual int ChangeSlot(const DString& slotname,							//改变挂接物品
		   const DString& modelname);

		virtual bool ChangeEffect(const DString& effectname,					//改变挂接特效
		   const DString& locatorname,const DString& bone);

		virtual bool SaveToObj(Ogre::String StrFileName);						//生成obj文件

		virtual void LoadObjTemplateRes(DString templatename);					//加载obj模板资源

		virtual std::vector<bool> GetGetUIShow();								//获取要显示的UI

		bool Avatar(const Fairy::DString &name,const DString& modelname);		//

		void Assembled(const DString& entfile,bool FirstPart);					//组装模型

		void UpdateObjTemplateData(DString templatename);						//更新obj文件模板数据

		void ClearButtonShow();

		void UpdateLocatorPosAndOriData(const DString& locatorname);
		
		void ChangeMat(const Ogre::String &name, const Ogre::String &value);

		bool ChangeHints(const Ogre::String &name, const Ogre::String &hitns);	//改变属性提示

		void SetTempEntity(Fairy::LogicModel* tempmap);

		void SaveTempEntity();

		void GetTempntity();
	public:
		Ogre::SceneNode* m_rootNode;											//场景根节点

		Ogre::SceneNode* m_pObjNode;											//物体节点	

		Fairy::SceneManipulator* m_pSceneManipulator;							//场景管理器

		DStringList m_ShareSkeletonIni;											//共享骨骼配置文件列表

		DString m_Identifier;													//唯一标识符

		E_MODEL_TYPE m_ModelType;												//模型类型

		DStringList m_ActorAnimationList;										//动画列表	

		Fairy::LogicModel* m_pObjModel;											//obj模型实例

		Fairy::LogicModel* m_pObjTemplate;										//obj文件模板

		DString m_Objectname;													//obj文件名
		
		DStringList m_Meshnmamelist;											//mesh文件名

		DStringList m_Modelnamelist;											//模型名

		Fairy::LogicModel::AttribMap m_objAttribute;							//attribute	

		Fairy::LogicModel::EntityMap m_objEntitylist;							//entitylist

		Fairy::LogicModel::MaterialMap m_objMateriallist;						//materiallsit

		Fairy::LogicModel::LocatorMap m_objLocatorlist;							//locator

		DStringList m_AniSkelist;												//AnimationSkeletonLinks

		typedef std::map<DString,DStringList> AniSkelistLink;
		AniSkelistLink m_AniSkelistLink;

		typedef std::list<BoneLocatorList>	LocatorBoneList;
		LocatorBoneList	m_LocatorBoneList;										//骨头与挂点名称匹配表

		typedef std::map<DString,DString>	AllGeneralLocatname;
		AllGeneralLocatname m_AllGeneralLocatname;								//所有通用绑定点名称

		int m_nLocatorNum;														//绑定点个数

		bool m_Conversion;														//是否需要转换数据形式

		bool m_IsAvatarTemplate;												//是否为AVATAR模板

		std::vector<bool> m_IsButtonShow;

		bool m_IsInsertModelMap;												//是否插入模型表

		bool m_IsCanSelectTemplate;												//是否可以选择obj模板
	
		Fairy::LogicModel::EntityMap m_AvatarTempEntity;
};
}
#endif