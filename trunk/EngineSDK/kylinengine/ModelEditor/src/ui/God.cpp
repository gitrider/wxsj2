#include "God.h"
#include "DataManipulator.h"

const DString HEROSKELETONINIFILE			= "男主角骨骼列表.ini";
const DString HEROINESKELETONINIFILE		= "女主角骨骼列表.ini";
const DString NORMALMANSKELETONINIFILE		= "标准男骨骼列表.ini";
const DString FATMANSKELETONINIFILE			= "胖男骨骼列表.ini";
const DString CHILDSKELETONINIFILE			= "小孩男骨骼列表.ini";
const DString STRONGMANSKELETONINIFILE		= "壮硕男骨骼列表.ini";
const DString NORMALWOMANSKELETONINIFILE	= "标准女骨骼列表.ini";
const DString LOCATORNAMEFILE				= "绑定点命名称.ini";
const DString AVATARFILELIST				= "AVATAR配置表.ini";

namespace Fairy {
	CGod::CGod(Fairy::SceneManipulator *sceneManipulator)
		: CEditDobject_NT(sceneManipulator)
		, m_pSceneManipulator(sceneManipulator)
		, m_rootNode(NULL)
		, m_pObjNode(NULL)
		, m_pObjModel(NULL)
		, m_pObjTemplate(NULL)
		, m_Identifier("")
		, m_Objectname("")
		, m_nLocatorNum(0)
		, m_Conversion(true)
		, m_IsAvatarTemplate(false)
		, m_IsInsertModelMap(false)
		, m_IsCanSelectTemplate(true)

	{
		InitShareSkeletonIni();
		LoadGeneralLocatorListIni();
		m_ActorAnimationList.clear();
		m_objAttribute.clear();
		m_objEntitylist.clear();
		m_objMateriallist.clear();
		m_objLocatorlist.clear();
		m_AniSkelist.clear();
		m_Meshnmamelist.clear();
		m_Modelnamelist.clear();
		ClearButtonShow();
		m_AniSkelistLink.clear();
	}

	CGod::~CGod()
	{
		m_ShareSkeletonIni.clear();
		LogicModelManager::getSingleton().destroyLogicModel(m_pObjModel);
	}

	void CGod::InitShareSkeletonIni()
	{
		m_ShareSkeletonIni.push_back(HEROSKELETONINIFILE);
		m_ShareSkeletonIni.push_back(HEROINESKELETONINIFILE);
		m_ShareSkeletonIni.push_back(NORMALMANSKELETONINIFILE);
		m_ShareSkeletonIni.push_back(FATMANSKELETONINIFILE);
		m_ShareSkeletonIni.push_back(CHILDSKELETONINIFILE);
		m_ShareSkeletonIni.push_back(STRONGMANSKELETONINIFILE);
		m_ShareSkeletonIni.push_back(NORMALWOMANSKELETONINIFILE);
	}

