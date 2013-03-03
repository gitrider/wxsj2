#include "DataManipulator.h"
#include <OgreBone.h>
#include <OgreSkeletonInstance.h>
#include <OgreEntity.h>
#include "Core/FairyUtils.h"
#include "Core/FairyResources.h"
#include "OgreSubMesh.h"
#include <OgreStringConverter.h>
#include <OgreLogManager.h>
#include <OgreStringVector.h>
#include "OgreMaterialManager.h"
#include "OgreWireBoundingBox.h"
#include "Core/FairyObject.h"
#include "Core/FairyObjectFactoryManager.h"
#include "Core/FairyStaticEntityObject.h"
namespace Fairy {

		Ogre::SceneNode* CDataManipulator::m_baseNode=0;
		Ogre::SceneNode* CDataManipulator::m_axex=0;
		Ogre::Entity*	 CDataManipulator::mDummy=0;
		Ogre::SceneNode* CDataManipulator::mAxisNode_x=0;
		Ogre::SceneNode* CDataManipulator::mAxisNode_y=0;
		Ogre::SceneNode* CDataManipulator::mAxisNode_z=0;
		Ogre::Entity*	 CDataManipulator::mAxis_x=0;
		Ogre::Entity*	 CDataManipulator::mAxis_y=0;
		Ogre::Entity*	 CDataManipulator::mAxis_z=0;

	CDataManipulator* CDataManipulator::m_DataManipulatr = NULL;
	CDataManipulator::CDataManipulator(SceneManipulator* sceneManipulator)
		: m_pSceneManipulator(sceneManipulator)
		, m_bBuild(false)
		, m_pAnimationSelector(NULL)
		, m_pObjFileEditorPanel(NULL)
		, m_pObjTemplate(NULL)
		, m_pObjSceneNode(NULL)
		, m_curEffectHandle(0)
		, m_IndicatorEntity(NULL)
		, m_bObjModified(false)
		, mShowBoundingBox(false)
		, mShowWorldBoundingBox(false)
		, mBoxIndicator(NULL)
		, mBoxSceneNode(NULL)
		, mSaveDefaultSkeleton(true)
		, m_pMeshObject()
	{
		m_MatTable.clear();
		m_ObjTable.clear();
		m_MatFileList.clear();
		m_EffectTable.clear();	
		m_SkeletonTable.clear();
		m_MeshTable.clear();

		mAnim=NULL;
		mAnimState=NULL;
		mAnimParam = new CUSTOM_PARAM();
		mAnimParam->animState=NULL;
		mAnimParam->stateEnabled=false;
		//mAnimParam->node=NULL;
	}

	CDataManipulator::~CDataManipulator()
	{
		LogicModelManager::getSingleton().destroyLogicModel(m_pObjTemplate);
		if (mBoxIndicator)
		{
			delete mBoxIndicator;
			mBoxIndicator = NULL;
		}
		if (mAnimParam)
		{
			delete mAnimParam;
			mAnimParam = NULL;
		}
	}
	
	void CDataManipulator::Build()
	{
		if(IsBuild())
			return;

		m_DataManipulatr = this;
		m_bBuild = true;

		if (!m_axex && !mDummy && !m_baseNode)
		{
			m_baseNode = m_pSceneManipulator->getSceneManager()->getRootSceneNode()->createChildSceneNode();
			m_axex = m_baseNode->createChildSceneNode();

			mDummy =m_pSceneManipulator->getSceneManager()->createEntity("MeshViewerCnDummy","axes.mesh");
			Ogre::Vector3 scale = Ogre::Vector3(2,2,2);
			m_axex->setScale(scale);

			Ogre::Vector3 position;
			position = Ogre::Vector3(0,0,0);

			m_axex->translate(position);
			m_axex->attachObject((Ogre::MovableObject*)mDummy);	

			// Create axis balls
			Ogre::MeshPtr sphereMesh = createCommonSphere("AxisEntity_x",1.5, 16, 16);
			// X
			m_pMaterialX = createPureColourMaterial(
				Ogre::ColourValue(1, 0, 0, 0.5)  );

			mAxis_x = m_pSceneManipulator->getSceneManager()->createEntity("AxisEntity_x", sphereMesh->getName());
			m_pMaterialX->setDepthCheckEnabled(false);
			m_pMaterialX->setDepthWriteEnabled(false);
			m_pMaterialX->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);
			//david-<<
			//mAxis_x->setNormaliseNormals(true);
			//david->>
			mAxis_x->setMaterialName(m_pMaterialX->getName());
			mAxisNode_x = m_axex->createChildSceneNode();
			mAxisNode_x->translate(Ogre::Vector3(13,0,0));
			mAxisNode_x->attachObject(mAxis_x);

			// Y
			m_pMaterialY = createPureColourMaterial(
				Ogre::ColourValue(0, 1, 0, 0.5)  );

			mAxis_y = m_pSceneManipulator->getSceneManager()->createEntity("AxisEntity_y", sphereMesh->getName());
			m_pMaterialY->setDepthCheckEnabled(false);
			m_pMaterialY->setDepthWriteEnabled(false);
			m_pMaterialY->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);
			//david-<<
			//mAxis_y->setNormaliseNormals(true);
			//david->>
			mAxis_y->setMaterialName(m_pMaterialY->getName());
			mAxisNode_y = m_axex->createChildSceneNode();
			mAxisNode_y->translate(Ogre::Vector3(0,13,0));
			mAxisNode_y->attachObject(mAxis_y);

			// Z
			m_pMaterialZ = createPureColourMaterial(
				Ogre::ColourValue(0, 0, 1, 0.5)  );

