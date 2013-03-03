#include "editdobject_nt.h"

#include <OgreLogManager.h>
#include <OgreStringConverter.h>
#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreSkeletonInstance.h>
#include <OgreBone.h>
#include <OgreTagPoint.h>
#include <OgreTechnique.h>
#include <OgreTextureUnitState.h>
#include <Ogrematerial.h>
#include <OgrematerialManager.h>
#include <OgreSubEntity.h> 

CEditDobject_NT::CEditDobject_NT( Fairy::System *system ) :
DObject_NT(system)
{
}

CEditDobject_NT::~CEditDobject_NT(void)
{
}



// ����ģ�͵�͸����
// 0 -- ��ȫ͸����
// 1 -- ��͸����
void CEditDobject_NT::SetTransparence(float Transparence)
{
	int iCount = m_EntityList.size();
	Ogre::Entity* pEntity = NULL;

	for(int i = 0; i < iCount; i++)
	{
		pEntity = m_EntityList[i].pEntity;
		if(pEntity)
		{
			 Ogre::SubEntity* pSubEntiy = pEntity->getSubEntity(0);
			
			 if(pSubEntiy)
			 {
				Ogre::MaterialPtr material1 = pSubEntiy->getMaterial();
				Ogre::Technique* t1 = material1->getBestTechnique();
				Ogre::Pass* p1 = t1->getPass(0);

				p1->setSceneBlending(Ogre::SBT_ADD  );
				p1->setSceneBlending(Ogre::SBF_SOURCE_ALPHA , Ogre::SBF_ONE_MINUS_SOURCE_ALPHA );
				Ogre::TextureUnitState* pTextureState = p1->getTextureUnitState(0);
				pTextureState->setAlphaOperation(Ogre::LBX_MODULATE, Ogre::LBS_TEXTURE, Ogre::LBS_MANUAL, 1, Transparence, 1);
				 
				//Ogre::ColourValue color(1,0.0,0.0,0.1) ;
				//pTextureState->setColourOperationEx(LBX_ADD , LBS_TEXTURE , LBS_MANUAL, color, color );
				
			 }

		}
	}
}


// ����ģ�͵Ļ����ɫ
void CEditDobject_NT::SetBlendColor(float fRed, float fGreen, float fBlue)
{
	//int iCount = m_EntityList.size();
	//Ogre::Entity* pEntity = NULL;

	//for(int i = 0; i < iCount; i++)
	//{
	//	pEntity = m_EntityList[i].pEntity;
	//	if(pEntity)
	//	{
	//		 Ogre::SubEntity* pSubEntiy = pEntity->getSubEntity(0);
	//		 if(pSubEntiy)
	//		 {
	//			Ogre::MaterialPtr material1 = pSubEntiy->getMaterial();
	//			Ogre::Technique* t1 = material1->getBestTechnique();
	//			Ogre::Pass* p1 = t1->getPass(0);

	//			p1->setSceneBlending(SBT_ADD  );
	//			p1->setSceneBlending(SBF_SOURCE_ALPHA , SBF_ONE_MINUS_SOURCE_ALPHA );
	//			TextureUnitState* pTextureState = p1->getTextureUnitState(0);
	//			pTextureState->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1, Transparence, 1);
	//			 
	//			//Ogre::ColourValue color(1,0.0,0.0,0.1) ;
	//			//pTextureState->setColourOperationEx(LBX_ADD , LBS_TEXTURE , LBS_MANUAL, color, color );
	//			
	//		 }

	//	}
	//}//
}



// ����ѡ�е������ɫ.
void CEditDobject_NT::SetSelectLook(Ogre::ColourValue color)
{

	if(0 == m_materialSelVector.size())
	{
		// ѡ�в��ʵ�����.
		Ogre::String strCloneName;
		int iCount = m_EntityList.size();
		Ogre::Entity* pEntity = NULL;

		for(int i = 0; i < iCount; i++)
		{
			pEntity = m_EntityList[i].pEntity;
			if(pEntity)
			{
				Ogre::SubEntity* pSubEntiy = pEntity->getSubEntity(0);
				if(pSubEntiy)
				{
					
					Ogre::MaterialPtr pMaterial = pSubEntiy->getMaterial();
					
					if(pMaterial.isNull())
					{
						return;
					}//

					const Ogre::String& strName = pMaterial->getName();

					if("BaseWhite" == strName)
					{
						continue;
					}

					strCloneName = strName;
					strCloneName += "_select";
					
					Ogre::MaterialManager* pMaterialManager = (Ogre::MaterialManager*)(pMaterial->getCreator());

					if(NULL == pMaterialManager)
					{
						return;
					}

					Ogre::MaterialPtr pMaterialClone = pMaterialManager->getByName(strCloneName); 
						
					if(pMaterialClone.isNull())
					{
						pMaterialClone = pMaterial->clone(strCloneName);
					}
					//if(!pMaterialClone)
					//{
					//	return;
					//}//

					Ogre::Technique* pTechnique = pMaterialClone->getBestTechnique();
					Ogre::Pass* pPass = pTechnique->getPass(0);

					//pPass->setSceneBlending(SBT_ADD);
					//pPass->setSceneBlending(SBF_SOURCE_ALPHA , SBF_ONE_MINUS_SOURCE_ALPHA );
					//pTextureState->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1, Transparence, 1);//
				
					Ogre::TextureUnitState* pTextureState = pPass->getTextureUnitState(0);
					pTextureState->setColourOperationEx(Ogre::LBX_ADD , Ogre::LBS_TEXTURE , Ogre::LBS_MANUAL, color, color );
					pSubEntiy->setMaterialName(strCloneName);
					m_materialSelVector.push_back(pMaterialClone);
					m_materilaOldVector.push_back(pMaterial);
					
				}

			}
		}
	}
	else
	{
		int iIndex = 0;

		int iCount = m_EntityList.size();
		Ogre::Entity* pEntity = NULL;

		for(int i = 0; i < iCount; i++)
		{
			pEntity = m_EntityList[i].pEntity;
			if(pEntity)
			{
				Ogre::SubEntity* pSubEntiy = pEntity->getSubEntity(0);
				
				if(pSubEntiy)
				{
					if(iIndex >= (int)m_materialSelVector.size())
					{
						continue;
					}

					std::string strMaterialName = m_materialSelVector[iIndex]->getName();
					pSubEntiy->setMaterialName(strMaterialName);	
					iIndex++;
				}

			}
		}
	}
}

// ���������������ɫ.
void CEditDobject_NT::SetUnSelLook()
{
	if(0 == m_materilaOldVector.size())
	{
		return;
	}

	int iCount = m_EntityList.size();
	Ogre::Entity* pEntity = NULL;

	int iIndex = 0;

	for(int i = 0; i < iCount; i++)
	{
		pEntity = m_EntityList[i].pEntity;
		if(pEntity)
		{
			Ogre::SubEntity* pSubEntiy = pEntity->getSubEntity(0);
			if(pSubEntiy)
			{
				if(iIndex >= (int)m_materilaOldVector.size())
				{
					continue;
				}
			
				std::string strMaterialName = m_materilaOldVector[iIndex]->getName();
				pSubEntiy->setMaterialName(strMaterialName);
				iIndex++;
				
			}

		}
	}

}

// ��ղ��� 
void CEditDobject_NT::ClearMaterial()
{
	// object ѡ��ʱʹ�õĲ��ʡ�
	m_materialSelVector.clear();

	// object ԭ���Ĳ���.
	m_materilaOldVector.clear();
}