	void CGod::LoadGeneralLocatorListIni()
	{
		//先读取绑定点命名称.ini文件，获取所有的绑点名称
		Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(LOCATORNAMEFILE, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		while(!stream->eof())
		{
			DString line;
			line = stream->getLine();
			std::vector<String> vec = Ogre::StringUtil::split(line);			
			BoneLocatorList locatorlist(vec[0],vec[1]);
			m_LocatorBoneList.push_back(locatorlist);
		}
	}

	bool CGod::CreateModel(const DString& entfile,const DString& group)
	{
		if(entfile.empty())
			return false;

		m_rootNode = m_pSceneManipulator->getSceneManager()->getRootSceneNode();

		if(m_rootNode)
		{
			Fairy::CGod::m_pObjNode = Fairy::CGod::m_rootNode->createChildSceneNode();

			if(NULL == m_pObjNode)
				return false;
			else
			{
				SetObjectNode(m_pObjNode);
			}
		}

		DString name = m_pObjNode->getName();

		std::vector<Ogre::String> vec = Ogre::StringUtil::split(entfile,".");	

		DString modelname = vec[0] + ".obj";

		m_Objectname = modelname;

		DString meshfile = vec[0] + ".mesh";

		bool GetSkeInfoFromObj = true;

		//先拿到mesh模型里的数据，然后进行obj的创建
		SetGroupName(group);
		_AddEntity(vec[0],meshfile,vec[0],"");
		_LoadEntity();
		//更新骨骼数据
		UpdateAnimationList(meshfile);
		UpdateLinkSkeAnimationList(m_ShareSkeletonIni);

		if(vec.size() >= 2)
		{
			if(vec[1].compare("mesh") == 0)
			{
				m_Conversion = true;
				
				//如果有对应的obj文件就不需要转换
				Ogre::FileInfoListPtr fileInfoList =
					Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
					Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
					"*.obj");

				for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
				{	
					const Ogre::String& name = it->filename;

					std::basic_string <char>::size_type indexCh;
					indexCh = name.find(modelname);

					if(indexCh != std::string::npos)
					{
						m_Conversion = false;
						m_pObjModel = LogicModelManager::getSingleton().createLogicModel(vec[0], modelname);						
						break;
					}
				}
				//需要转换
				if(m_Conversion)
				{
					ModelConversion(vec[0],modelname);

					//获取角色动画列表
					AnimationSkeletonLinksMap::iterator itSke = m_LinkMap.begin();

					AnimationListIterator::iterator itAni;

					while(itSke != m_LinkMap.end())
					{
						itAni = itSke->second.begin();
						int nCount = itSke->second.size();
						for(int i=0;i<nCount;i++)
						{
							m_ActorAnimationList.push_back(itSke->second[i]);
						}
						++itSke;
					}

					GetSkeInfoFromObj = false;
				}
			}
			else
			{
				m_IsCanSelectTemplate = false;

				Avatar(vec[0], modelname);

				m_IsInsertModelMap = true;

				std::pair<CDataManipulator::ModelMap::iterator,bool> inserted = 
					GetDataManipulatr()->m_Modelmap.insert(CDataManipulator::ModelMap::value_type(vec[0],this));

				if(false == inserted.second)
				{
					Ogre::LogManager::getSingleton().logMessage(vec[0] + "插入模型表失败!");
					return false;
				}

				UpdateObjTemplateData(modelname);
			}


			//加载obj模板里的数据

			if(NULL == m_pObjModel)
				return false;

			if(GetSkeInfoFromObj)
			{
				//获取角色动画列表AnimationSkeletonLinksIterator
				Fairy::LogicModelManager::AnimationSkeletonLinksIterator it = 
					Fairy::LogicModelManager::getSingleton().getAnimationSkeletonLinksIterator(m_pObjModel->getName());

				bool bFirst = false;

				while(it.hasMoreElements())
				{
					DString aniname = it.peekNextKey();
					m_ActorAnimationList.push_back(aniname);
					if(!bFirst)
					{
						//m_DefaultAniName = aniname;
						bFirst = true;
					}
					it.moveNext();
				}				
			}

			m_pObjModel->setParentSceneNode(m_pObjNode);	

			m_pObjNode->setVisible(false);

			m_pObjModel->mModelMainNode->setVisible(true);

			//设置节点位置
			Ogre::Vector3 position;
			position.x = -30;
			position.z = -1550.3062;
			position = m_pSceneManipulator->_alignPosition(position);
			position.y = m_pSceneManipulator->getTerrainData()->getHeightAt(position.x, position.z);
			position = m_pSceneManipulator->_alignPosition(position);
			m_pObjNode->translate(position);

		}
		return true;
	}

	void CGod::ModelConversion(const DString& name,const DString& modelname)
	{
		LogicModel* pModel = new LogicModel(modelname, m_pSceneManipulator);

		if(NULL == pModel)
			return;

		//开始创建obj模板,这里只要EntityList和AnimationSkeletonLinks两个数据就可以了
		//新建一个link map
		LogicModelManager::AnimationSkeletonLinks* links = 
			LogicModelManager::getSingleton()._createAnimationSkeletonLinksForModel(modelname);

		if(NULL == links)
			return;

		//添加link map数据
		std::pair<LogicModelManager::LogicModelMap::iterator, bool> inserted = 
			LogicModelManager::getSingleton().mLogicModelTemplateMap.insert( LogicModelManager::LogicModelMap::value_type(modelname, pModel) );

		if (false == inserted.second)
		{
			delete pModel;
			pModel = NULL;
		}
		
		//添加EntityList数据
		int EntityListCount = m_EntityList.size();

		for(int i=0;i<EntityListCount;i++)
		{
			if(pModel)
			{
				pModel->mEntityMap.clear();

				DObject_NT::_EntityList::iterator it = m_EntityList.begin();
				while(it != m_EntityList.end())
				{
					pModel->addEntity((*it).Name,(*it).File,(*it).Material);
					++it;
				}
			}
		}

		m_pObjModel = LogicModelManager::getSingleton().createLogicModel(name, modelname);
	}