			mAxis_z = m_pSceneManipulator->getSceneManager()->createEntity("AxisEntity_z", sphereMesh->getName());
			m_pMaterialZ->setDepthCheckEnabled(false);
			m_pMaterialZ->setDepthWriteEnabled(false);
			m_pMaterialZ->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);
			//david-<<
			//mAxis_z->setNormaliseNormals(true);
			//david->>
			mAxis_z->setMaterialName(m_pMaterialZ->getName());
			mAxisNode_z = m_axex->createChildSceneNode();
			mAxisNode_z->translate(Ogre::Vector3(0,0,13));
			mAxisNode_z->attachObject(mAxis_z);
		}

		Ogre::MeshPtr sphereMesh = createCommonSphere("TestingEntity",10, 16, 16);
		Ogre::MaterialPtr material = createPureColourMaterial(
			Ogre::ColourValue(0.5, 0.5, 0, 0.5)  );
		material->setDepthCheckEnabled(false);
		material->setDepthWriteEnabled(false);
		material->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);

		m_IndicatorEntity = m_pSceneManipulator->getSceneManager()->createEntity("TestingEntity", sphereMesh->getName());
		//david-<<
		//m_IndicatorEntity->setNormaliseNormals(true);
		//david->>
		m_IndicatorEntity->setMaterialName(material->getName());
		
	}

	void CDataManipulator::UpdateAllResource()
	{
		OnInitObjTemplate();
		OnInitMatTable();
		OnInitEffectTable();
		OnInitSkeltonTable();
		OnInitMeshTable();
		if (m_pObjFileEditorPanel && m_pObjTemplate)
		{
			Ogre::String curSelName= m_pObjFileEditorPanel->m_entityNameCombo->GetStringSelection().c_str();
			if(curSelName.empty())
			{
				curSelName = m_pObjFileEditorPanel->m_entityNameCombo->GetValue().c_str();
			}
			GetDataManipulator()->UpdateEntityInfo(curSelName);

			curSelName= m_pObjFileEditorPanel->m_materialNameCombo->GetStringSelection().c_str();
			if(curSelName.empty())
			{
				curSelName = m_pObjFileEditorPanel->m_materialNameCombo->GetValue().c_str();
			}
			GetDataManipulator()->UpdateMaterialInfo(curSelName);

			UpdateLocatorInfo(m_pObjFileEditorPanel->m_curLocatorName);
		}
	}

	void CDataManipulator::UpdateObjResource()
	{
		OnInitObjTemplate();
		if (m_pObjTemplate)
		{
			UpdateLocatorInfo(m_pObjFileEditorPanel->m_curLocatorName);
		}		
	}

	void CDataManipulator::UpdateAnimationListUI()
	{
		if(m_pAnimationSelector)
		{
			m_pAnimationSelector->mIsMeshAnim = false;
			m_pAnimationSelector->Reload();
		}
	}

	void CDataManipulator::UpdateMeshAnimationListUI()
	{
		if(m_pAnimationSelector)
		{
			m_pAnimationSelector->mIsMeshAnim = true;
			m_pAnimationSelector->Reload();
		}
	}

	bool CDataManipulator::_updateBoneList()
	{
		bool showBone = true;
		if (m_pObjTemplate && m_pObjFileEditorPanel)
		{	
			Fairy::LogicModel::LocatorMap::iterator locatorIter= m_pObjTemplate->mLocatorMap.begin();	

			// For effect locator
			m_pObjFileEditorPanel->m_effectLocatorCombo->Clear();
			m_pObjFileEditorPanel->m_effectLocatorCombo->AppendString(wxEmptyString);

			while(locatorIter != m_pObjTemplate->mLocatorMap.end())
			{
				m_pObjFileEditorPanel->m_effectLocatorCombo->AppendString(locatorIter->first.c_str());
				++locatorIter;
			}

			m_pObjFileEditorPanel->m_boneNameCombo->Clear();			
			m_curBoneList.clear();

			// Add the empty string first
			m_pObjFileEditorPanel->m_boneNameCombo->AppendString(wxEmptyString);

			DString skeletonName = "";

			if ( (int)m_skeltonFileList.size()>0 )
			{
				skeletonName = m_skeltonFileList[0];
			} 			
			if ( skeletonName.empty() )
			{
				showBone = false;
				return showBone;
			}
			
			//Init the bone list
			Ogre::Skeleton::BoneIterator iter = _getBone(skeletonName);
			while (iter.hasMoreElements())
			{
				Ogre::Bone* bone = iter.getNext();
				m_pObjFileEditorPanel->m_boneNameCombo->AppendString(bone->getName().c_str());
				m_curBoneList.insert(BoneList::value_type(bone->getName(), bone->getName()));
			}	

			m_pObjFileEditorPanel->m_curLocatorName = "";
			//locatorName
			m_pObjFileEditorPanel->m_locatorNameCombo->Clear();
			m_pObjFileEditorPanel->m_locatorNameCombo->AppendString(NEWLOCATORTIP);
			m_pObjFileEditorPanel->m_locatorNameCombo->AppendString(DELLOCATORTIP);

			m_pObjFileEditorPanel->m_slotNameCombo->Enable(false);
			m_pObjFileEditorPanel->m_objNameCombo->Enable(false);
			m_pObjFileEditorPanel->m_attributeEffectCombo->Enable(false);

			locatorIter= m_pObjTemplate->mLocatorMap.begin();	

			while(locatorIter != m_pObjTemplate->mLocatorMap.end())
			{
				m_pObjFileEditorPanel->m_locatorNameCombo->AppendString(locatorIter->first.c_str());
				if (m_curBoneList.find(locatorIter->second.mBoneName)==m_curBoneList.end())
				{
					// Give the warning if the bone is not valid
					wxLogMessage(wxT("挂点<%s>对应的骨头<%s>没有找到!"), locatorIter->first.c_str(), locatorIter->second.mBoneName.c_str());
				}
				++locatorIter;
			}
		}
		return showBone;
	}

	bool CDataManipulator::_updateSkeAniList()
	{
		m_skeAniLinkList.clear();
		//Init the ske-ani link list for print the obj file
		for ( int idx = 0; idx < (int)m_skeltonFileList.size(); ++idx )
		{
			DStringList aniList;
			m_skeAniLinkList.insert(SkeAniLinkList::value_type(m_skeltonFileList[idx], aniList));
		}

		if (!m_pObjTemplate->mlinks)
		{
			return true;
		}

		for(std::map<Ogre::String, Ogre::String>::iterator iter = m_pObjTemplate->mlinks->begin();  iter != m_pObjTemplate->mlinks->end(); ++iter)
		{
			SkeAniLinkList::iterator skeIter = m_skeAniLinkList.find(iter->second);
			if( skeIter != m_skeAniLinkList.end())
			{
				skeIter->second.push_back(iter->first);
			}
		}
		return true;
	}

	bool CDataManipulator::_updateSkeletonList()
	{
		if (m_pObjFileEditorPanel && m_pObjTemplate)
		{	
			m_pObjFileEditorPanel->m_skeletonCombo->Clear();

			if (m_pObjTemplate->mlinks)
			{
				LogicModelManager::AnimationSkeletonLinks::iterator aniIter = m_pObjTemplate->mlinks->begin();
				while ( aniIter !=  m_pObjTemplate->mlinks->end() )
				{
					DString skeletonName = aniIter->second;
					bool bValid = true;
					for (int index = 0; index<(int)m_skeltonFileList.size(); ++index )
					{
						if (skeletonName == m_skeltonFileList[index])
						{
							// Existing skeleton file
							bValid = false;
							break;
						}
					}

					if (bValid)
					{
						m_skeltonFileList.push_back(skeletonName);
					}	
					++aniIter;
				}
			}

			int length = m_skeltonFileList.size();

			for(int i=0;i<length;i++)
			{
				m_pObjFileEditorPanel->m_skeletonCombo->AppendString(m_skeltonFileList[i].c_str());
			}
		}
		
		return true;
	}

	bool CDataManipulator::_updateEffectList()
	{
		if (m_pObjFileEditorPanel && m_pObjTemplate)
		{
			m_pObjFileEditorPanel->m_effectNameCombo->Clear();	

			m_pObjFileEditorPanel->m_effectNameCombo->AppendString(NEWEFFECTTIP);
			m_pObjFileEditorPanel->m_effectNameCombo->AppendString(DELEFFECTTIP);	

			m_pObjFileEditorPanel->m_effectFileCombo->SetValue(wxEmptyString);
			m_pObjFileEditorPanel->m_effectLocatorCombo->SetValue(wxEmptyString);

			Fairy::LogicModel::EffectMap::iterator effectIter = m_pObjTemplate->mEffectMap.begin();

			while(effectIter != m_pObjTemplate->mEffectMap.end())
			{
				m_pObjFileEditorPanel->m_effectNameCombo->AppendString(effectIter->first.c_str());
				if (m_pObjTemplate->mLocatorMap.find(effectIter->second.mLocator) == m_pObjTemplate->mLocatorMap.end())
				{
					// Give the warning if the locator is not valid
					wxLogMessage(wxT("特效<%s>对应的挂点<%s>没有找到!"), effectIter->first.c_str(), effectIter->second.mLocator.c_str());
				}
				++ effectIter;
			}
		}
		return true;
	}

	void CDataManipulator::UpdateObjFileEditDlgUI2()
	{
		if (m_pObjFileEditorPanel && m_pObjTemplate)
		{
			m_skeltonFileList.clear();
			if ( m_pObjTemplate->mSkeletonEntity )
			{
				m_skeltonFileList.push_back(m_pObjTemplate->mSkeletonEntity->getSkeleton()->getName());
				_addSkeleton(m_pObjTemplate->mSkeletonEntity->getSkeleton()->getName());
			}
			m_baseNode->setPosition(Ogre::Vector3::ZERO);
			m_baseNode->setOrientation(Ogre::Quaternion::IDENTITY);

			m_axex->setPosition(Ogre::Vector3::ZERO);
			m_axex->setOrientation(Ogre::Quaternion::IDENTITY);
			m_pObjFileEditorPanel->m_fileNameTxt->SetValue(m_CurObjFile.c_str());

			m_pObjFileEditorPanel->m_posText->SetValue(wxEmptyString);
			m_pObjFileEditorPanel->m_rotText->SetValue(wxEmptyString);			

			bool showFlags[6];
			DStringList list;	
			int listLength=0;

			////attributes
			////name
			m_pObjFileEditorPanel->m_attributeNameCombo->Clear();
			m_pObjFileEditorPanel->m_slotNameCombo->Clear();

			Fairy::LogicModel::AttribMap::iterator attributeIter = m_pObjTemplate->mAttribMap.begin();
			m_pObjFileEditorPanel->m_slotNameCombo->AppendString(_T(""));

			DStringList entityList;
			DStringList materialList;
			while(attributeIter != m_pObjTemplate->mAttribMap.end())
			{
				list.push_back(attributeIter->first);
				if(attributeIter->second.mHint.compare(LogicModel::SLMT_CREATE_HINT) == 0)
				{
					m_pObjFileEditorPanel->m_slotNameCombo->AppendString(attributeIter->first.c_str());
				}	
				else if (attributeIter->second.mHint.compare(LogicModel::MESH_HINT) == 0)
				{
					entityList.push_back(attributeIter->first);
				}
				else
				{
					materialList.push_back(attributeIter->first);
				}
				++attributeIter;
			}

			listLength = list.size();

			showFlags[0] = (listLength <= 0)?false:true;

			for(int i=0; i<listLength; ++i)
			{
				m_pObjFileEditorPanel->m_attributeNameCombo->AppendString(list[i].c_str());
			}

			if (showFlags[0])
			{
				// show the first attribute default
				DString attibuteName = list[0];
				m_pObjFileEditorPanel->m_curAttibuteName = attibuteName;
				m_pObjFileEditorPanel->m_attributeNameCombo->SetStringSelection(attibuteName.c_str());
				UpdateAttributeInfo(attibuteName);
			}

			////entitylist
			////name
			m_pObjFileEditorPanel->m_entityNameCombo->Clear();
			m_pObjFileEditorPanel->m_materialEntityCombo->Clear();

			listLength = entityList.size();

			showFlags[1] = (listLength <= 0)?false:true;

			for( int i=0; i<listLength; ++i )
			{
				m_pObjFileEditorPanel->m_entityNameCombo->AppendString(entityList[i].c_str());
				m_pObjFileEditorPanel->m_materialEntityCombo->AppendString(entityList[i].c_str());
			}	

			if (showFlags[1])
			{
				// show the first attribute default
				DString entityName = entityList[0];
				m_pObjFileEditorPanel->m_entityNameCombo->SetStringSelection(entityName.c_str());
				UpdateEntityInfo(entityName);
			}

			////materiallist
			////name
			m_pObjFileEditorPanel->m_materialNameCombo->Clear();

			listLength = materialList.size();

			showFlags[2] = (listLength <= 0)?false:true;

			for(int i=0; i<listLength; ++i)
			{
				m_pObjFileEditorPanel->m_materialNameCombo->AppendString(materialList[i].c_str());
			}

			if (showFlags[2])
			{
				// show the first attribute default
				DString materialName = materialList[0];
				m_pObjFileEditorPanel->m_entityNameCombo->SetStringSelection(materialName.c_str());
				UpdateMaterialInfo(materialName);
			}
			
			// skeleton			
			showFlags[5] = _updateSkeletonList();

			// effects
			showFlags[4] = _updateEffectList();
			
			// bone and locator
			showFlags[3] = _updateBoneList();
			
			m_pObjFileEditorPanel->SwitchUI(IFS_FILE_SIZER, true);	
			
			for ( int idx = 0; idx < 6; ++idx )
			{
				m_pObjFileEditorPanel->SwitchUI(IFS_ATTRIBUTE_SIZER+idx, showFlags[idx]);
			}
		}
	}


	void CDataManipulator::OnInitObjTemplate()
	{
		if(NULL == m_pObjFileEditorPanel)
			return;		

		Ogre::FileInfoListPtr fileInfoList =
			Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			"*.obj");

		m_ObjTable.clear();
		m_pObjFileEditorPanel->m_objNameCombo->Clear();

		m_pObjFileEditorPanel->m_objNameCombo->AppendString(_T(""));
		for(Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
		{	
			const Ogre::String name = it->filename;
			m_ObjTable.push_back(name);
			m_pObjFileEditorPanel->m_objNameCombo->AppendString(name.c_str());
		}
	}

	void CDataManipulator::OnInitMatTable()
	{
		Ogre::FileInfoListPtr fileInfoList =
			Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			"*.material");

		static bool first_call = true;
		if (first_call)
		{
			for(Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
			{	
				m_MatFileList.insert(std::map<DString,DString>::value_type(it->filename, it->filename));
			}
			first_call = false;
		}
		else
		{
			for(Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
			{	
				if (m_MatFileList.find(it->filename) == m_MatFileList.end())
				{
					Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(it->filename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
					Ogre::MaterialManager::getSingleton().parseScript(stream, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
					m_MatFileList.insert(std::map<DString,DString>::value_type(it->filename, it->filename));
				}
				else
				{
					//TODO:reload the resource
				}
			}
		}		

		m_MatTable.clear();

		Ogre::MaterialManager::ResourceMapIterator materialIter = Ogre::MaterialManager::getSingleton().getResourceIterator();
		while (materialIter.hasMoreElements())
		{
			Ogre::MaterialPtr material = materialIter.getNext();
			m_MatTable.push_back(material->getName());
		}

		m_pObjFileEditorPanel->m_materialCombo->Clear();
		m_pObjFileEditorPanel->m_entityMaterialCombo->Clear();

		int nMateriallistEntityCount = m_MatTable.size();


		m_pObjFileEditorPanel->m_entityMaterialCombo->AppendString(_T(""));
		m_pObjFileEditorPanel->m_materialCombo->AppendString(_T(""));

		for(int i=0;i<nMateriallistEntityCount;i++)
		{	
			m_pObjFileEditorPanel->m_materialCombo->AppendString(m_MatTable[i].c_str());
			m_pObjFileEditorPanel->m_entityMaterialCombo->AppendString(m_MatTable[i].c_str());
		}
	}

	void CDataManipulator::OnInitHintsTable()
	{
		if (m_pObjFileEditorPanel)
		{
			m_pObjFileEditorPanel->m_attributeHintsCombo->AppendString(LogicModel::MESH_HINT);
			m_pObjFileEditorPanel->m_attributeHintsCombo->AppendString(LogicModel::MATERIAL_HINT);
			m_pObjFileEditorPanel->m_attributeHintsCombo->AppendString(LogicModel::SLMT_CREATE_HINT);
		}
	}

	void CDataManipulator::OnInitMeshTable()
	{
		Ogre::FileInfoListPtr fileInfoList =
			Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			"*.mesh");

		for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
		{	
			const Ogre::String name = it->filename;
			m_MeshTable.push_back(name.c_str());
		}

		m_pObjFileEditorPanel->m_entityFileCombo->Clear();

		int nMeshlistCount = m_MeshTable.size();

		m_pObjFileEditorPanel->m_entityFileCombo->AppendString(_T(""));
		for(int i=0;i<nMeshlistCount;i++)
		{
			m_pObjFileEditorPanel->m_entityFileCombo->AppendString(m_MeshTable[i].c_str());
		}
	}

	void CDataManipulator::OnInitEffectTable()
	{
		Ogre::FileInfoListPtr fileInfoList =
			Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			"*.effect");

		m_EffectTable.clear();

		for(Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
		{	
			const Ogre::String name = it->filename;
			Ogre::StringVector effect = Ogre::StringUtil::split(name,".");
			m_EffectTable.push_back(effect[0].c_str());
		}
		m_pObjFileEditorPanel->m_attributeEffectCombo->Clear();
		m_pObjFileEditorPanel->m_attributeEffectCombo->AppendString(_T(""));

		m_pObjFileEditorPanel->m_effectFileCombo->Clear();
		m_pObjFileEditorPanel->m_effectFileCombo->AppendString(_T(""));

		int nEffectlistCount = m_EffectTable.size();

		for(int i=0;i<nEffectlistCount;i++)
		{
			m_pObjFileEditorPanel->m_attributeEffectCombo->AppendString(m_EffectTable[i].c_str());
			m_pObjFileEditorPanel->m_effectFileCombo->AppendString(m_EffectTable[i].c_str());
		}
	}

	void CDataManipulator::OnInitSkeltonTable()
	{
		Ogre::FileInfoListPtr fileInfoList =
			Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			"*.Skeleton");

		m_SkeletonTable.clear();

		for(Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
		{	
			const Ogre::String name = it->filename;
			m_SkeletonTable.push_back(name.c_str());
		}
		m_pObjFileEditorPanel->m_resCombo->Clear();

		int nSkeletontablelistCount = m_SkeletonTable.size();

		for(int i=0;i<nSkeletontablelistCount;i++)
		{
			m_pObjFileEditorPanel->m_resCombo->AppendString(m_SkeletonTable[i].c_str());
		}
	}

	void CDataManipulator::LoadMeshRes(Ogre::String meshName)
	{
		m_bObjModified = false;

		mAnimParam->stateEnabled=false;
		mAnimParam->animState = NULL;

		m_CurMeshFile = meshName;

		// First hide the logic model
		if (!m_CurObjFile.empty() && m_pObjTemplate)
		{
			m_pObjSceneNode->setVisible(false);
		}

		if (m_pMeshObject)
		{
			// detach the current object first
			m_pSceneManipulator->removeObject(m_pMeshObject->getName());
			m_pMeshObject.reset();
		}

		if (meshName.empty())
		{
			return;
		}

		m_pMeshObject = ObjectFactoryManager::getSingleton().createInstance("StaticEntity");

		Fairy::StaticEntityObject* entity = static_cast<Fairy::StaticEntityObject*>((m_pMeshObject).get());
		entity->setMeshName(meshName);
		m_pMeshObject->createRenderInstance(m_pSceneManipulator);
		m_pMeshObject->setProperty("position", Ogre::Vector3::ZERO);
		m_pSceneManipulator->addObject(m_pMeshObject);
	}

	void CDataManipulator::LoadObjTemplateRes(Ogre::String templatename)
	{
		if (m_pMeshObject)
		{
			// detach the current object first
			m_pSceneManipulator->removeObject(m_pMeshObject->getName());
			m_pMeshObject.reset();
		}

		m_bObjModified =false;

		mAnimParam->stateEnabled=false;
		mAnimParam->animState = NULL;

		m_CurObjFile = templatename;

		if (m_baseNode)
		{
			Ogre::SceneNode* parent = m_baseNode->getParentSceneNode();
			if (parent)
			{
				parent->removeChild(m_baseNode);
			}
		}

		if(templatename.empty())
			return;

		Ogre::StringVector vec = Ogre::StringUtil::split(templatename,".");	

		if( m_pObjTemplate )
		{
			if (m_curEffectHandle)
			{
				m_pObjTemplate->delEffect(m_curEffectHandle);
				m_curEffectHandle = 0;
			}

			if (m_pObjTemplate->mSkeletonEntity)
			{
				m_pObjTemplate->mSkeletonEntity->detachObjectFromBone(m_IndicatorEntity);
			}			
			m_pObjTemplate = NULL;		
		}

		LogicModelManager::getSingleton().destroyAllLogicModels();
	
 	/*	if (LogicModelManager::getSingleton().getTemplateByName(templatename) == NULL)
 		{
			Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(templatename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			LogicModelManager::getSingleton().parseScript(stream,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		}	*/

		if (LogicModelManager::getSingleton().getTemplateByName(templatename))
		{
			Fairy::LogicModelManager::LogicModelMap::iterator iter = LogicModelManager::getSingleton().mLogicModelTemplateMap.find(templatename);
			if(iter != LogicModelManager::getSingleton().mLogicModelTemplateMap.end() )
			{
				Fairy::LogicModel* pModel = iter->second;
				if (pModel)
				{
					delete pModel;
					pModel = NULL;
				}
				LogicModelManager::getSingleton().mLogicModelTemplateMap.erase(iter);
			}
		}		

		Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(templatename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		LogicModelManager::getSingleton().parseScript(stream,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		
		m_pObjTemplate = LogicModelManager::getSingleton().createLogicModel(vec[0], templatename);
		m_pObjTemplate->mModelMainNode->addChild(m_baseNode);

 		// Init the scene node
 		Ogre::SceneNode* pRootNode = m_pSceneManipulator->getSceneManager()->getRootSceneNode();
 		if(pRootNode && !m_pObjSceneNode)
 		{
 			m_pObjSceneNode = pRootNode->createChildSceneNode();
 
 			if(!m_pObjSceneNode)
 			{
 				return;
 			}	
			
			//设置节点位置
 			Ogre::Vector3 position;
 			position.x = 0;
 			position.z = 0;
 			position = m_pSceneManipulator->_alignPosition(position);
 			position.y = m_pSceneManipulator->getTerrainData()->getHeightAt(position.x, position.z);
 			position = m_pSceneManipulator->_alignPosition(position);
 			m_pObjSceneNode->translate(position);		
 		}

		m_pObjSceneNode->setVisible(true);
 
 		m_pObjTemplate->setParentSceneNode(m_pObjSceneNode);
 		m_pObjTemplate->mModelMainNode->setVisible(true);		
		m_pObjTemplate->_updateAllEffects();
		m_pObjTemplate->_updateLocatorPos();

		

		if (m_pObjTemplate)
		{
			m_pObjTemplate->getSceneNode()->showBoundingBox(mShowBoundingBox);
		}

		if (!mBoxSceneNode)
		{
			// Build the base node first
			mBoxSceneNode = pRootNode->createChildSceneNode();	
			if (mBoxSceneNode)
			{
				if (!mBoxIndicator)
				{
					mBoxIndicator = new Ogre::WireBoundingBox();
					mBoxIndicator->setCastShadows(false);
					mBoxIndicator->setQueryFlags(0);
				}				
			}	
		}
		else
		{
			if (mBoxIndicator)
			{
				if (mBoxIndicator->getParentSceneNode())
				{
					mBoxIndicator->getParentSceneNode()->detachObject(mBoxIndicator->getName());
				}
			}
		}
		showWorldBoundingBox(mShowWorldBoundingBox);
	}

	void CDataManipulator::showWorldBoundingBox(bool bShow)
	{
		if (mBoxIndicator && m_pObjTemplate && bShow)
		{
			Ogre::Vector3 additionPos = Ogre::Vector3::ZERO;
			Ogre::Matrix4 addTrans = Ogre::Matrix4::IDENTITY;
			/*if (m_pObjTemplate->mSkeletonEntity)
			{
				Ogre::Bone* rootBone = m_pObjTemplate->mSkeleton->getRootBone();
				if (rootBone)
				{
					additionPos = rootBone->getPosition()-rootBone->getInitialPosition();
					addTrans.setTrans(additionPos);
				}
			}*/
			Ogre::AxisAlignedBox aabb = m_pObjTemplate->getBoundingBox();
			aabb.transformAffine(m_pObjTemplate->getSceneNode()->_getFullTransform()*addTrans);
			mBoxIndicator->setupBoundingBox(aabb);
			mBoxSceneNode->attachObject(mBoxIndicator);
		}
		else if (mBoxIndicator)
		{
			if (mBoxIndicator->getParentSceneNode())
			{
				mBoxIndicator->getParentSceneNode()->detachObject(mBoxIndicator->getName());
			}
		}
	}

	void CDataManipulator::switchBoundingBox(bool bWorld)
	{
		if (bWorld)
		{
			mShowWorldBoundingBox = !mShowWorldBoundingBox;
			showWorldBoundingBox(mShowWorldBoundingBox);
		}
		else
		{
			mShowBoundingBox = !mShowBoundingBox;
			if (m_pObjTemplate)
			{
				m_pObjTemplate->getSceneNode()->showBoundingBox(mShowBoundingBox);
			}
		}
		
	}

	bool CDataManipulator::SaveToObj(Ogre::String fileName)
	{
		long i = 0, j = 0;
		FILE* fout;
		fout = fopen(fileName.c_str(), "w");
		if(!fout) return false;

		if(NULL == m_pObjTemplate)
			return false;

		wxString pathname = fileName;
		wxString name;
		wxFileName::SplitPath(pathname, NULL, &name, NULL);

		fprintf(fout, "<Object name=\"%s\">\n", name.c_str());

		//output attributes
		{
			Fairy::LogicModel::AttribMap::iterator it;
			fprintf(fout, "\t<Attributes>\n");
			for(it = m_pObjTemplate->mAttribMap.begin(); it != m_pObjTemplate->mAttribMap.end(); it ++)
			{
				fprintf(fout, "\t\t<Attribute name=\"%s\" value=\"%s\" hints=\"%s\"/>\n", it->first.c_str(), it->second.mAttribValue.c_str(), it->second.mHint.c_str());
			}
			fprintf(fout, "\t</Attributes>\n");
		}

		//output entitys
		{
			Fairy::LogicModel::EntityMap::iterator it;

			fprintf(fout, "\t<EntityList>\n");
				for(it = m_pObjTemplate->mEntityMap.begin(); it != m_pObjTemplate->mEntityMap.end(); it ++)
				{
					fprintf(fout, "\t\t<Entity name=\"%s\" file=\"%s\" material=\"%s\" condition=\"%s\"/>\n", it->first.c_str(), it->second.mMeshName.c_str(), it->second.mMaterialName.c_str(), "");
				}
			fprintf(fout, "\t</EntityList>\n");
		}

		//output matriallist
		{
			Fairy::LogicModel::MaterialMap::iterator it;
			fprintf(fout, "\t<MaterialList>\n");
			for(it = m_pObjTemplate->mMaterialMap.begin(); it != m_pObjTemplate->mMaterialMap.end(); it ++)
			{
				fprintf(fout, "\t\t<Material name=\"%s\" entity=\"%s\" material=\"%s\"/>\n", it->first.c_str(),it->second.mEntityEntry.c_str(),it->second.mMaterialName.c_str());
			}
			fprintf(fout, "\t</MaterialList>\n");
		}

		//output locators
		{
			Fairy::LogicModel::LocatorMap::iterator it;
			int nCount = static_cast<int>(m_pObjTemplate->mLocatorMap.size());
			char szCount[64];
			memset(szCount,0,64);
			sprintf_s(szCount,64,"\t<Locators count=%d>\n",nCount);
			fprintf(fout, szCount);
			for(it = m_pObjTemplate->mLocatorMap.begin(); it != m_pObjTemplate->mLocatorMap.end(); it ++)
			{
				Ogre::Radian angle;
				Ogre::Vector3 axis = Ogre::Vector3::ZERO;
				Ogre::Quaternion tempRot = it->second.mOffsetOrientation;
				tempRot.ToAngleAxis(angle, axis);

				Fairy::LogicModel::SlotMap::iterator sit;
				Ogre::Vector3 qv;
				fprintf(fout, "\t\t<Locator name=\"%s\" bonename=\"%s\"  x=\"%f\" y=\"%f\" z=\"%f\" qx=\"%f\" qy=\"%f\" qz=\"%f\" qw=\"%f\">\n", 
					it->first.c_str(), it->second.mBoneName.c_str(), it->second.mOffsetPos.x, it->second.mOffsetPos.y, it->second.mOffsetPos.z, axis.x, axis.y, axis.z, angle.valueRadians());
				for(sit = it->second.mSlotMap.begin(); sit != it->second.mSlotMap.end(); sit ++)
				{
					fprintf(fout, "\t\t\t<Slot name=\"%s\" object=\"%s\" attribs=\"%s\"/>\n", 
						sit->first.c_str(), sit->second.mModelName.c_str(), "");
				}
				fprintf(fout, "\t\t</Locator>\n");
			}
			fprintf(fout, "\t</Locators>\n");
		}	

		//output effects
		{
			fprintf(fout, "\t<Effects>\n");
			Fairy::LogicModel::EffectMap::iterator it;
			for(it = m_pObjTemplate->mEffectMap.begin(); it != m_pObjTemplate->mEffectMap.end(); it ++)
			{
				Fairy::LogicModel::EffectValue& tempValue = it->second;
				fprintf(fout, "\t\t<Effect name=\"%s\" effect=\"%s\"  locator=\"%s\" translateAll=\"%s\"/>\n", 
					it->first.c_str(), tempValue.mEffectName.c_str(), tempValue.mLocator.c_str(), Ogre::StringConverter::toString(tempValue.mTranslateAll).c_str(), Ogre::StringConverter::toString(tempValue.mEffectColour).c_str());
			}
			fprintf(fout, "\t</Effects>\n");
		}	


		//output skeleton
 		{
 			fprintf(fout, "\t<Skeletons>\n");

			_updateSkeAniList();
			SkeAniLinkList::iterator iter = m_skeAniLinkList.begin();

			Ogre::String defaultSkeleton = "";
			if (m_pObjTemplate->mSkeletonEntity)
			{
				defaultSkeleton = m_pObjTemplate->mSkeletonEntity->getSkeleton()->getName();
				Ogre::StringUtil::toLowerCase(defaultSkeleton);
			}			

  			while(iter != m_skeAniLinkList.end())
  			{
				// check the default skeleton
				if (!mSaveDefaultSkeleton && !defaultSkeleton.empty())
				{					
					Ogre::String curSkeleton = iter->first;
					Ogre::StringUtil::toLowerCase(curSkeleton);
					if (defaultSkeleton == curSkeleton)
					{
						++iter;
						continue;
					}
				}
  				fprintf(fout,"\t\t<Skeleton skeleton=\"%s\" animations=\"",iter->first.c_str());
  				DStringList namelist = iter->second;
  				DStringList::iterator aniIter = namelist.begin();
  				while(aniIter != namelist.end())
  				{
  					fprintf(fout,"%s\t",aniIter->c_str());
  					++aniIter;
  				}
  				fprintf(fout," \"/>\n");
  				++iter;
  			}
  			fprintf(fout, "\t</Skeletons>\n");
 		}

		fprintf(fout, "</Object>\n");

		fclose(fout);

		return true;
	}

	void CDataManipulator::UpdateAttributeInfo(DString attibuteName)
	{		
		if (m_pObjFileEditorPanel && m_pObjTemplate)
		{	
			m_pObjFileEditorPanel->m_attributeValueText->Enable(false);
			////attributes name
			Fairy::LogicModel::AttribMap::iterator attributeIter = m_pObjTemplate->mAttribMap.find(attibuteName);

			if(attributeIter != m_pObjTemplate->mAttribMap.end())
			{
				m_pObjFileEditorPanel->m_attributeHintsCombo->SetValue(attributeIter->second.mHint.c_str());
				m_pObjFileEditorPanel->m_attributeValueText->SetValue(attributeIter->second.mAttribValue.c_str());	

				if (attibuteName == LogicModel::SCALE_FACTOR ||
					attibuteName == LogicModel::MOVEMENT_TYPE_ATTRIBUTE ||
					attibuteName == LogicModel::DEFAULT_ANIM_NAME ||
					attibuteName == LogicModel::BOUNDING_BOX)
				{
					m_pObjFileEditorPanel->m_attributeValueText->Enable(true);
				}				
			}
		}
	}

	bool CDataManipulator::ChangeAttributeValue(DString attributeValue)
	{
		bool validFirst = true;
		Ogre::String curAttribute = m_pObjFileEditorPanel->m_curAttibuteName;
		if (curAttribute == LogicModel::SCALE_FACTOR ||
			curAttribute == LogicModel::BOUNDING_BOX)
		{
			Ogre::StringVector values = Ogre::StringUtil::split(attributeValue, " ");
			if (values.size() != 3)
			{
				validFirst = false;
			}
		}
		else if(curAttribute == LogicModel::MOVEMENT_TYPE_ATTRIBUTE)
		{
			if ( attributeValue != "Normal"  && attributeValue == "RotateX"  && attributeValue == "RotateXZ" )
			{
				validFirst = false;
			}
		}
		else if(curAttribute == LogicModel::DEFAULT_ANIM_NAME )
		{
		}		
		else
		{
			validFirst = false;
		}

		if (validFirst)
		{
			m_pObjTemplate->changeAttribute(curAttribute, attributeValue);
			Fairy::LogicModel::AttribMap::iterator attributeIter = m_pObjTemplate->mAttribMap.find(curAttribute);

			if(attributeIter != m_pObjTemplate->mAttribMap.end())
			{
				Fairy::LogicModel::AttribValue& tempValue = attributeIter->second;
				tempValue.mAttribValue = attributeValue;
			}

			m_bObjModified = true;
			return true;
		}
		
		return false;
	}

	void CDataManipulator::UpdateEntityInfo(DString entityName)
	{
		if (m_pObjFileEditorPanel && m_pObjTemplate)
		{
			////attributes name
			Fairy::LogicModel::EntityMap::iterator entityIter = m_pObjTemplate->mEntityMap.find(entityName);

			if(entityIter != m_pObjTemplate->mEntityMap.end())
			{
				m_pObjFileEditorPanel->m_entityNameCombo->SetValue(entityName);
				m_pObjFileEditorPanel->m_entityFileCombo->SetValue(entityIter->second.mMeshName.c_str());	
				Ogre::String materialFullName = entityIter->second.mMaterialName;
				m_pObjFileEditorPanel->m_entityMaterialCombo->SetValue(materialFullName.c_str());	
			}			
		}
	}

	void CDataManipulator::UpdateMaterialInfo(DString materialName)
	{
		if (m_pObjFileEditorPanel && m_pObjTemplate)
		{	
			////attributes name
			Fairy::LogicModel::MaterialMap::iterator materialIter = m_pObjTemplate->mMaterialMap.find(materialName);

			if(materialIter != m_pObjTemplate->mMaterialMap.end())
			{
				m_pObjFileEditorPanel->m_materialNameCombo->SetValue(materialName);
				m_pObjFileEditorPanel->m_materialEntityCombo->SetValue(materialIter->second.mEntityEntry.c_str());	
				Ogre::String materialFullName = materialIter->second.mMaterialName;
				m_pObjFileEditorPanel->m_materialCombo->SetValue(materialFullName.c_str());	
			}
		}
	}

	DString CDataManipulator::_formatValidLocatorName()
	{
		int index = 0;
		DString locatorName = PREFIX_NEW_LOCATOR+Ogre::StringConverter::toString(index);
		while (m_pObjTemplate->mLocatorMap.find(locatorName) != m_pObjTemplate->mLocatorMap.end())
		{
			++index;
			locatorName = PREFIX_NEW_LOCATOR+Ogre::StringConverter::toString(index);
		}
		return locatorName;
	}

	DString CDataManipulator::_formatValidEffectName()
	{
		int index = 0;
		DString effectName = PREFIX_NEW_EFFECT+Ogre::StringConverter::toString(index);
		while (m_pObjTemplate->mEffectMap.find(effectName) != m_pObjTemplate->mEffectMap.end())
		{
			++index;
			effectName = PREFIX_NEW_EFFECT+Ogre::StringConverter::toString(index);
		}
		return effectName;
	}

	void CDataManipulator::UpdateLocatorInfo(DString locatorName)
	{
		if (m_pObjTemplate)
		{
			m_pObjTemplate->_updateLocatorPos();
			m_pObjTemplate->_updateAllEffects();
		}		
		
		m_pObjFileEditorPanel->m_posText->Clear();
 		m_pObjFileEditorPanel->m_rotText->Clear();
 
 		Fairy::LogicModel::LocatorMap::iterator locatorIter= m_pObjTemplate->mLocatorMap.find(locatorName);
 
 		DString slotName;

		m_baseNode->setPosition(Ogre::Vector3::ZERO);
		m_baseNode->setOrientation(Ogre::Quaternion::IDENTITY);

		m_axex->setPosition( Ogre::Vector3::ZERO );
		m_axex->setOrientation( Ogre::Quaternion::IDENTITY );	

 		if(locatorIter != m_pObjTemplate->mLocatorMap.end())
 		{
 			
 			Ogre::Vector3 pos	 = locatorIter->second.mOffsetPos;
 			Ogre::Quaternion rot = locatorIter->second.mOffsetOrientation; 

			bool boneValid = false;

			if (m_pObjTemplate->mSkeletonEntity)
			{
				m_pObjTemplate->mSkeletonEntity->detachObjectFromBone(m_IndicatorEntity);

				if (m_curBoneList.find(locatorIter->second.mBoneName)!=m_curBoneList.end())
				{
					Ogre::Bone* bone = m_pObjTemplate->mSkeleton->getBone(locatorIter->second.mBoneName); 				
  					if (bone)
					{
						boneValid = true;

						m_pObjTemplate->mSkeletonEntity->attachObjectToBone(locatorIter->second.mBoneName, m_IndicatorEntity);
						//wangling-<<
  						m_baseNode->setPosition(bone->_getDerivedPosition());
  						m_baseNode->setOrientation(bone->_getDerivedOrientation());
						//wangling->>

						m_pObjFileEditorPanel->m_boneNameCombo->SetValue(locatorIter->second.mBoneName.c_str());
						m_pObjFileEditorPanel->m_slotNameCombo->Enable(true);
						//m_pObjFileEditorPanel->m_attributeEffectCombo->Enable(true);
  					}
				}
			}

			if (!boneValid)
			{
				// Disable the slot info
				m_pObjFileEditorPanel->m_boneNameCombo->SetValue(wxEmptyString);
				m_pObjFileEditorPanel->m_slotNameCombo->Enable(false);
				m_pObjFileEditorPanel->m_attributeEffectCombo->Enable(false);
			}

			if (locatorIter->second.mTranslateFirst)
			{				
				m_baseNode->translate(pos,Ogre::Node::TS_LOCAL);
				//m_axex->setOrientation( rot );	
				m_baseNode->rotate( rot ,Ogre::Node::TS_LOCAL);
			}
			else
			{
				//m_axex->setOrientation( rot );	
				m_baseNode->rotate( rot ,Ogre::Node::TS_LOCAL);
				m_baseNode->translate(pos,Ogre::Node::TS_LOCAL);	
			}			

 			m_pObjFileEditorPanel->m_posText->SetValue(Ogre::StringConverter::toString(pos)); 
 			m_pObjFileEditorPanel->m_rotText->SetValue(Ogre::StringConverter::toString(rot));

  			Fairy::LogicModel::SlotMap::iterator slotIter = locatorIter->second.mSlotMap.begin();
   			if (slotIter != locatorIter->second.mSlotMap.end())
   			{
   				slotName = slotIter->first;
   			}
  		}
 
 		m_pObjFileEditorPanel->m_slotNameCombo->SetValue(slotName);
 		UpdateSlotInfo(slotName, locatorName);
	}

	void CDataManipulator::UpdateEffectInfo(DString effectName)
	{
		Fairy::LogicModel::EffectMap::iterator effectIter = m_pObjTemplate->mEffectMap.find(effectName);

		if(effectIter != m_pObjTemplate->mEffectMap.end())
		{
			// effect file
			Ogre::String fileName = effectIter->second.mEffectName;
			if (m_pObjFileEditorPanel->m_effectFileCombo->FindString(fileName) == -1)
			{
				wxLogMessage(wxT("特效<%s>对应的文件<%s>不存在!"),effectName.c_str(),fileName.c_str());
				m_pObjFileEditorPanel->m_effectFileCombo->SetValue(wxEmptyString);
			}
			else
			{
				m_pObjFileEditorPanel->m_effectFileCombo->SetValue(fileName.c_str());
			}

			// effect locator
			Ogre::String locatorName = effectIter->second.mLocator;
			if (m_pObjFileEditorPanel->m_effectLocatorCombo->FindString(locatorName) == -1)
			{
				wxLogMessage(wxT("特效<%s>对应的挂点<%s>不存在!"),effectName.c_str(),locatorName.c_str());
				m_pObjFileEditorPanel->m_effectLocatorCombo->SetValue(wxEmptyString);
			}
			else
			{
				m_pObjFileEditorPanel->m_effectLocatorCombo->SetValue(locatorName.c_str());
			}

			// effect trans
			m_pObjFileEditorPanel->m_effectTransCheck->SetValue(effectIter->second.mTranslateAll);
			
			// effect colour
			m_pObjFileEditorPanel->m_effectColour->SetValue(Ogre::StringConverter::toString(effectIter->second.mEffectColour).c_str());	
		}
		else
		{
			wxLogMessage(wxT("特效<%s>不存在!"),effectName.c_str());
			// effect file
			m_pObjFileEditorPanel->m_effectFileCombo->SetValue(wxEmptyString);
			// effect locator
			m_pObjFileEditorPanel->m_effectLocatorCombo->SetValue(wxEmptyString);
			// effect trans
			m_pObjFileEditorPanel->m_effectTransCheck->SetValue(false);
			// effect colour
			m_pObjFileEditorPanel->m_effectColour->SetValue(wxEmptyString);	
		}
	}

	bool CDataManipulator::addEffectInfo(const DString& effectName)
	{
		if (!m_pObjTemplate)
		{
			return false;
		}

		if(m_pObjTemplate->addEffectInfo(effectName, "", ""))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool CDataManipulator::deleteCurEffect()
	{
		Ogre::String effectName = m_pObjFileEditorPanel->m_curEffectName;

		if ( effectName.empty()
			|| effectName.compare(NEWEFFECTTIP) == 0
			|| effectName.compare(DELEFFECTTIP) == 0)
		{
			return false;
		}

		Fairy::LogicModel::EffectMap::iterator effectIter = m_pObjTemplate->mEffectMap.find(effectName);
		if(effectIter != m_pObjTemplate->mEffectMap.end())
		{
			// Remove created effect from logicmodel
			Fairy::LogicModel::EffectHandleMap::iterator handleIter = m_pObjTemplate->mEffectHandleMap.find(effectName);
			if(handleIter != m_pObjTemplate->mEffectHandleMap.end())
			{
				m_pObjTemplate->delEffect(handleIter->second);
			}
			// Remove effect info from effectmap
			m_pObjTemplate->mEffectMap.erase(effectName);
			return true;
		}
		else
		{
			return true;
		}
	}

	void CDataManipulator::changeEffectInfo( const DString& name, const DString& effectName, 
												const DString& locatorName, bool translateAll, const Ogre::ColourValue& colour)
	{
		Fairy::LogicModel::EffectMap::iterator effectIter = m_pObjTemplate->mEffectMap.find(name);

		if(effectIter != m_pObjTemplate->mEffectMap.end())
		{
			// Remove created effect from logicmodel
			Fairy::LogicModel::EffectHandleMap::iterator handleIter = m_pObjTemplate->mEffectHandleMap.find(name);
			if(handleIter != m_pObjTemplate->mEffectHandleMap.end())
			{
				m_pObjTemplate->delEffect(handleIter->second);
			}
			// Remove effect info from effectmap
			m_pObjTemplate->mEffectMap.erase(name);
			// Add new effect info into effectmap
			if(m_pObjTemplate->addEffectInfo(name,effectName, locatorName, translateAll, colour))
			{
				// Create new effect handle
				Fairy::LogicModel::EffectHandle handle = m_pObjTemplate->addEffect(effectName, locatorName, translateAll?Fairy::LogicModel::GTIT_ALL:Fairy::LogicModel::GTIT_POSITION, colour);

				if (handle != 0)
				{
					m_pObjTemplate->mEffectHandleMap.insert(Fairy::LogicModel::EffectHandleMap::value_type(name, handle));
				}
				m_pObjTemplate->_updateAllEffects();
				m_pObjTemplate->_updateLocatorPos();				
			}
			else
			{
				wxLogMessage(wxT("添加特效<%s>失败!"),name.c_str());
			}

			m_bObjModified =true;
		}
		else
		{
			wxLogMessage(wxT("特效<%s>不存在!"),name.c_str());
		}
	}

	bool CDataManipulator::changeEffectName( const DString& oldName, const DString& newName)
	{

		Fairy::LogicModel::EffectMap::iterator effectIter = m_pObjTemplate->mEffectMap.find(newName);
		if(effectIter != m_pObjTemplate->mEffectMap.end())
		{
			wxLogMessage(wxT("特效<%s>已存在!"),newName.c_str());
			return false;
		}

		effectIter = m_pObjTemplate->mEffectMap.find(oldName);
		if(effectIter != m_pObjTemplate->mEffectMap.end())
		{
			// Change the effect info
			Fairy::LogicModel::EffectValue tempValue = effectIter->second;
			m_pObjTemplate->mEffectMap.erase(oldName);
			m_pObjTemplate->mEffectMap.insert(Fairy::LogicModel::EffectMap::value_type(newName, tempValue));

			Fairy::LogicModel::EffectHandleMap::iterator handleIter = m_pObjTemplate->mEffectHandleMap.find(oldName);
			if(handleIter != m_pObjTemplate->mEffectHandleMap.end())
			{
				Fairy::LogicModel::EffectHandle handle = handleIter->second;
				m_pObjTemplate->mEffectHandleMap.erase(oldName);
				m_pObjTemplate->mEffectHandleMap.insert(Fairy::LogicModel::EffectHandleMap::value_type(newName, handle));
			}
			m_bObjModified =true;
			return true;
		}
		else
		{
			wxLogMessage(wxT("特效<%s>不存在!"),oldName.c_str());
			return false;
		}		
	}

	void CDataManipulator::_updateAxexInfo()
	{
		if (m_pObjTemplate)
		{
			m_pObjTemplate->_updateLocatorPos();
			m_pObjTemplate->_updateAllEffects();
		}		
		m_baseNode->setPosition(Ogre::Vector3::ZERO);
		m_baseNode->setOrientation(Ogre::Quaternion::IDENTITY);

		Fairy::LogicModel::LocatorMap::iterator locatorIter = m_pObjTemplate->mLocatorMap.find(m_pObjFileEditorPanel->m_curLocatorName);

		if(locatorIter != m_pObjTemplate->mLocatorMap.end())
		{
			if (m_curBoneList.find(locatorIter->second.mBoneName)!=m_curBoneList.end())
			{
				Ogre::Bone* bone = m_pObjTemplate->mSkeleton->getBone(locatorIter->second.mBoneName); 				
				if (bone)
				{
					//wangling-<<
					m_baseNode->setPosition(bone->_getDerivedPosition());
					m_baseNode->setOrientation(bone->_getDerivedOrientation());
					//wangling->>
				}
			}

			Ogre::Vector3 pos	 = locatorIter->second.mOffsetPos;
			Ogre::Quaternion qRot = locatorIter->second.mOffsetOrientation; 
			
			if (locatorIter->second.mTranslateFirst)
			{				
				m_baseNode->translate(pos,Ogre::Node::TS_LOCAL);
				m_baseNode->rotate( qRot ,Ogre::Node::TS_LOCAL);
			}
			else
			{
				m_baseNode->rotate( qRot ,Ogre::Node::TS_LOCAL);
				m_baseNode->translate(pos,Ogre::Node::TS_LOCAL);	
			}
		}		
	}
	
	bool CDataManipulator::ChangeLocatorInfo(DString locator, DString bone, Ogre::Vector3 pos, Ogre::Quaternion rot)
	{
		if ( !m_pObjTemplate)
		{
			return false;
		}

		Fairy::LogicModel::LocatorValue locatorInfo;
		if (m_pObjTemplate->getLocatorInfo(locator, locatorInfo))
		{
			bool bChangeBone = false;
			if (locatorInfo.mBoneName != bone)
			{
				bChangeBone = true;
			}

			locatorInfo.mBoneName = bone;
			locatorInfo.mOffsetPos = pos;
			locatorInfo.mOffsetOrientation = rot;
			if (m_pObjTemplate->setLocatorInfo(locator, locatorInfo))
			{
				// Update the axis pos and rot
				if (bChangeBone)
				{
					_updateAxisByBone(locatorInfo.mBoneName);
				}
			
				m_bObjModified =true;
				return true;
			}			
		}

		return false;
	}

	void CDataManipulator::_updateAxisByBone(const Ogre::String & boneName)
	{
		if (m_pObjTemplate->mSkeletonEntity)
		{
			m_pObjTemplate->mSkeletonEntity->detachObjectFromBone(m_IndicatorEntity);
			if (m_curBoneList.find(boneName)!=m_curBoneList.end())
			{
				Ogre::Bone* bone = m_pObjTemplate->mSkeleton->getBone(boneName);			
				if (bone)
				{
					m_pObjTemplate->mSkeletonEntity->attachObjectToBone(boneName, m_IndicatorEntity);
				}
			}

			_updateAxexInfo();
		}
	}

	void CDataManipulator::UpdateSlotInfo(DString slotName, DString locatorName)
	{
 		if (m_pObjFileEditorPanel && m_pObjTemplate)
 		{
  			if (m_curEffectHandle)
  			{
  				m_pObjTemplate->delEffect(m_curEffectHandle);
 				m_curEffectHandle = 0;
 			}
 
 			m_pObjFileEditorPanel->m_objNameCombo->SetValue("");
 			m_pObjFileEditorPanel->m_attributeEffectCombo->SetValue("");
 
 			////attributes name
 			Fairy::LogicModel::LocatorMap::iterator locatorIter = m_pObjTemplate->mLocatorMap.find(locatorName);
			m_pObjFileEditorPanel->m_objNameCombo->Enable(false);
 
 			if(locatorIter != m_pObjTemplate->mLocatorMap.end())
 			{
 				Fairy::LogicModel::SlotMap::iterator slotIter = locatorIter->second.mSlotMap.find(slotName);
 				if (slotIter != locatorIter->second.mSlotMap.end())
 				{
 					m_pObjFileEditorPanel->m_objNameCombo->SetValue(slotIter->second.mModelName.c_str());
					if (m_pObjFileEditorPanel->m_slotNameCombo->IsEnabled())
					{
						m_pObjFileEditorPanel->m_objNameCombo->Enable(true);
					} 					
 				} 				
 			}
 		}
	}

	bool CDataManipulator::changeEntityMeshAndMat(const Ogre::String &name,const Ogre::String &mesh, const Ogre::String & material)
	{
		if ( !m_pObjTemplate )
		{
			return false;
		}

		LogicModel::EntityMap::iterator i = m_pObjTemplate->mEntityMap.find(name);
		if ( i == m_pObjTemplate->mEntityMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Entity with name '" + name + "' doesn't exists! " +
				"CDataManiputator::changeEntityMeshAndMat " + name );

			return false;
		}

		LogicModel::EntityValue& tempValue = i->second;

		if (tempValue.mMeshName == mesh && tempValue.mMaterialName == material)
		{
			return true;
		}
		else
		{
			if (tempValue.mMeshName != mesh)
			{				
				// Check the skeleton info first
				if (mesh.empty())
				{
					m_pObjTemplate->mEntityMap[name].mMeshName = mesh;
					m_pObjTemplate->changeAttribute(name, mesh);
					if (!isCurObjHasEntity())
					{
						m_pObjTemplate->_delSkeletonEntity();
					}
				}
				else
				{
					Ogre::MeshPtr newMesh = Fairy::loadMesh( mesh, Ogre::StringUtil::BLANK,
						Ogre::StringUtil::BLANK, DEFAULT_RESOURCE_GROUP_NAME);

					//Check if the skeleton of newMesh is the same with the old skeletonEntity
					if (m_pObjTemplate->mSkeletonEntity )
					{
						Ogre::String tempBaseSkeleton = m_pObjTemplate->mSkeletonEntity->getMesh()->getSkeletonName();
						Ogre::String tempNewSkeleton = newMesh->getSkeletonName();
						Ogre::StringVector vec = Ogre::StringUtil::split(tempBaseSkeleton,".");
						tempBaseSkeleton = vec[0];
						vec = Ogre::StringUtil::split(tempNewSkeleton,".");
						tempNewSkeleton =  vec[0];
						if (tempBaseSkeleton.compare(tempNewSkeleton) != 0)
						{
							wxLogMessage(wxT("<%s>的骨骼<%s>与现有骨骼<%s>不匹配!"),mesh.c_str(),tempNewSkeleton.c_str(), tempBaseSkeleton.c_str());
							m_pObjFileEditorPanel->m_entityFileCombo->SetValue(tempValue.mMeshName.c_str());
							return false;
						}
					}

					if (!tempValue.mMeshName.empty())
					{
						Ogre::MeshPtr baseMesh = Fairy::loadMesh( tempValue.mMeshName, Ogre::StringUtil::BLANK,
							Ogre::StringUtil::BLANK, DEFAULT_RESOURCE_GROUP_NAME);
						
						if (baseMesh->hasSkeleton() && newMesh->hasSkeleton())
						{
							if ( !_IsSkeletonMatching(baseMesh->getSkeletonName(), newMesh->getSkeletonName()) )
							{
								m_pObjFileEditorPanel->m_entityFileCombo->SetValue(tempValue.mMeshName.c_str());
								return false;
							}
						}
					}
					else
					{
						// add new mesh
						if (m_pObjTemplate->mlinks && (int)m_pObjTemplate->mlinks->size()>0 && newMesh->hasSkeleton())
						{
							LogicModelManager::AnimationSkeletonLinks::iterator firstIter = m_pObjTemplate->mlinks->begin();
							Ogre::String oldSkeleton = firstIter->second;

							if ( !_IsSkeletonMatching(oldSkeleton, newMesh->getSkeletonName()) )
							{
								m_pObjFileEditorPanel->m_entityFileCombo->SetValue(wxT(""));
								return false;
							}
						}						
					}

					m_pObjTemplate->mEntityMap[name].mMeshName = mesh;
					m_pObjTemplate->changeAttribute(name, mesh);

					if (newMesh->hasSkeleton())
					{
						if(_addSkeleton( newMesh->getSkeletonName() , false ))
						{
							bool valid = true;
							for (Fairy::DStringList::iterator it = m_skeltonFileList.begin(); it != m_skeltonFileList.end(); ++it)
							{
								if (newMesh->getSkeletonName() == *it)
								{
									valid = false;
									break;
								}
							}
							if (valid)
							{
								m_pObjFileEditorPanel->m_skeletonCombo->AppendString(newMesh->getSkeletonName());
								m_skeltonFileList.push_back(newMesh->getSkeletonName());
								if (m_skeltonFileList.size() == 1)
								{
									// Check the bone list to show the locator list
									bool showFlag = _updateBoneList();
									m_pObjFileEditorPanel->SwitchUI(IFS_SLOTS_SIZER,showFlag);
								}
							}							
						}
					}
				}

				// When some mesh changed, update the locator info
				UpdateLocatorInfo(m_pObjFileEditorPanel->m_curLocatorName);
			}
			else if (tempValue.mMaterialName != material)
			{	
				m_pObjTemplate->mEntityMap[name].mMaterialName = material;
				m_pObjTemplate->changeAttribute(name, mesh);

				Ogre::String validMaterialName = material;
				if (validMaterialName.empty())
				{
					// Find the material from mesh file
					validMaterialName = _findMaterialFromEntity(name);					
				}	

				Ogre::Entity* entity = m_pObjTemplate->mEntityMap[name].mEntity;
				if (entity)
				{
					m_pObjTemplate->_setEntityMaterial(name,validMaterialName);
				}
			}
		}
		m_bObjModified =true;
		return true;
	}

	DString CDataManipulator::_findMaterialFromEntity(const DString & entityName)
	{
		if ( !m_pObjTemplate )
		{
			return "";
		}

		if ( entityName.empty() )
		{
			return "";
		}
		else
		{
			Fairy::LogicModel::EntityMap::iterator itEntity = m_pObjTemplate->mEntityMap.find(entityName);

			if ( itEntity == m_pObjTemplate->mEntityMap.end() )
			{
				Ogre::LogManager::getSingleton().logMessage( "Logic Model Entity with name '" + entityName + "' dosen't exists! " +
					"CDataManipulator::_findMaterialFromEntity " + m_pObjTemplate->getName() );

				return "";
			}

			Fairy::LogicModel::EntityValue& tempEntityValue = itEntity->second;

			if (tempEntityValue.mMeshName.empty())
			{
				return "";
			}

			Ogre::MeshPtr entityMesh = Fairy::loadMesh( tempEntityValue.mMeshName, Ogre::StringUtil::BLANK,
				Ogre::StringUtil::BLANK, DEFAULT_RESOURCE_GROUP_NAME);
 			for( Ogre::uint i=0; i<entityMesh->getNumSubMeshes(); ++i )
 			{
				Ogre::SubMesh* subMesh = entityMesh->getSubMesh(i); 
 				if ( subMesh )
 				{
 					return subMesh->getMaterialName();
 				}
 			}
			return "";
		}	

	}

	bool CDataManipulator::changeMaterialEntiryAndMat(const Ogre::String &name,const Ogre::String &entity, const Ogre::String & material)
	{
		if ( !m_pObjTemplate )
		{
			return false;
		}

		LogicModel::MaterialMap::iterator i = m_pObjTemplate->mMaterialMap.find(name);

		if ( i == m_pObjTemplate->mMaterialMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Material with name '" + name + "' doesn't exists! " +
				"CDataManipulator::changeMaterialEntiryAndMat " + name );

			return false;
		}

		LogicModel::MaterialValue& tempValue = i->second;

		if (tempValue.mEntityEntry == entity && tempValue.mMaterialName == material)
		{
			return true;
		}
		else
		{
			if (tempValue.mEntityEntry != entity)
			{
				m_pObjTemplate->mMaterialMap[name].mEntityEntry = entity;
				m_pObjTemplate->changeAttribute(name, material);
			}

			if (tempValue.mMaterialName != material)
			{
				m_pObjTemplate->mMaterialMap[name].mMaterialName = material;
				
				Fairy::LogicModel::EntityMap::iterator itEntity = m_pObjTemplate->mEntityMap.find(m_pObjTemplate->mMaterialMap[name].mEntityEntry);

				if ( itEntity != m_pObjTemplate->mEntityMap.end() )
				{
					Fairy::LogicModel::EntityValue& tempEntityValue = itEntity->second;
					Ogre::Entity* entity = tempEntityValue.mEntity;
					if (entity)
					{
						Ogre::String validMaterialName = material;
						if (validMaterialName.empty())
						{
							// Find the material from mesh file
							validMaterialName = _findMaterialFromEntity(m_pObjTemplate->mMaterialMap[name].mEntityEntry);					
						}
						m_pObjTemplate->changeAttribute(name, validMaterialName);
					}					
				}
			}
		}	
		m_bObjModified =true;
		return true;
	}

	DString CDataManipulator::isSelSlotUsed(const DString& locatorName, const DString& slotName)
	{
		if ( !m_pObjTemplate )
		{
			return "";
		}

		if (slotName.empty())
		{
			return "";
		}

		// Check if the slot is used
		LogicModel::LocatorMap::iterator locatorIter = m_pObjTemplate->mLocatorMap.begin();
		while (locatorIter != m_pObjTemplate->mLocatorMap.end())
		{
			if (locatorIter->first.compare(locatorName) == 0)
			{
				++locatorIter;
				continue;
			}

			LogicModel::SlotMap::iterator slotIter = locatorIter->second.mSlotMap.begin();
			while ( slotIter != locatorIter->second.mSlotMap.end() )
			{
				if (slotIter->first.compare(slotName) == 0)
				{
					DString locatorUse = locatorIter->first;
					return locatorUse;
				}	
				++slotIter;
			}
			++locatorIter;
		}

		return "";
	}

	bool CDataManipulator::deleteCurLocator()
	{
		if (!m_pObjTemplate)
		{
			return false;
		}

		Ogre::String locator = m_pObjFileEditorPanel->m_curLocatorName;

		if ( locator.empty()
			|| locator.compare(NEWLOCATORTIP) == 0
			|| locator.compare(DELLOCATORTIP) == 0)
		{
			return false;
		}

		changeLocatorSlot(locator, "","","");
		LogicModel::LocatorValue locatorValue;
		m_pObjTemplate->getLocatorInfo(locator,locatorValue);
		if (locatorValue.mLocatorNode)
		{
			// Delete locator node from scene
			if (m_pObjTemplate->mModelMainNode)
			{
				m_pObjTemplate->mModelMainNode->removeChild(locatorValue.mLocatorNode->getName());
				m_pObjTemplate->mModelMainNode->getCreator()->destroySceneNode(locatorValue.mLocatorNode->getName());
				locatorValue.mLocatorNode = NULL;
				m_pObjTemplate->setLocatorInfo(locator,locatorValue);
			}
		}

		// Delete locator from locator map
		m_pObjTemplate->DelLocator(locator);
		m_bObjModified =true;
		return true;
	}

	bool CDataManipulator::changeLocatorSlot(const Ogre::String & locatorName, const Ogre::String &slotName, const Ogre::String &objName, const Ogre::String &attrib )
	{
		if ( !m_pObjTemplate )
		{
			return false;
		}

		// 假设一个locator只能有一个slotInfo
		// slotName为空，则只删除原有slotInfo		
		LogicModel::LocatorMap::iterator itLocator = m_pObjTemplate->mLocatorMap.find(locatorName);
		if (itLocator == m_pObjTemplate->mLocatorMap.end())
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model locator with name '" + locatorName + "' doesn't exists! " +
				"CDataManipulator::changeLocatorSlot " + locatorName );

			return false;
		}
		else
		{
			LogicModel::LocatorValue& tempLocatorValue = itLocator->second;
			LogicModel::SlotMap::iterator itSlot = tempLocatorValue.mSlotMap.begin();
			if ( itSlot != tempLocatorValue.mSlotMap.end() )
			{
				tempLocatorValue.mSlotMap.erase(itSlot);				
			}

			if (!slotName.empty())
			{
				// Add this slot info to map
				LogicModel::SlotValue slotValue(objName);
				tempLocatorValue.mSlotMap.insert(LogicModel::SlotMap::value_type(slotName, slotValue));
			}

			if (!objName.empty())
			{
				if (!LogicModelManager::getSingleton().getTemplateByName(objName))
				{
					Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(objName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
					LogicModelManager::getSingleton().parseScript(stream,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
				}			
			}

			m_pObjTemplate->changeAttribute(slotName, objName);
			m_pObjTemplate->_updateLocatorPos();

			m_bObjModified =true;
			return true;
		}
	}

	bool CDataManipulator::changeLocatorName(const DString & oldLocatorName, const DString& newLocatorName)
	{
		if (!m_pObjTemplate ||newLocatorName.empty())
		{
			return false;
		}

		if (oldLocatorName == newLocatorName)
		{
			return true;
		}

		if (m_pObjTemplate->mLocatorMap.find(newLocatorName) != m_pObjTemplate->mLocatorMap.end())
		{
			return false;
		}

		Fairy::LogicModel::LocatorMap::iterator locatorIter= m_pObjTemplate->mLocatorMap.find(oldLocatorName);

		if(locatorIter != m_pObjTemplate->mLocatorMap.end())
		{
			LogicModel::LocatorValue tempValue = locatorIter->second;
			m_pObjTemplate->mLocatorMap.erase(locatorIter);
			m_pObjTemplate->mLocatorMap.insert(LogicModel::LocatorMap::value_type(newLocatorName, tempValue));

			// change the effect locator
			Fairy::LogicModel::EffectMap::iterator effectIter = m_pObjTemplate->mEffectMap.begin();

			for (;effectIter !=m_pObjTemplate->mEffectMap.end();++effectIter)
			{
				Fairy::LogicModel::EffectValue& tempValue = effectIter->second;
				if (tempValue.mLocator == oldLocatorName)
				{
					tempValue.mLocator = newLocatorName;
				}				
			}

			for (Fairy::LogicModel::CreatedEffectList::iterator iter = m_pObjTemplate->mCreatedEffectList.begin();
					iter != m_pObjTemplate->mCreatedEffectList.end(); ++iter)
			{
				Fairy::LogicModel::CreatedEffectInfo& effectInfo = *iter;
				if (effectInfo.mLocator == oldLocatorName)
				{
					effectInfo.mLocator == newLocatorName;
				}
			}

			m_bObjModified =true;
			return true;
		}
		else
		{
			return false;
		}	
	}

	int CDataManipulator::changeAttributeName(const DString & oldAttributeName, const DString& newAttributeName)
	{
		if (!m_pObjTemplate ||newAttributeName.empty())
		{
			return -1;
		}

		if (oldAttributeName == LogicModel::SCALE_FACTOR ||
			oldAttributeName == LogicModel::MOVEMENT_TYPE_ATTRIBUTE ||
			oldAttributeName == LogicModel::DEFAULT_ANIM_NAME ||
			oldAttributeName == LogicModel::BOUNDING_BOX)
		{
			wxLogMessage(wxT("属性<%s>为固定名字，不能改名。"),oldAttributeName.c_str());
			return -1;
		}

		if (oldAttributeName == newAttributeName)
		{
			return 0;
		}

		LogicModel::AttribMap::iterator attriIter= m_pObjTemplate->mAttribMap.find(oldAttributeName);

		if(attriIter != m_pObjTemplate->mAttribMap.end())
		{
			LogicModel::AttribValue tempValue = attriIter->second;
			if (!tempValue.mAttribValue.empty())
			{
				wxLogMessage(wxT("属性<%s>对应的当前值不为空，不能改名。"),oldAttributeName.c_str());
				return -1;
			}
			m_pObjTemplate->mAttribMap.erase(attriIter);
			m_pObjTemplate->mAttribMap.insert(LogicModel::AttribMap::value_type(newAttributeName, tempValue));

			if (tempValue.mHint.compare(LogicModel::MESH_HINT) == 0)
			{
				// change the entity list
				LogicModel::EntityMap::iterator entityIter= m_pObjTemplate->mEntityMap.find(oldAttributeName);
				if ( entityIter != m_pObjTemplate->mEntityMap.end() )
				{
					LogicModel::EntityValue entity = entityIter->second;
					m_pObjTemplate->mEntityMap.erase(entityIter);
					m_pObjTemplate->mEntityMap.insert(LogicModel::EntityMap::value_type(newAttributeName, entity));
				}

				LogicModel::MaterialMap::iterator materialIter= m_pObjTemplate->mMaterialMap.begin();
				while ( materialIter != m_pObjTemplate->mMaterialMap.end() )
				{
					LogicModel::MaterialValue& material = materialIter->second;
					if (material.mEntityEntry == oldAttributeName)
					{
						material.mEntityEntry = newAttributeName;
						break;
					}				
				}
				m_bObjModified =true;

				return HT_MESH;
			}
			else if (tempValue.mHint.compare(LogicModel::MATERIAL_HINT) == 0)
			{
				// change the material list
				LogicModel::MaterialMap::iterator materialIter= m_pObjTemplate->mMaterialMap.find(oldAttributeName);
				if ( materialIter != m_pObjTemplate->mMaterialMap.end() )
				{
					LogicModel::MaterialValue material = materialIter->second;
					m_pObjTemplate->mMaterialMap.erase(materialIter);
					m_pObjTemplate->mMaterialMap.insert(LogicModel::MaterialMap::value_type(newAttributeName, material));
				}
				m_bObjModified =true;

				return HT_MATERIAL;
			}
			else
			{
				// change the slot list
				Fairy::LogicModel::LocatorMap::iterator locatorIter= m_pObjTemplate->mLocatorMap.begin();

				DString slotName;

				while(locatorIter != m_pObjTemplate->mLocatorMap.end())
				{
					Fairy::LogicModel::SlotMap& slotMap = locatorIter->second.mSlotMap;
					Fairy::LogicModel::SlotMap::iterator slotIter = slotMap.find(oldAttributeName);
					if (slotIter != slotMap.end())
					{
						LogicModel::SlotValue slot = slotIter->second;
						slotMap.erase(slotIter);
						slotMap.insert(LogicModel::SlotMap::value_type(newAttributeName, slot));
					}
					++locatorIter;
				}
				m_bObjModified =true;

				return HT_SLOT;
			}			
		}
		else
		{
			return -1;
		}		
	}

	bool CDataManipulator::updateUIAfterByAttributeName(const DString & oldAttributeName, const DString& newAttributeName, int hint)
	{
		switch (hint)
		{
		case HT_MESH:
			{
				// Mesh
				int idx = m_pObjFileEditorPanel->m_entityNameCombo->FindString(oldAttributeName);
				m_pObjFileEditorPanel->m_entityNameCombo->SetString(idx, newAttributeName);

				idx = m_pObjFileEditorPanel->m_materialEntityCombo->FindString(oldAttributeName);
				m_pObjFileEditorPanel->m_materialEntityCombo->SetString(idx, newAttributeName);
				
				UpdateEntityInfo(newAttributeName);

				Ogre::String  materialName = m_pObjFileEditorPanel->m_materialNameCombo->GetStringSelection().c_str();
				if(materialName.empty())
				{
					materialName = m_pObjFileEditorPanel->m_materialNameCombo->GetValue().c_str();
				}
				UpdateMaterialInfo(materialName);

				return true;
			}
			break;
		case HT_MATERIAL:
			{
				// Material
				int idx = m_pObjFileEditorPanel->m_materialNameCombo->FindString(oldAttributeName);
				m_pObjFileEditorPanel->m_materialNameCombo->SetString(idx, newAttributeName);

				UpdateMaterialInfo(newAttributeName);

				return true;
			}
			break;
		case HT_SLOT:
			{
				// slot
				int idx = m_pObjFileEditorPanel->m_slotNameCombo->FindString(oldAttributeName);
				m_pObjFileEditorPanel->m_slotNameCombo->SetString(idx, newAttributeName);

				UpdateLocatorInfo(m_pObjFileEditorPanel->m_curLocatorName);

				return true;
			}
			break;
		default:
			{
				return false;
			}
			break;
		}
	}
	bool CDataManipulator::switchSlot(const Ogre::String & oldLocatorName, const Ogre::String & newLocatorName, const Ogre::String &slotName)
	{
		//delete slot from old locator
		if(changeLocatorSlot(oldLocatorName, "", "", ""))
		{
			//change the slot of the new locator
			if (changeLocatorSlot(newLocatorName,slotName,"",""))
			{
				return true;
			}
		}
		return false;
	}

	void CDataManipulator::changeSlotsEffect(const DString& effect, const DString& locatorName)
	{
		if (m_curEffectHandle)
		{
			m_pObjTemplate->delEffect(m_curEffectHandle);
			m_curEffectHandle = 0;
		}
		if (effect.empty())
		{
			return;
		}

		Fairy::LogicModel::LocatorMap::iterator locatorIter = m_pObjTemplate->mLocatorMap.find(locatorName);

		if(locatorIter != m_pObjTemplate->mLocatorMap.end())
		{
			if (m_curBoneList.find(locatorIter->second.mBoneName)!=m_curBoneList.end())
			{
				Ogre::Bone* bone = m_pObjTemplate->mSkeleton->getBone(locatorIter->second.mBoneName); 				
				if (bone)
				{
					m_pObjTemplate->_getLocatorSceneNode(locatorIter->second);
					Ogre::StringVector vec = Ogre::StringUtil::split(effect,".");

					// TODO:Check if the effect exist
					m_curEffectHandle = m_pObjTemplate->addEffect(vec[0], locatorName);
					m_pObjTemplate->_updateLocatorPos();
					m_pObjTemplate->_updateAllEffects();
				}
			}
		}
	}

	void CDataManipulator::_updateCurLocatorRot(Ogre::Quaternion rot)
	{
		if ( !m_pObjTemplate || !m_pObjFileEditorPanel)
		{
			return;
		}

		Ogre::String  locatorName = m_pObjFileEditorPanel->m_curLocatorName;
		if(locatorName.empty())
		{
			return;
		}

		Ogre::String  boneName = m_pObjFileEditorPanel->m_boneNameCombo->GetStringSelection().c_str();
		if(boneName.empty())
		{
			boneName = m_pObjFileEditorPanel->m_boneNameCombo->GetValue().c_str();
		}

		if (boneName.empty() || !m_pObjTemplate->mSkeletonEntity)
		{
			return;
		}

		Fairy::LogicModel::LocatorValue locatorInfo;
		if (!m_pObjTemplate->getLocatorInfo(locatorName, locatorInfo))
		{
			return;
		}

		if (m_pObjTemplate->mSkeletonEntity)
		{
			Ogre::Bone* bone = m_pObjTemplate->mSkeletonEntity->getSkeleton()->getBone(boneName);
			if (bone)
			{
				m_baseNode->rotate(rot);
				Ogre::Vector3 finalPos = m_baseNode->getPosition();
				Ogre::Quaternion finalRot = m_baseNode->getOrientation();
				//wangling-<<
				Ogre::Vector3 bonePos = bone->_getDerivedPosition();
				Ogre::Quaternion boneRot = bone->_getDerivedOrientation();
				

				Ogre::Quaternion deltaRot = boneRot.Inverse()*finalRot;
				Ogre::Vector3 deltaPos = finalRot.Inverse()*(finalPos - bonePos);
				
				if(GetDataManipulator()->ChangeLocatorInfo(locatorName, boneName, deltaPos, deltaRot))
				{
					// Update the ui
					_updateAxexInfo();
					m_pObjFileEditorPanel->m_posText->SetValue(Ogre::StringConverter::toString(deltaPos)); 
					m_pObjFileEditorPanel->m_rotText->SetValue(Ogre::StringConverter::toString(deltaRot));
				}
				//wangling->>
			}
		}

			
	}

	void CDataManipulator::_updateCurLocatorTrans(Ogre::Vector3 deltaPos, Ogre::Quaternion deltaRot, bool trans)
	{
		if ( !m_pObjTemplate || !m_pObjFileEditorPanel)
		{
			return;
		}

		Ogre::String  locatorName = m_pObjFileEditorPanel->m_curLocatorName;
		if(locatorName.empty())
		{
			return;
		}

		Ogre::String  boneName = m_pObjFileEditorPanel->m_boneNameCombo->GetStringSelection().c_str();
		if(boneName.empty())
		{
			boneName = m_pObjFileEditorPanel->m_boneNameCombo->GetValue().c_str();
		}

		Fairy::LogicModel::LocatorValue locatorInfo;
		if (!m_pObjTemplate->getLocatorInfo(locatorName, locatorInfo))
		{
			return;
		}

		if (trans)
		{
			// Add deltaPos to offset
			locatorInfo.mOffsetPos +=deltaPos;
		}
		else 
		{
			// Add deltaRot to offsetOrient
			locatorInfo.mOffsetOrientation  = locatorInfo.mOffsetOrientation*deltaRot;
		}

		if(GetDataManipulator()->ChangeLocatorInfo(locatorName, boneName, locatorInfo.mOffsetPos, locatorInfo.mOffsetOrientation))
		{
			// Update the ui
			_updateAxexInfo();
			m_pObjFileEditorPanel->m_posText->SetValue(Ogre::StringConverter::toString(locatorInfo.mOffsetPos)); 
			m_pObjFileEditorPanel->m_rotText->SetValue(Ogre::StringConverter::toString(locatorInfo.mOffsetOrientation));
		}	
	}

	LogicModelManager::AnimationSkeletonLinks * CDataManipulator::_getCurAniLinks()
	{
		if (!m_pObjTemplate)
		{
			return NULL;
		}

		if (!m_pObjTemplate->mlinks)
		{
			return NULL;
		}

		return m_pObjTemplate->mlinks;
	}

	bool CDataManipulator::changeMeshAnimation(const Ogre::String& aniName)
	{
		if (!m_pMeshObject)
		{
			return false;
		}

		Fairy::StaticEntityObject* entity = static_cast<Fairy::StaticEntityObject*>((m_pMeshObject).get());
		Ogre::Entity* pCurEntity = entity->getEntity();
		if (pCurEntity)
		{
			mAnimState = pCurEntity->getAnimationState(aniName);
			if(mAnimParam->animState)
			{
				mAnimParam->animState->setWeight(0);
			}

			mAnimState->setEnabled(true);
			mAnimState->setLoop(true);
			mAnimState->setTimePosition(0.0f);
			mAnimState->setWeight(1);

			mAnimParam->animState = mAnimState;
			mAnimParam->stateEnabled=true;
		}

		return true;
	}

	bool CDataManipulator::changeAnimation(const Ogre::String& aniName, Ogre::Real delay)
	{
		LogicModel* pObjModel = m_pObjTemplate;
		if (NULL == pObjModel)
		{
			return false;
		}

		if (pObjModel->mFadeOutAnimationState)
		{
			pObjModel->mFadeOutAnimationState->setWeight(0);
			pObjModel->mFadeOutAnimationState->setEnabled(false);
		}
		
		pObjModel->mFadeOutAnimationState = pObjModel->mCurrentAnimationState;
		//pObjModel->mFadeOutDelay = delay;
		pObjModel->mFadeOutDelay = m_pAnimationSelector->m_curFuseValue;
		
		if(aniName.empty())
		{
			pObjModel->mCurrentAnimationState = NULL;
			pObjModel->mCurrentAnimationName = "";
		}
		else
		{
			try{

				// 有可能因为资源的原因，缺少所需的skeleton文件，所以这个entity为空
				if (pObjModel->mSkeletonEntity)
				{
					pObjModel->mCurrentAnimationState = pObjModel->_getAnimationState(aniName);

					pObjModel->mSkeleton->getAnimation(aniName)->setInterpolationMode( 
						LogicModelManager::getSingleton().getAnimationInterpolationMode() );
				}
				else
				{
					Ogre::LogManager::getSingleton().logMessage( "Logic Model : " + pObjModel->getName() + 
						" Skeleton Entity is NULL, Maybe the skeleton file is lost!" +
						"LogicModel::_changeAnimation");
				}

			}
			catch (const Ogre::Exception& e)
			{
				Ogre::LogManager::getSingleton().logMessage("Model : " + pObjModel->getName() + '\n' + "_getAnimationState Failed!");
				pObjModel->mCurrentAnimationState = NULL;

				if (pObjModel->mSystem->getThrowExceptionIfResourceIncorrect())//添加捕捉完成（动作名不存在）
				{
					OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
						"Resource was incorrectness due incaution producer. "
						"Full error description: " + e.getFullDescription(),
						"LogicModel::_changeAnimation");
				}
			}

			if(pObjModel->mCurrentAnimationState)
			{
				if (pObjModel->mFadeOutAnimationState == pObjModel->mCurrentAnimationState)
				{
					pObjModel->mFadeOutAnimationState = NULL;
				}

				pObjModel->mCurrentAnimationState->setEnabled(true);
				pObjModel->setAnimationLoop(true);
				pObjModel->mCurrentAnimationState->setTimePosition(0.0f);

				if (pObjModel->mFadeOutAnimationState)
				{
					pObjModel->mCurrentAnimationState->setWeight(0);
					pObjModel->mFadeOutAnimationState->setWeight(1);
					pObjModel->mFadeOutAnimationState->setEnabled(true);
				}
				else
				{
					pObjModel->mCurrentAnimationState->setWeight(1);
				}

				pObjModel->mCurrentAnimationName = aniName;

				return true;
			}
			else
			{
				pObjModel->mCurrentAnimationName = "";
				return false;
			}
		}
		
		return true;
	}

	void CDataManipulator::updateAnimationInfo()
	{
		if (m_pAnimationSelector && m_pAnimationSelector->mIsMeshAnim)
		{
			return;
		}

		if (mBoxIndicator && m_pObjTemplate && mShowWorldBoundingBox)
		{
			Ogre::Vector3 additionPos = Ogre::Vector3::ZERO;
			Ogre::Matrix4 addTrans = Ogre::Matrix4::IDENTITY;
			/*if (m_pObjTemplate->mSkeletonEntity)
			{
				Ogre::Bone* rootBone = m_pObjTemplate->mSkeleton->getRootBone();
				if (rootBone)
				{
					additionPos = rootBone->getPosition()-rootBone->getInitialPosition();
					addTrans.setTrans(additionPos);
				}
			}*/
			Ogre::AxisAlignedBox aabb = m_pObjTemplate->getBoundingBox();
			aabb.transformAffine(m_pObjTemplate->getSceneNode()->_getFullTransform()*addTrans);
			mBoxIndicator->setupBoundingBox(aabb);
		}

		if ( !m_pAnimationSelector || !m_pAnimationSelector->hasSelectedItem())
		{
			return;
		}

		if (m_pAnimationSelector->getPaused())
		{
			return;
		}

		LogicModel* pObjModel = m_pObjTemplate;

		if ( !pObjModel || pObjModel->getCurrentAnimName().empty())
		{
			return;
		}
		else
		{
			Ogre::String animationName = pObjModel->getCurrentAnimName();
			Ogre::Real timePos = pObjModel->mCurrentAnimationState->getTimePosition();
			m_pAnimationSelector->m_curTimeText->SetValue(Ogre::StringConverter::toString(timePos));
			m_pAnimationSelector->m_curFrameText->SetValue(Ogre::StringConverter::toString(timePos*STATIC_FPS));

			int curPos = timePos*100/pObjModel->mCurrentAnimationState->getLength();
			m_pAnimationSelector->m_slider->SetValue(curPos);

			// Update current axis info
			if (m_pObjFileEditorPanel)
			{
				Ogre::String  boneName = m_pObjFileEditorPanel->m_boneNameCombo->GetStringSelection().c_str();
				if(boneName.empty())
				{
					boneName = m_pObjFileEditorPanel->m_boneNameCombo->GetValue().c_str();
				}
				if (!boneName.empty())
				{
					_updateAxisByBone(boneName);
				}				
			}			
		}

	}

	bool CDataManipulator::_IsSkeletonMatching(const Ogre::String & baseSkeleton, const Ogre::String &newSkeleton)
	{
		// Check the bone info
		Ogre::SkeletonPtr pBaseSkeleton = Ogre::SkeletonManager::getSingleton().load(baseSkeleton, "General");
		Ogre::SkeletonPtr pNewSkeleton = Ogre::SkeletonManager::getSingleton().load(newSkeleton, "General");

		if (pBaseSkeleton->getNumBones() != pNewSkeleton->getNumBones())
		{
			wxLogMessage(wxT("骨骼<%s>与原骨骼<%s>不匹配!"),newSkeleton.c_str(),baseSkeleton.c_str());
			return false;
		}
		Ogre::Skeleton::BoneIterator baseIter = pBaseSkeleton->getBoneIterator();
		Ogre::Skeleton::BoneIterator newIter = pNewSkeleton->getBoneIterator();
		while (baseIter.hasMoreElements())
		{
			Ogre::Bone* baseBone = baseIter.getNext();
			bool bFinded = false;

			while (newIter.hasMoreElements())
			{
				Ogre::Bone* newBone = newIter.getNext();
				if (baseBone->getName() == newBone->getName())
				{
					bFinded = true;
					break;
				}
			}

			if( ! bFinded )
			{
				wxLogMessage(wxT("骨骼<%s>与原骨骼<%s>不匹配!"),newSkeleton.c_str(),baseSkeleton.c_str());
				return false;
			}
		}
		return true;
	}

	bool CDataManipulator::isCurObjHasEntity()
	{
		if (!m_pObjTemplate)
		{
			return false;
		}

		bool bHas = false;

		Fairy::LogicModel::EntityMap::iterator entityIter = m_pObjTemplate->mEntityMap.begin();

		while(entityIter != m_pObjTemplate->mEntityMap.end())
		{
			Fairy::LogicModel::EntityValue& tempValue = entityIter->second;
			if (!tempValue.mMeshName.empty())
			{
				bHas = true;
				break;
			}
			++entityIter;
		}

		return bHas;
	}

	bool CDataManipulator::_addSkeleton(const Ogre::String &SkeletonName, bool bCheckSkeMatching )
	{
		if (m_pObjTemplate->mlinks)
		{
			Fairy::LogicModelManager::AnimationSkeletonLinksIterator it =  Fairy::LogicModelManager::AnimationSkeletonLinksIterator(m_pObjTemplate->mlinks->begin(), m_pObjTemplate->mlinks->end());

			while(it.hasMoreElements())
			{
				Ogre::String aniname = it.peekNextValue();

				Ogre::StringVector vec = Ogre::StringUtil::split(aniname,".");
				Ogre::String tempBaseSkeleton = vec[0];
				vec = Ogre::StringUtil::split(SkeletonName,".");
				Ogre::String tempNewSkeleton =  vec[0];
				if (tempBaseSkeleton.compare(tempNewSkeleton) == 0 )
				{
					return false;
				} 

				it.moveNext();
			}	
		}

		if ( bCheckSkeMatching )
		{
			DString baseSkeletonName;
			// Check the skeleton first
			if ( m_pObjTemplate->mlinks && (int)m_pObjTemplate->mlinks->size() >0 )
			{
				LogicModelManager::AnimationSkeletonLinks::iterator firstIter = m_pObjTemplate->mlinks->begin();
				baseSkeletonName = firstIter->second;
			}

			if ( !baseSkeletonName.empty() )
			{
				// Check the bone info
				if (!_IsSkeletonMatching(baseSkeletonName, SkeletonName))
				{
					return false;
				}
			}
			else
			{
				if (m_pObjTemplate->mSkeletonEntity)
				{
					Ogre::String oldBaseSkeleton = m_pObjTemplate->mSkeletonEntity->getSkeleton()->getName();
					if (!_IsSkeletonMatching(oldBaseSkeleton, SkeletonName))
					{
						return false;
					}
				}
			}
		}		

		if (!m_pObjTemplate->mlinks)
		{
			m_pObjTemplate->mlinks = LogicModelManager::getSingleton()._createAnimationSkeletonLinksForModel(m_pObjTemplate->getName());
		}

		Ogre::SkeletonPtr pSkeleton ;
		pSkeleton = Ogre::SkeletonManager::getSingleton().load(SkeletonName, "General");

		//更新动画列表
		int numAnim =  pSkeleton->getNumAnimations();
		for(int i=0;i<numAnim;++i)
		{
			Ogre::String strAnimName = pSkeleton->getAnimation(i)->getName();
			m_pObjTemplate->mlinks->insert( Fairy::LogicModelManager::AnimationSkeletonLinks::value_type( strAnimName, SkeletonName ) );
		}

		UpdateAnimationListUI();
		return true;
	}

	bool CDataManipulator::_delSkeleton(const Ogre::String &SkeletonName)
	{
 		if ( m_pObjTemplate->mSkeletonEntity )
 		{
			Ogre::StringVector vec = Ogre::StringUtil::split(m_pObjTemplate->mSkeletonEntity->getSkeleton()->getName(),".");
			Ogre::String tempBaseSkeleton = vec[0];
			vec = Ogre::StringUtil::split(SkeletonName,".");
			Ogre::String tempNewSkeleton =  vec[0];
			if (tempBaseSkeleton.compare(tempNewSkeleton) == 0 )
			{
				return false;
			} 			
 		}

		LogicModelManager::AnimationSkeletonLinks* links = m_pObjTemplate->mlinks;

		for(std::map<Ogre::String, Ogre::String>::iterator iter = links->begin();  iter != links->end(); )
		{
			if ((*iter).second == SkeletonName)
			{
				std::map<Ogre::String, Ogre::String>::iterator iterToErase = iter;
				++iter;
				links->erase(iterToErase);
			}
			else
			{
				++iter;
			}
		}

		UpdateAnimationListUI();
		return true;
	}

	Ogre::Skeleton::BoneIterator CDataManipulator::_getBone(const Ogre::String &SkeletonName)
	{
		Ogre::SkeletonPtr pSkeleton ;
		pSkeleton = Ogre::SkeletonManager::getSingleton().load(SkeletonName, "General");
		return pSkeleton->getBoneIterator();
	}

	void CDataManipulator::setSaveDefaultSkeleton(bool flag)
	{
		mSaveDefaultSkeleton = flag;
	}
}