	bool CGod::Avatar(const Fairy::DString &name,const DString& modelname)
	{
		//创建一个空模板
		m_pObjModel = LogicModelManager::getSingleton().createLogicModel(name, modelname);
		
		if(NULL == m_pObjModel)
			return false;

		Fairy::LogicModel::EntityMap::iterator io = m_pObjModel->mEntityMap.begin();

		if(!io->second.mMeshName.empty())
			return true;

		m_IsAvatarTemplate = true;

		LogicModelManager::LogicModelMap::iterator it = 
			LogicModelManager::getSingleton().mLogicModelMap.find(name);

		if(it != LogicModelManager::getSingleton().mLogicModelMap.end())
		{
			//记录一下原始的obj中mEntityMap的内容，留着存obj文件用
			m_objEntitylist.clear();
			m_objEntitylist = m_pObjModel->mEntityMap;
			LogicModelManager::getSingleton().mLogicModelMap.erase(it);
			LogicModelManager::getSingleton().destroyLogicModel(m_pObjModel);
			m_pObjModel = NULL;
		}

		//空模板去读表里的数据
		LogicModel* pTemplate = const_cast<LogicModel*>(LogicModelManager::getSingleton().getTemplateByName(modelname));
		
		if(pTemplate)
		{
			pTemplate->mEntityMap.clear();

			Ogre::DataStreamPtr stream = 
				Ogre::ResourceGroupManager::getSingleton().openResource(AVATARFILELIST, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			
			bool bIsRecord = false;

			bool bIsLoadSkeData = false;
			
			while(!stream->eof())
			{
				DString line;
				line = stream->getLine();

				if(0 == line.compare("!"+modelname))
				{
					bIsRecord = false;
				}

				if(bIsRecord)
				{
					std::vector<Ogre::String> vec = Ogre::StringUtil::split(line);
					if(vec.size() >= 3)
						pTemplate->addEntity(vec[0],vec[1],vec[2]);

					if(!bIsLoadSkeData)
					{
						//拿到mesh模型里的数据，不然更新不了绑点数据
						SetGroupName(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
						_AddEntity(vec[0],vec[1],vec[0],"");
						_LoadEntity();
						//更新骨骼数据
						UpdateAnimationList(vec[1]);
						UpdateLinkSkeAnimationList(Fairy::CGod::m_ShareSkeletonIni);
						bIsLoadSkeData = true;
					}
				}

				if(0 == line.compare(modelname))
				{
					bIsRecord = true;
				}
			}
		}

		m_pObjModel = LogicModelManager::getSingleton().createLogicModel(name, modelname);

		return true;
	}

	void CGod::Assembled(const DString& entfile,bool FirstPart)
	{
		if(entfile.empty())
			return;
	
		//如果为第一个组装部件就需要为其创建结点
		if(FirstPart)
		{
			m_rootNode = m_pSceneManipulator->getSceneManager()->getRootSceneNode();

			if(m_rootNode)
			{
				m_pObjNode = m_rootNode->createChildSceneNode();

				if(NULL == m_pObjNode)
					return;
				else
				{
					SetObjectNode(m_pObjNode);
				}

				DString name = m_pObjNode->getName();

				std::vector<Ogre::String> vec = Ogre::StringUtil::split(entfile,".");	

				DString modelname = vec[0] + ".obj";

				SetGroupName(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

				_AddEntity(vec[0],entfile,vec[0],"");

				_LoadEntity(entfile);

				//更新骨骼数据
				UpdateAnimationList(entfile);

				UpdateLinkSkeAnimationList(Fairy::CGod::m_ShareSkeletonIni);

				m_pObjNode->setVisible(true);

				//设置父类方法中的场景管理指针
				SetSceneMgr(m_pSceneManipulator->getSceneManager());

				//设置节点位置
				Ogre::Vector3 position;
				position.x = -30;
				position.z = -1550.3062;
				position = m_pSceneManipulator->_alignPosition(position);
				position.y = m_pSceneManipulator->getTerrainData()->getHeightAt(position.x, position.z);
				position = m_pSceneManipulator->_alignPosition(position);
				Fairy::CGod::m_pObjNode->translate(position);

			}
		}
		else
		{
			DString name = Fairy::CGod::m_pObjNode->getName();

			std::vector<Ogre::String> vec = Ogre::StringUtil::split(entfile,".");	

			SetGroupName(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

			_AddEntity(vec[0],entfile,vec[0],"");

			_LoadEntity(entfile);
		}				
	}

	void CGod::PlayAnimation(const DString& aniname)
	{
		if(aniname.empty())
			return;

		bool bPlay = false;

		AnimationSkeletonLinksMap::iterator it = m_LinkMap.begin();
		AnimationListIterator::iterator itAni;
		while(it != m_LinkMap.end())
		{
			itAni = it->second.begin();
			int nCount = it->second.size();
			for(int i=0;i<nCount;i++)
			{
				if(aniname.compare(it->second[i]) == 0)
					bPlay = true;
			}
			++it;
		}

		if(bPlay)
		{
			if(m_pObjModel)
				m_pObjModel->createSkill(aniname);
		}
	}

	DString CGod::InitObject()
	{
		return m_Objectname;
	}

	DStringList CGod::InitAttributename()
	{
		DStringList list;

		if(NULL == m_pObjTemplate)
			return list;

		Fairy::LogicModel::AttribMap::iterator attributeit = m_pObjTemplate->mAttribMap.begin();

		while(attributeit != m_pObjTemplate->mAttribMap.end())
		{
			list.push_back(attributeit->first);
			++attributeit;
		}

		return list;
	}

	DStringList CGod::InitAttributehints()
	{
		DStringList list;

		list.push_back("Mesh");
		list.push_back("Material");
		list.push_back("SlotCreate");
		list.push_back("MovementType");
		list.push_back("DefaultAnimation");
		list.push_back("BoundingBox");
		list.push_back("Scale");
		list.push_back("HairMesh");

		return list;
	}

	DStringList CGod::InitEntityListname()
	{
		DStringList list;

		if(!m_IsAvatarTemplate)
		{
			DObject_NT::_EntityList::iterator it;

			for(it = m_EntityList.begin(); it != m_EntityList.end(); it ++)
			{
				list.push_back(it->Name);
			}

			m_Modelnamelist = list;
		}
		else
		{
			if(NULL == m_pObjTemplate)
				return list;

			Fairy::LogicModel::EntityMap::iterator entityit = m_pObjTemplate->mEntityMap.begin();

			while(entityit != m_pObjTemplate->mEntityMap.end())
			{
				list.push_back(entityit->first);

				++entityit;
			}
		}

		return list;
	}

	DString	CGod::InitEntityListFile()
	{
		DStringList list;

		if(!m_IsAvatarTemplate)
		{
			DObject_NT::_EntityList::iterator it;

			for(it = m_EntityList.begin(); it != m_EntityList.end(); it ++)
			{
				list.push_back(it->File);
			}

			m_Meshnmamelist = list;
		}

		if(list.size()<=0)
			return "";

		return list[0];
	}

	void CGod::InsertEntity(const DString& name,const DString& file,const DString& material)
	{
		m_objEntitylist.insert(Fairy::LogicModel::EntityMap::value_type(name,Fairy::LogicModel::EntityValue(file,material)));
	}

	DStringList CGod::InitMaterialListname()
	{
		DStringList list;

		if(NULL == m_pObjTemplate)
			return list;
		
		Fairy::LogicModel::MaterialMap::iterator matit = m_pObjTemplate->mMaterialMap.begin();

		while(matit != m_pObjTemplate->mMaterialMap.end())
		{
			list.push_back(matit->first);
			++matit;
		}

		return list;
	}

	DStringList CGod::InitMaterialTable()
	{
		DStringList list;

		if(NULL == GetDataManipulatr())
			return list;

		return GetDataManipulatr()->m_MatTable;
	}

	void CGod::InsertMaterial(const DString& name,const DString& file,const DString& material)
	{
		m_objMateriallist.insert(Fairy::LogicModel::MaterialMap::value_type(name,Fairy::LogicModel::MaterialValue(file,material)));
	}
	
	DStringList CGod::InitLocatorName()
	{
		//DStringList list;

		//Fairy::LogicModel::LocatorMap::iterator locatorit = m_pObjTemplate->mLocatorMap.begin();
		//
		//while(locatorit != m_pObjTemplate->mLocatorMap.end())
		//{
		//	list.push_back(locatorit->first);

		//	++locatorit;
		//}

		//return list;
		DStringList list;
		DStringList Delaylist;

		DObject_NT::SkelBoneLinks::iterator it;
		DObject_NT::BoneListIterator::iterator itbone;
		LocatorBoneList::iterator itlocator;
		Fairy::LogicModel::LocatorMap::iterator locatorit = m_pObjTemplate->mLocatorMap.begin();

		it = DObject_NT::SkelBoneLinksmap.begin();

		//if(it != DObject_NT::SkelBoneLinksmap.end())
		//{
			//itbone = (*it).second.begin();

			while(locatorit != m_pObjTemplate->mLocatorMap.end())
			//while(itbone != (*it).second.end())
			{
				bool bDelay = true;

				DString LocatornameAndBone = "";

				DString BoneName = "";

				itlocator = m_LocatorBoneList.begin();

				//进行绑点匹配项查寻
				while(itlocator != m_LocatorBoneList.end())
				{
					DString result = itlocator->FindMatching(locatorit->second.mBoneName/*(*itbone).c_str()*/);
					if(result.empty())
					{
						LocatornameAndBone = "未进行绑点设置";
						++itlocator;
					}
					else
					{
						LocatornameAndBone = result;
						bDelay = false;
						break;
					}
				}
				
				if(!LocatornameAndBone.empty())
				{
					BoneName = locatorit->second.mBoneName;
					LocatornameAndBone = locatorit->second.mBoneName + "\\" + LocatornameAndBone;
				}
				if(bDelay)
				{
					Delaylist.push_back(LocatornameAndBone.c_str());
				}
				else
				{
					list.push_back(LocatornameAndBone.c_str());
				}
				++locatorit;
				//++itbone;
			}
			//加载延迟项
			int nSize = Delaylist.size();
			//for(int i=0;i<nSize;i++)
			//{
				//list.push_back(Delaylist[i].c_str());
			//}
		//}
		return list;
	}

	DStringList CGod::InitAttachModel()
	{
		DStringList list;

		if(NULL == GetDataManipulatr())
			return list;

		return GetDataManipulatr()->m_ObjTable;
	}

	DStringList CGod::InitSlotName()
	{
		DStringList list;

		list.push_back("LeftWeaponObj");
		list.push_back("RightWeaponObj");
		list.push_back("LeftShieldObj");

		return list;
	}

	DStringList CGod::InitBoneName()
	{
		DStringList list;

		DObject_NT::SkelBoneLinks::iterator it;
		DObject_NT::BoneListIterator::iterator itbone;

		it = DObject_NT::SkelBoneLinksmap.begin();

		if(it != DObject_NT::SkelBoneLinksmap.end())
		{
			itbone = (*it).second.begin();
			while(itbone != (*it).second.end())
			{
				list.push_back((*itbone).c_str());
				++itbone;
			}
		}
		return list;
	}

	DStringList CGod::InitEffectName()
	{
		DStringList list;

		if(NULL == GetDataManipulatr())
			return list;

		return GetDataManipulatr()->m_EffectTable;
	}

	DStringList CGod::InitSkeletonName()
	{
		DStringList list;

		int nSize = m_AniSkelist.size();

		for(int i=0;i<nSize;i++)
		{
			list.push_back(m_AniSkelist[i]);
		}

		return list;
	}

	DStringList CGod::InitSkeletonTable()
	{
		DStringList list;

		if(NULL == GetDataManipulatr())
			return list;

		return GetDataManipulatr()->m_SkeletonTable;
	}

	void CGod::InsertLocator(const DString& name,const DString& bonename,const DString& slotname,const Ogre::Vector3& pos,const Ogre::Quaternion& orientation,bool transferable,bool translateFirst)
	{
		m_objLocatorlist.insert(Fairy::LogicModel::LocatorMap::value_type(name,Fairy::LogicModel::LocatorValue(bonename, pos, orientation, transferable, translateFirst)));
		
		
		Fairy::LogicModel::SlotMap &slotMap = m_objLocatorlist[name].mSlotMap;
		std::pair<Fairy::LogicModel::SlotMap::iterator, bool> inserted = 
			slotMap.insert( Fairy::LogicModel::SlotMap::value_type( slotname, Fairy::LogicModel::SlotValue("") ) );
	}

	bool CGod::AddLocator(const DString& name,const DString& bonename,const DString& slotname,const Ogre::Vector3& pos,const Ogre::Quaternion& orientation,bool transferable,bool translateFirst)
	{
		if(name.empty())
			return false;

		if(NULL == m_pObjModel)
			return false;

		if(m_pObjModel->mLocatorMap.count(name))	
		{
			return false;					
		}

		m_pObjModel->addLocator(name,bonename,pos,orientation);

		if(!slotname.empty())
			m_pObjModel->addSlot(name,slotname,"","");
		return true;								
	}	

	bool CGod::DelLocator(const DString& name)
	{
		if(NULL == m_pObjModel)
			return false;

		Fairy::LogicModel::LocatorMap::iterator it = m_pObjModel->mLocatorMap.find(name);

		if(it != m_pObjModel->mLocatorMap.end())	
		{
			m_pObjModel->DelLocator(name);
			return true;
		}
		return false;
	}

	int CGod::ChangeSlot(const DString& slotname,const DString& modelname)
	{
		if(slotname.empty() || modelname.empty())
			return 0;

		if(NULL == m_pObjModel)
			return 0;

		DString CheckBone;

		DString CheckHints;

		bool bCheck = false;

		DObject_NT::BoneListIterator::iterator itbone;

		int nCount  = m_BoneList.size();

		//增加一个错误判断
		if(slotname.compare("LeftWeaponObj") == 0)
		{
			CheckBone = "LeftHand";
			CheckHints = "LeftWeaponObj";
			bCheck = true;
		}
		else if(slotname.compare("RightWeaponObj") == 0)
		{
			CheckBone = "RightHand";
			CheckHints = "RightWeaponObj";
			bCheck = true;
		}
		else if(slotname.compare("LeftShieldObj") == 0)
		{
			CheckBone = "LeftShield";
			CheckHints = "LeftShieldObj";
			bCheck = true;
		}

		if(bCheck)
		{
			for(int i=0;i<nCount;i++)
			{
				if((m_BoneList[i]).compare(CheckBone) == 0)
				{
					Fairy::LogicModel::AttribMap::iterator it = m_pObjModel->mAttribMap.find(CheckHints);
					if(it != m_pObjModel->mAttribMap.end())
					{
						if(it->second.mHint.compare("SlotCreate") == 0)
						{
							goto CGod_ChangeSlot_CHANGE;
						}
						else
						{
							return 2;
						}
					}
					else
					{
						return 3;
					}
				}
			}
			return 0;
		}

CGod_ChangeSlot_CHANGE:

		m_pObjModel->changeSlotAttrib(slotname,modelname);

		m_pObjModel->_updateLocatorPos();

		return 1;
	}


	bool CGod::ChangeEffect(const DString& effectname,const DString& locatorname,const DString& bone)
	{
		if(effectname.empty() || locatorname.empty())
			return false;

		if(NULL == m_pObjModel)
			return false;

		if(!m_pObjModel->mLocatorMap.count(locatorname))	
		{
			return false;					
		}

		if(m_BoneList.size() <= 0)
			return false;

		bool correctdata = false;

		for(int i=0;i<(int)m_BoneList.size();i++)
		{
			if(bone.compare(m_BoneList[i]) == 0)
				correctdata = true;
		}
		
		if(correctdata)
		{
			Fairy::LogicModel::LocatorMap::iterator i = m_pObjModel->mLocatorMap.find(locatorname);

			if(i != m_pObjModel->mLocatorMap.end())
			{
				m_pObjModel->_getLocatorSceneNode(i->second);

				m_pObjModel->addEffect(effectname,locatorname);

				m_pObjModel->_updateLocatorPos();

				m_pObjModel->_updateAllEffects();
			}
		}
		return true;
	}

	bool CGod::SaveToObj(Ogre::String StrFileName)
	{
		long i = 0, j = 0;
		FILE* fout;
		fout = fopen(StrFileName.c_str(), "w");
		if(!fout) return false;

		if(NULL == m_pObjModel)
			return false;
		
		fprintf(fout, "<Object name=\"%s\">\n", GetObjName().c_str());

		//output attributes
		{
			Fairy::LogicModel::AttribMap::iterator it;
			fprintf(fout, "\t<Attributes>\n");
			for(it = m_pObjModel->mAttribMap.begin(); it != m_pObjModel->mAttribMap.end(); it ++)
			{
				fprintf(fout, "\t\t<Attribute name=\"%s\" value=\"%s\" hints=\"%s\"/>\n", it->first.c_str(), it->second.mAttribValue.c_str(), it->second.mHint.c_str());
			}
			fprintf(fout, "\t</Attributes>\n");
		}

		//output entitys
		{
			Fairy::LogicModel::EntityMap::iterator it;

			fprintf(fout, "\t<EntityList>\n");
			if(!m_IsAvatarTemplate)
			{
				int nSize = m_Meshnmamelist.size();
				for(int i=0;i<nSize;i++)
				{
					fprintf(fout, "\t\t<Entity name=\"%s\" file=\"%s\" material=\"%s\" condition=\"%s\"/>\n", m_Modelnamelist[i].c_str(), m_Meshnmamelist[i].c_str(), m_Modelnamelist[i].c_str(), "");
				}
			}
			else
			{
				for(it = m_pObjModel->mEntityMap.begin(); it != m_pObjModel->mEntityMap.end(); it ++)
				{
					fprintf(fout, "\t\t<Entity name=\"%s\" file=\"%s\" material=\"%s\" condition=\"%s\"/>\n", it->first.c_str(), it->second.mMeshName.c_str(), it->second.mMaterialName.c_str(), "");
				}
			}
			fprintf(fout, "\t</EntityList>\n");
		}

		//output matriallist
		{
			Fairy::LogicModel::MaterialMap::iterator it;
			fprintf(fout, "\t<MaterialList>\n");
			for(it = m_pObjModel->mMaterialMap.begin(); it != m_pObjModel->mMaterialMap.end(); it ++)
			{
				fprintf(fout, "\t\t<Material name=\"%s\" entity=\"%s\" material=\"%s\"/>\n", it->first.c_str(),it->second.mEntityEntry.c_str(),it->second.mMaterialName.c_str());
			}
			fprintf(fout, "\t</MaterialList>\n");
		}

		//output locators
		{
			Fairy::LogicModel::LocatorMap::iterator it;
			int nCount = static_cast<int>(m_pObjModel->mLocatorMap.size());
			char szCount[64];
			memset(szCount,0,64);
			sprintf_s(szCount,64,"\t<Locators count=%d>\n",nCount);
			fprintf(fout, szCount);
			for(it = m_pObjModel->mLocatorMap.begin(); it != m_pObjModel->mLocatorMap.end(); it ++)
			{
				Fairy::LogicModel::SlotMap::iterator sit;
				Ogre::Vector3 qv;
				fprintf(fout, "\t\t<Locator name=\"%s\" bonename=\"%s\"  x=\"%f\" y=\"%f\" z=\"%f\" qx=\"%f\" qy=\"%f\" qz=\"%f\" qw=\"%f\">\n", 
					it->first.c_str(), it->second.mBoneName.c_str(), it->second.mOffsetPos.x, it->second.mOffsetPos.y, it->second.mOffsetPos.z, it->second.mOffsetOrientation.x, it->second.mOffsetOrientation.y, it->second.mOffsetOrientation.z, it->second.mOffsetOrientation.w);
				for(sit = it->second.mSlotMap.begin(); sit != it->second.mSlotMap.end(); sit ++)
				{
					fprintf(fout, "\t\t\t<Slot name=\"%s\" object=\"%s\" attribs=\"%s\"/>\n", 
						sit->first.c_str(), "", "");
				}
				fprintf(fout, "\t\t</Locator>\n");
			}
			fprintf(fout, "\t</Locators>\n");
		}	
		
		//add by zhuwenchao 增加输出骨骼及动作明细
		//output skeleton
		{
			fprintf(fout, "\t<Skeletons>\n");
			AniSkelistLink::iterator it = m_AniSkelistLink.begin();
			while(it != m_AniSkelistLink.end())
			{
				fprintf(fout,"\t\t<Skeleton skeleton=\"%s\" animations=\"",it->first.c_str());
				DStringList namelist = it->second;
				DStringList::iterator ite = namelist.begin();
				while(ite != namelist.end())
				{
					fprintf(fout,"%s\t",ite->c_str());
					++ite;
				}
				fprintf(fout," \"/>\n");
				++it;
			}
			fprintf(fout, "\t</Skeletons>\n");
		}

		fprintf(fout, "</Object>\n");

		fclose(fout);

		return true;
	}

	void CGod::LoadObjTemplateRes(DString templatename)
	{
		if(templatename.empty())
			return;

		Fairy::LogicModel* modelTemplate = 
			const_cast<Fairy::LogicModel*>(LogicModelManager::getSingleton().getTemplateByName(templatename));

		m_pObjTemplate = modelTemplate;

		if(NULL == m_pObjModel)
			return;

		m_pObjModel->mAttribMap.clear();
		//attribute
		Fairy::LogicModel::AttribMap::iterator attributeit = modelTemplate->mAttribMap.begin();

		while(attributeit != modelTemplate->mAttribMap.end())
		{
			m_pObjModel->addAttribute(attributeit->first,attributeit->second.mAttribValue,attributeit->second.mHint);
			m_pObjModel->addAttribute(attributeit->first,attributeit->second.mAttribValue,attributeit->second.mHint);
			++attributeit;
		}

		m_pObjModel->mEntityMap.clear();
		//entitylist
		Fairy::LogicModel::EntityMap::iterator entityit = modelTemplate->mEntityMap.begin();

		while(entityit != modelTemplate->mEntityMap.end())
		{
			m_pObjTemplate->addEntity(entityit->first,entityit->second.mMeshName,entityit->second.mMaterialName);
			m_pObjModel->addEntity(entityit->first,entityit->second.mMeshName,entityit->second.mMaterialName);
			if(entityit->second.mMeshName.empty())
				m_IsAvatarTemplate = true;
			
			++entityit;
		}

		if(m_objEntitylist.size()>0)
		{
			//说明是AVATAR文件，且由obj文件创建，这里是让取回正常的值
			m_pObjModel->mEntityMap = m_objEntitylist;
		}

		m_pObjModel->mMaterialMap.clear();
		//materiallist
		Fairy::LogicModel::MaterialMap::iterator matit = modelTemplate->mMaterialMap.begin();

		while(matit != modelTemplate->mMaterialMap.end())
		{
			m_pObjTemplate->addMaterial(matit->first,matit->second.mEntityEntry,matit->second.mMaterialName);
			m_pObjModel->addMaterial(matit->first,matit->second.mEntityEntry,matit->second.mMaterialName);
			++matit;
		}

		m_pObjModel->mLocatorMap.clear();
		//locator && slot
		Fairy::LogicModel::LocatorMap::iterator locatorit = modelTemplate->mLocatorMap.begin();
		Fairy::LogicModel::SlotMap::iterator slotit;
		
		while(locatorit != modelTemplate->mLocatorMap.end())
		{
			m_pObjModel->addLocator(locatorit->first,locatorit->second.mBoneName,locatorit->second.mOffsetPos,locatorit->second.mOffsetOrientation);
			m_pObjModel->addLocator(locatorit->first,locatorit->second.mBoneName,locatorit->second.mOffsetPos,locatorit->second.mOffsetOrientation);
			slotit = locatorit->second.mSlotMap.begin();

			while(slotit != locatorit->second.mSlotMap.end())
			{
				m_pObjTemplate->addSlot(locatorit->first,slotit->first,slotit->second.mModelName,"");
				m_pObjModel->addSlot(locatorit->first,slotit->first,slotit->second.mModelName,"");
				++slotit;
			}

			++locatorit;
		}

		//skeleton
		m_AniSkelistLink.clear();

		m_AniSkelist.clear();

		Fairy::LogicModelManager::AnimationSkeletonLinksMap::iterator linkit = Fairy::LogicModelManager::getSingleton().mAnimationSkeletonLinksMap.find(templatename);
		
		if(linkit != Fairy::LogicModelManager::getSingleton().mAnimationSkeletonLinksMap.end())
		{
			Fairy::LogicModelManager::AnimationSkeletonLinks::iterator skeit = linkit->second->begin();
			DString tempskename;
			DStringList Aniname; 

			while(skeit != linkit->second->end())
			{
				DString skename = skeit->second;

				if(skename.compare(tempskename) != 0)
				{
					m_AniSkelist.push_back(skename);

					tempskename = skename;

					Aniname.clear();

					Fairy::LogicModelManager::AnimationSkeletonLinks::iterator skeitt = linkit->second->begin();
					
					while(skeitt != linkit->second->end())
					{
						if(tempskename.compare(skeitt->second) == 0)
						{
							Aniname.push_back(skeitt->first);
						}
						++skeitt;
					}

					m_AniSkelistLink.insert(AniSkelistLink::value_type(tempskename,Aniname));
				}

				++skeit;
			}
		}
	}

	std::vector<bool> CGod::GetGetUIShow()
	{
		std::vector<bool> boolvec;

		if(m_IsButtonShow.size()>=5)
			return m_IsButtonShow;

		return boolvec;
	}

	void CGod::UpdateObjTemplateData(DString templatename)
	{
		if(m_pObjTemplate)
			m_pObjTemplate = NULL;		

		LoadObjTemplateRes(templatename);

		if(GetDataManipulatr())
			GetDataManipulatr()->UpdateObjFileEditDlgUI();

		std::vector<bool> boolvec = GetGetUIShow();

		if(boolvec.size() >= 5)
		{
			GetDataManipulatr()->m_pObjFileEditDialog->DynamicShowUI(boolvec[0],boolvec[1],boolvec[2],boolvec[3],boolvec[4]);
		}
	}

	void CGod::ClearButtonShow()
	{
		m_IsButtonShow.clear();

		for(int i=0;i<5;i++)
		{
			m_IsButtonShow.push_back(true);
		}
	}

	void CGod::UpdateLocatorPosAndOriData(const DString& locatorname)
	{
		if(locatorname.empty())
			return;

		if(NULL == m_pObjModel)
			return;

		if(NULL == GetDataManipulatr())
			return;

		if(!m_pObjModel->mLocatorMap.count(locatorname))	
		{
			return;					
		}
		
		Fairy::LogicModel::LocatorMap::iterator it =  m_pObjModel->mLocatorMap.find(locatorname);

		if(it != m_pObjModel->mLocatorMap.end())
		{
			Ogre::Vector3 pos	 = it->second.mOffsetPos;
			Ogre::Quaternion ori = it->second.mOffsetOrientation;

			GetDataManipulatr()->m_pObjFileEditDialog->m_pLocatorPosTextCtrl->SetValue(
			Ogre::StringConverter::toString(pos.x)+
			" "+
			Ogre::StringConverter::toString(pos.y)+
			" "+
			Ogre::StringConverter::toString(pos.z));

			GetDataManipulatr()->m_pObjFileEditDialog->m_pLocatorOriTextCtrl->SetValue(
			Ogre::StringConverter::toString(ori.x)+
			" "+
			Ogre::StringConverter::toString(ori.y)+
			" "+
			Ogre::StringConverter::toString(ori.z)+
			" "+
			Ogre::StringConverter::toString(ori.w));
		}
	}

	void CGod::ChangeMat(const Ogre::String &name, const Ogre::String &value)
	{
		//实现方式待定
	}

	bool CGod::ChangeHints(const Ogre::String &name, const Ogre::String &hitns)
	{
		if(NULL == m_pObjModel)
			return false;

		return m_pObjModel->changeAttributeHints(name,hitns);
	}

	void CGod::SetTempEntity(Fairy::LogicModel* tempmap)
	{
		m_pObjModel->mEntityMap = m_pObjTemplate->mEntityMap;
	}

	void CGod::SaveTempEntity()
	{
		m_AvatarTempEntity = this->m_pObjModel->mEntityMap;
	}

	void CGod::GetTempntity()
	{
		 m_pObjModel->mEntityMap = m_AvatarTempEntity;
	}
}