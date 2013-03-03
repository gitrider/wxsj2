#include "UnreachRegion.h"
#include "SceneManipulator.h"

#include "Core/FairyUtils.h"
#include "Core/TerrainData.h"

#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreMaterialManager.h>

namespace Fairy 
{
	using namespace Ogre;

	#define HIT_SPHERE_RADIUS	10
	// ����λ��
	void CPointInfo::SetPos(Ogre::Vector3 position)
	{
		m_Position = position;

		if(m_pSceneNode)
		{
			m_pSceneNode->setPosition(position); 
		}
	}

	// �õ�λ��
	Ogre::Vector3 CPointInfo::GetPosition()
	{
		return m_Position;
	}

	// ���ò���
	void CPointInfo::SetMaterial(String strMaterialName)
	{
		if(m_pEntity)
		{
			m_pEntity->setMaterialName(strMaterialName);
		}
	}

	// ���ø������ڵ�.
	void CPointInfo::SetParentSceneNode(Ogre::SceneNode* pSceneNode)
	{
		m_pParentSceneNode = pSceneNode;
	}

	// ����SceneManipulator���
	void CPointInfo::SetSceneManipulator(SceneManipulator*  SceneManipulator)
	{
		m_pSceneManipulator = SceneManipulator;
	}

	// ����ģ��
	unsigned long CPointInfo::CreatePointShape()
	{
		CreateSceneNode();
		return CreateEntity();
		//return 0;
	}

	// ����ģ��
	unsigned long CPointInfo::CreateSceneNode()
	{
		if(m_pParentSceneNode)
		{
			 m_pSceneNode = (SceneNode*)m_pParentSceneNode->createChildSceneNode();
			 if(m_pSceneNode)
			 {
				 // �����ɹ�.
				 return 0;
			 }
			 else
			 {
				 // ����ʧ��.
				 return 0xffffffff;
			 }
		}

		// �����ɹ�.
		return 0;
	}


	// ����ģ��
	unsigned long CPointInfo::CreateEntity()
	{

		if(m_pEntity)
		{
			return 0;
		}

		static int iCount = 0;
		iCount++;

        if((m_pSceneManipulator)&&(m_pSceneManipulator->getSceneManager()))
		{
			char Namebuf[256];
			memset(Namebuf, 0, sizeof(Namebuf));
			sprintf(Namebuf, "Region%d_Point%d_%d", m_ulParentId, m_ulPointId, iCount);

			Ogre::MeshPtr sphereMesh = createCommonSphere(HIT_SPHERE_RADIUS, 16, 16);
	     	m_pEntity = m_pSceneManipulator->getSceneManager()->createEntity(Namebuf, sphereMesh->getName());
			m_pEntity->setMaterialName("Region/PointSel");

			if(m_pEntity)
			{
				if(m_pSceneNode)
				{
					m_pSceneNode->attachObject(m_pEntity);
					m_pSceneNode->setPosition(m_Position);
				}
				else
				{
					return 0xfffffffe;
				}

				return 0;
			}
			else
			{
				return 0xffffffff;
			}
			
		}
		return 0;
	}


	// ���ø������id
	void CPointInfo::SetParentRegionId(unsigned long ulParentId)
	{
		m_ulParentId = ulParentId;
	}


	// ���ͷų���֮ǰ���ͷ�����ʵ��
	void CPointInfo::DetachEntity()
	{
		if((m_pSceneNode)&&(m_pEntity))
		{
			Ogre::SceneManager* creator = m_pSceneNode->getCreator();
			m_pSceneNode->detachObject(m_pEntity);
			if(creator)
			{
				creator->destroyEntity(m_pEntity);
			}
				
		}
	}


	void CPointInfo::SetSelMaterial()
	{
		SetMaterial("Region/PointSel");
	}


	// ���ͷų���֮ǰ���ͷų����ڵ�
	void CPointInfo::DelSceneNode()
	{
		DetachEntity();

		if((m_pParentSceneNode)&&(m_pSceneNode))
		{
			m_pParentSceneNode->removeAndDestroyChild (m_pSceneNode->getName());
		}
		
	}



	/****************************************/
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��������������
	//
	CUnreachRegion::CUnreachRegion(void)
		: m_ulId(0)				
		, m_ulCurMaxPointId(0)
		, m_ulCurMaxLineId(0)
		, m_pParentSceneNode(NULL)
		, m_pSceneNode(NULL)
		, m_pSceneManipulator(NULL)
		, m_ulCurEditPointId(0)
		, m_ulCurEditLineId(0)	
		, m_pLineSceneNode(NULL)
		, m_pLineSceneNode1(NULL)
		, m_pAreaSceneNode(NULL)
		, m_pAreaSceneNode1(NULL)
		, m_pLineShape(NULL)
		, m_pLineShape1(NULL)
		, m_bFlyEnable(false)
	{
	}

	CUnreachRegion::CUnreachRegion(unsigned long ulId, Ogre::SceneNode* pParentSceneNode, SceneManipulator* pSceneManipulator)
		: m_ulId(ulId)				
		, m_ulCurMaxPointId(0)
		, m_ulCurMaxLineId(0)
		, m_pParentSceneNode(pParentSceneNode)
		, m_pSceneNode(NULL)
		, m_pSceneManipulator(pSceneManipulator)
		, m_ulCurEditPointId(0)
		, m_ulCurEditLineId(0)	
		, m_pLineSceneNode(NULL)
		, m_pLineSceneNode1(NULL)
		, m_pAreaSceneNode(NULL)
		, m_pAreaSceneNode1(NULL)
		, m_pLineShape(NULL)
		, m_pLineShape1(NULL)
		, m_bFlyEnable(false)

	{
		CreateSceneNode();
		CreateLineShape();
		CreateLineShape1();
	}

	CUnreachRegion::~CUnreachRegion(void)
	{

		// �����������.
		ClearlineShape();

		// ��յ������.
		ClearPointInfo();

	}


	// ���ø������ڵ�.
	void CUnreachRegion::SetParentSceneNode(Ogre::SceneNode* pSceneNode)
	{
		m_pParentSceneNode = pSceneNode;
	}

	// ����SceneManipulator���
	void CUnreachRegion::SetSceneManipulator(SceneManipulator*  SceneManipulator)
	{
		m_pSceneManipulator = SceneManipulator;
	}

	// ���������ڵ�ģ��.
	unsigned long CUnreachRegion::CreateSceneNode()
	{

		if(m_pSceneNode)
		{
			return 0;
		}

		if(m_pParentSceneNode)
		{
			 m_pSceneNode = (SceneNode*)m_pParentSceneNode->createChildSceneNode();
			 if(m_pSceneNode)
			 {
				 // �����ߵĳ����ڵ�
				 m_pLineSceneNode = (SceneNode*)m_pSceneNode->createChildSceneNode();
				 m_pLineSceneNode1 = (SceneNode*)m_pSceneNode->createChildSceneNode();
				 m_pAreaSceneNode = (SceneNode*)m_pSceneNode->createChildSceneNode();
				 m_pAreaSceneNode1 = (SceneNode*)m_pSceneNode->createChildSceneNode();

				 // �����ɹ�.
				 return 0;
			 }
			 else
			 {
				 // ����ʧ��.
				 return 0xffffffff;
			 }
		}

		// �����ɹ�.
		return 0;
	}

	// ����µĵ�
	unsigned long CUnreachRegion::AddNewPoint(Ogre::Vector3 position)
	{
		// ��ʱ���ܷ����һ������ж�ȥ����
		// ����ж������û�������һ������༭��ť��ʱ�������жϡ�


		HideAreaNode(true);
		//if(CanAddNewPoint(position))
		{
			// ����ǰһ��С��Ĳ���
			SetPrePointMaterial();
			
			// ����һ���µ���
			m_ulCurMaxPointId++;
			unsigned long ulPointId = m_ulCurMaxPointId;
			CPointInfo* pPointInfo = new CPointInfo(
													position,
													m_pSceneNode,
													m_pSceneManipulator,
													ulPointId,
													m_ulId
													);

			if(pPointInfo)
			{
				
				m_pointMap[ulPointId] = pPointInfo;
				m_ulCurEditPointId = ulPointId;

				m_pLineShape->AddNewPoint(position);
				//ReShapeArea();
				return m_ulCurEditPointId;
			
			}
		}

		return 0;
	}

	// ����µĵ�
	unsigned long CUnreachRegion::AddNewPoint1(Ogre::Vector3 position)
	{
		HideAreaNode(true);
		
		// ����ǰһ��С��Ĳ���
		SetPrePointMaterial();

		// ����һ���µ���
		m_ulCurMaxPointId++;
		unsigned long ulPointId = m_ulCurMaxPointId;
		CPointInfo* pPointInfo = new CPointInfo(position, m_pSceneNode, m_pSceneManipulator, ulPointId, m_ulId);
		if(pPointInfo)
		{
			m_pointMap1[ulPointId] = pPointInfo;
			m_ulCurEditPointId = ulPointId;

			m_pLineShape1->AddNewPoint1(position);
			return m_ulCurEditPointId;
		}
		
		return 0;
	}

	void  CUnreachRegion::SetPrePointMaterial()
	{
		if(m_pointMap.count(m_ulCurEditPointId))
		{
			CPointInfo* pPointInfo = m_pointMap[m_ulCurEditPointId];
			if(pPointInfo)
			{
				pPointInfo->SetMaterial("Region/RegionSel");
			}
		}
		if(m_pointMap1.count(m_ulCurEditPointId))
		{
			CPointInfo* pPointInfo = m_pointMap1[m_ulCurEditPointId];
			if(pPointInfo)
			{
				pPointInfo->SetMaterial("Region/RegionSel");
			}
		}
	}


	// ������ʵ��Ĳ���
	Ogre::MaterialPtr	CUnreachRegion::_createIntersectGridMaterial(void)
    {
        String name = "kkk";

        Ogre::ResourcePtr res = Ogre::MaterialManager::getSingleton().getByName(name);
        if (!res.isNull())
            return res;

        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
       
        material->setLightingEnabled(false);
        material->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
        material->setDepthWriteEnabled(false);

		return material;

	}

	// ������ʵ��
	unsigned long CUnreachRegion::CreateLineShape1()
	{
		m_pLineShape1 = new CCustomShape;
		if(m_pLineShape1)
		{
			m_pLineShape1->setQueryFlags(0);
			if(m_pLineSceneNode1)
			{
				m_pLineSceneNode1->attachObject(m_pLineShape1);
			}

			if(m_pAreaSceneNode1)
			{
				m_pAreaSceneNode1->attachObject(&m_AreaShape1);
			}

			m_pLineShape1->FillPointData1();
		}

		m_pLineShape1->setMaterial(_createIntersectGridMaterial()->getName()); 

		return 0;

	}

	unsigned long CUnreachRegion::CreateLineShape()
	{
		m_pLineShape = new CCustomShape;
		if(m_pLineShape)
		{
			m_pLineShape->setQueryFlags(0);
			if(m_pLineSceneNode)
			{
				m_pLineSceneNode->attachObject(m_pLineShape);
			}

			if(m_pAreaSceneNode)
			{
				m_pAreaSceneNode->attachObject(&m_AreaShape);
			}

			m_pLineShape->FillPointData();
		}

		m_pLineShape->setMaterial(_createIntersectGridMaterial()->getName()); 

		return 0;
	}

	// �ͷ���ʵ����Դ
	void CUnreachRegion::ClearlineShape()
	{
		delete m_pLineShape;
		m_pLineShape = NULL;
		delete m_pLineShape1;
		m_pLineShape1 = NULL;
	}


	// ��������Ϣ
	void CUnreachRegion::ClearPointInfo()
	{
		POINT_MAP::iterator it;
		POINT_MAP::iterator itEnd;

		itEnd = m_pointMap.end();
		CPointInfo* pPointInfo = NULL;
		for(it = m_pointMap.begin(); it != itEnd; it++)
		{
			pPointInfo = it->second;
			if(pPointInfo)
			{
				delete pPointInfo;
			}
		}

		itEnd = m_pointMap1.end();
		for(it = m_pointMap1.begin(); it != itEnd; it++)
		{
			pPointInfo = it->second;
			if(pPointInfo)
			{
				delete pPointInfo;
			}
		}
	}


	// ͨ���±꣬ �õ�һ�������Ϣ.
	CPointInfo* CUnreachRegion::GetPointInfoByIndex(int iIndex)
	{
		if(-1 == iIndex)
		{
			return NULL;
		}

		POINT_MAP::iterator it;
		POINT_MAP::iterator itEnd;

		itEnd = m_pointMap.end();
		it	  = m_pointMap.begin();

		while((iIndex)&&(it != itEnd))
		{
			iIndex--;
			it++;
		}

		if(it != itEnd)
		{
			CPointInfo* pInfo = it->second;
			return pInfo;
		}

		return NULL;
	}

	// ͨ���±꣬ �õ�һ�������Ϣ.
	CPointInfo* CUnreachRegion::GetPointInfoByIndex1(int iIndex)
	{
		if(-1 == iIndex)
		{
			return NULL;
		}

		POINT_MAP::iterator it;
		POINT_MAP::iterator itEnd;

		itEnd = m_pointMap1.end();
		it	  = m_pointMap1.begin();

		while((iIndex)&&(it != itEnd))
		{
			iIndex--;
			it++;
		}

		if(it != itEnd)
		{
			CPointInfo* pInfo = it->second;
			return pInfo;
		}

		return NULL;
	}

	//ͨ��id�� �õ�һ�������Ϣ.
	CPointInfo* CUnreachRegion::GetPointInfoById(unsigned long ulId)
	{
		if(m_pointMap.count(ulId))
		{
			CPointInfo* pInfo = m_pointMap[ulId];
			return pInfo;
		}

		return NULL;
	}

	CPointInfo* CUnreachRegion::GetPointInfoById1(unsigned long ulId)
	{
		if(m_pointMap1.count(ulId))
		{
			CPointInfo* pInfo = m_pointMap1[ulId];
			return pInfo;
		}

		return NULL;
	}

	// �õ���ĸ���
	int CUnreachRegion::GetPointCount()
	{
		return m_pointMap.size();
	}

	int CUnreachRegion::GetPointCount1()
	{
		return m_pointMap1.size();
	}

	// �����������
	void CUnreachRegion::SetRegionMaterial(String strMatrerial)
	{
		POINT_MAP::iterator it;
		POINT_MAP::iterator itEnd;

		itEnd = m_pointMap.end();

		CPointInfo* pPointInfo = NULL;
		for(it = m_pointMap.begin(); it != itEnd; it++)
		{
			pPointInfo = it->second;
			if(pPointInfo)
			{
				pPointInfo->SetMaterial(strMatrerial);
			}
		}

		itEnd = m_pointMap1.end();
		for(it = m_pointMap1.begin(); it != itEnd; it++)
		{
			pPointInfo = it->second;
			if(pPointInfo)
			{
				pPointInfo->SetMaterial(strMatrerial);
			}
		}
	}

	// ������������״̬���ʲ���.
	void CUnreachRegion::SetRegionNorMaterial()
	{
		SetRegionMaterial("Region/RegionNor");
		SetLineNorColor();
		m_AreaShape.setMaterial("Region/AreaRegionSel");
	}
	void CUnreachRegion::SetRegionNorMaterial1()
	{
		SetRegionMaterial("Region/RegionNor");
		SetLineNorColor();
		m_AreaShape1.setMaterial("Region/AreaRegionSel");
	}


	// ��������ѡ��״̬����.
	void CUnreachRegion::SetRegionSelMaterial()
	{
		SetRegionMaterial("Region/RegionSel");
		SetLineSelColor();
		m_AreaShape.setMaterial("Region/AreaRegionNor");
		
	}
	void CUnreachRegion::SetRegionSelMaterial1()
	{
		SetRegionMaterial("Region/RegionSel");
		SetLineSelColor();
		m_AreaShape1.setMaterial("Region/AreaRegionNor");

	}


		// �����ߵ���ɫ
	void CUnreachRegion::SetLineColor(Ogre::ColourValue color)
	{
		if(m_pLineShape)
		{
			m_pLineShape->SetDefaultColor(color);
			m_pLineShape->FillPointData();
		}
	}
	void CUnreachRegion::SetLineColor1(Ogre::ColourValue color)
	{
		if(m_pLineShape1)
		{
			m_pLineShape1->SetDefaultColor(color);
			m_pLineShape1->FillPointData1();
		}
	}

		// ������ѡ�е���ɫ
	void CUnreachRegion::SetLineSelColor()
	{
		SetLineColor(Ogre::ColourValue(0.8, 0.5, 0.2, 0.7));
	}

		// �����ߵ���������ɫ
	void CUnreachRegion::SetLineNorColor()
	{
		SetLineColor(Ogre::ColourValue(0.2, 0.5, 0.8, 0.7));
	}


	
	// ���ݵ��θı����е�ĸ߶�
	void CUnreachRegion::SetPointHeight( TerrainData* pTerrainData)
	{
		if(NULL == pTerrainData)
		{
			return;
		}

		POINT_MAP::iterator it;
		POINT_MAP::iterator itEnd;
		CPointInfo* pPointInfo = NULL;
		Ogre::Vector3  point;
		float fHeight = 0;
		
		itEnd = m_pointMap.end();
		for(it = m_pointMap.begin(); it != itEnd; it++)
		{
			pPointInfo = it->second;
			if(pPointInfo)
			{
				point = pPointInfo->GetPosition();
				if( point.y == -100000 )
				{
					point.y = pTerrainData->getHeightAt(point.x, point.z);
					pPointInfo->SetPos(point);
				}

			}
		}

	}
	void CUnreachRegion::SetPointHeight1( TerrainData* pTerrainData)
	{
		if(NULL == pTerrainData)
		{
			return;
		}

		POINT_MAP::iterator it;
		POINT_MAP::iterator itEnd;
		CPointInfo* pPointInfo = NULL;
		Ogre::Vector3  point;
		float fHeight = 0;

		itEnd = m_pointMap1.end();
		for(it = m_pointMap1.begin(); it != itEnd; it++)
		{
			pPointInfo = it->second;
			if(pPointInfo)
			{
				point = pPointInfo->GetPosition();
				if( point.y == -100000 )
				{
					point.y = pTerrainData->getHeightAt(point.x, point.z);
					pPointInfo->SetPos(point);
				}

			}
		}
	}
	// ���ݵ����޸������ߵĸ߶�λ��
	void CUnreachRegion::SetLineHeight( TerrainData* pTerrainData)
	{
		if(NULL == pTerrainData)
		{
			return;
		}

		if(m_pLineShape)
		{
			int iCount = m_pLineShape->m_pointArray.size();

			for(int i = 0; i < iCount; i++)
			{
				if( m_pLineShape->m_pointArray[i].y == -100000 )
				{
					m_pLineShape->m_pointArray[i].y = pTerrainData->getHeightAt(m_pLineShape->m_pointArray[i].x, m_pLineShape->m_pointArray[i].z);
				}
			}

			m_pLineShape->FillPointData();
		}
	}
	void CUnreachRegion::SetLineHeight1( TerrainData* pTerrainData)
	{
		if(NULL == pTerrainData)
		{
			return;
		}

		if(m_pLineShape1)
		{
			int iCount = m_pLineShape1->m_pointArray.size();

			for(int i = 0; i < iCount; i++)
			{
				if( m_pLineShape1->m_pointArray[i].y == -100000 )
				{
					m_pLineShape1->m_pointArray[i].y = pTerrainData->getHeightAt(m_pLineShape1->m_pointArray[i].x, m_pLineShape1->m_pointArray[i].z);
				}
			}

			m_pLineShape1->FillPointData1();
		}
	}


	// ���ͷų���֮ǰ���ͷ�����ʵ��
	void CUnreachRegion::DetachAllEntity()
	{
		// �ͷ���ʵ��
		if((m_pLineShape)&&(m_pLineSceneNode))
		{
			m_pLineSceneNode->detachObject(m_pLineShape->getName());
			m_pAreaSceneNode->detachObject(m_AreaShape.getName());
		}

		if((m_pLineShape1)&&(m_pLineSceneNode1))
		{
			m_pLineSceneNode1->detachObject(m_pLineShape1->getName());
			m_pAreaSceneNode1->detachObject(m_AreaShape1.getName());
		}

		// �ͷ����е�ʵ��.
		POINT_MAP::iterator it;
		POINT_MAP::iterator itEnd;
		CPointInfo* pPointInfo = NULL;
				
		itEnd = m_pointMap.end();
		for(it = m_pointMap.begin(); it != itEnd; it++)
		{
			pPointInfo = it->second;
			if(pPointInfo)
			{
				pPointInfo->DetachEntity();
			}
		}

		itEnd = m_pointMap1.end();
		for(it = m_pointMap1.begin(); it != itEnd; it++)
		{
			pPointInfo = it->second;
			if(pPointInfo)
			{
				pPointInfo->DetachEntity();
			}
		}
	}

	// ɾ�������ӽڵ�
	void CUnreachRegion::DestroyAllChildSceneNode()
	{

		// ɾ�����е�ڵ�
		POINT_MAP::iterator it;
		POINT_MAP::iterator itEnd;
		CPointInfo* pPointInfo = NULL;
				
		itEnd = m_pointMap.end();
		for(it = m_pointMap.begin(); it != itEnd; it++)
		{
			pPointInfo = it->second;
			if(pPointInfo)
			{
				m_pSceneNode->removeAndDestroyChild (pPointInfo->m_pSceneNode->getName());

			}
		}

		itEnd = m_pointMap1.end();
		for(it = m_pointMap1.begin(); it != itEnd; it++)
		{
			pPointInfo = it->second;
			if(pPointInfo)
			{
				m_pSceneNode->removeAndDestroyChild (pPointInfo->m_pSceneNode->getName());

			}
		}

		// ɾ���߽ڵ�
		if(m_pLineSceneNode)
		{
			m_pSceneNode->removeAndDestroyChild(m_pLineSceneNode->getName());
			m_pSceneNode->removeAndDestroyChild(m_pAreaSceneNode->getName());
		}

		if(m_pLineSceneNode1)
		{
			m_pSceneNode->removeAndDestroyChild(m_pLineSceneNode1->getName());
			m_pSceneNode->removeAndDestroyChild(m_pAreaSceneNode1->getName());
		}
	}

	
// �ж�����ֱ���Ƿ��ཻ.
bool CUnreachRegion::CheckCross(CRegionPoint &fvMS, CRegionPoint &fvME, CRegionPoint &fvNS, CRegionPoint &fvNE)
{
	
	if ( max(fvMS.x,fvME.x) <= min(fvNS.x,fvNE.x) )
	{
		return false;
	}
	if ( min(fvMS.x,fvME.x) >= max(fvNS.x,fvNE.x) )
	{
		return false;
	}
	if ( max(fvMS.y,fvME.y) <= min(fvNS.y,fvNE.y) )
	{
		return false;
	}
	if ( min(fvMS.y,fvME.y) >= max(fvNS.y,fvNE.y) )
	{
		return false;
	}

	float temp1 = fvMulti(fvSub(fvMS,fvNS),fvSub(fvNE,fvNS)) * fvMulti( fvSub(fvNE,fvNS),fvSub(fvME,fvNS));
	float temp2 = fvMulti(fvSub(fvNS,fvMS),fvSub(fvME,fvMS)) * fvMulti( fvSub(fvME,fvMS),fvSub(fvNE,fvMS));

	if((temp1>=-0.2)&&(temp2>=-0.2))
	{
		return true;
	}
	return false;	
}

bool CUnreachRegion::fvEuqual(const Ogre::Vector3& fv1, const Ogre::Vector3& fv2)
{
	if ((abs(fv1.x-fv2.x)<1e-10) &&(abs(fv1.z-fv2.z)<1e-10))
	{
		return true;
	}
	return false;
}

bool CUnreachRegion::CheckCross(Ogre::Vector3 fvMS, Ogre::Vector3 fvME, Ogre::Vector3 fvNS, Ogre::Vector3 fvNE)
{
	if ( max(fvMS.x,fvME.x) <= min(fvNS.x,fvNE.x) )
	{
		return FALSE;
	}
	if ( min(fvMS.x,fvME.x) >= max(fvNS.x,fvNE.x) )
	{
		return FALSE;
	}
	if ( max(fvMS.z,fvME.z) <= min(fvNS.z,fvNE.z) )
	{
		return FALSE;
	}
	if ( min(fvMS.z,fvME.z) >= max(fvNS.z,fvNE.z) )
	{
		return FALSE;
	}

	FLOAT temp1 = fvMulti(fvSub(fvMS,fvNS),fvSub(fvNE,fvNS)) * fvMulti( fvSub(fvNE,fvNS),fvSub(fvME,fvNS));
	FLOAT temp2 = fvMulti(fvSub(fvNS,fvMS),fvSub(fvME,fvMS)) * fvMulti( fvSub(fvME,fvMS),fvSub(fvNE,fvMS));

	if((temp1>=0)&&(temp2>=0))
	{
		return TRUE;
	}
	return FALSE;	
}

bool CUnreachRegion::CheckCross(Ogre::Vector3 fvmStart, Ogre::Vector3 fvmEnd)
{
	POSITION_ARRAY& pa = m_pLineShape->GetPointArray();
	if( pa.size() <= 1 ) return false;

	for( int i = 0; i < (int)pa.size(); i ++ )
	{
		Ogre::Vector3 fv0 = pa[i];
		Ogre::Vector3 fv1;
		if( i == pa.size() -1 ) fv1 = pa[0];
		else fv1 = pa[i+1];
		if( !( fvEuqual(fvmStart,fv0) || fvEuqual(fvmStart,fv1) || fvEuqual(fvmEnd,fv0) || fvEuqual(fvmEnd,fv1) ))
		{
			if(CheckCross(fvmStart,fvmEnd,fv0,fv1))
				return true;
		}
	}
	return false;
}

// �Ƿ�������һ���µ�.
bool CUnreachRegion::CanAddNewPoint(Ogre::Vector3 position)
{
	// ������������
	CRegionPoint fvReverse3;
	// �����ڶ�����
	CRegionPoint fvReverse2;
	// ������һ����
	CRegionPoint fvReverse1;

    if(m_pLineShape)
	{
		int iPointCount = m_pLineShape->m_pointArray.size();

		// �������б���ֻ��������, ��������µ�.
		if(iPointCount <= 2)
		{
			return true;
		}

		// ������һ������0
		fvReverse1.x = m_pLineShape->m_pointArray[0].x;
		fvReverse1.y = m_pLineShape->m_pointArray[0].z;

		// �����ڶ�����������ӵĵ㡣
		fvReverse2.x = position.x;
		fvReverse2.y = position.z;

		// ����������������һ����Ӻ�����һ���㡣
		fvReverse3.x = m_pLineShape->m_pointArray[iPointCount -1].x;
		fvReverse3.y = m_pLineShape->m_pointArray[iPointCount -1].z;
		
		// ��һ����ʼ�ĵ㡣
		CRegionPoint p1s;
		// ��һ�������ĵ㡣
		CRegionPoint p1e;

		// �ڶ�����ʼ�ĵ㡣
		CRegionPoint p2s;

		// �ڶ��������ĵ㡣
		CRegionPoint p2e;

		for(int i = 0; i < iPointCount - 1; i++)
		{
			p1s = fvReverse2;
			p1e = fvReverse1;

			// 
			p2s.x = m_pLineShape->m_pointArray[i].x;
			p2s.y = m_pLineShape->m_pointArray[i].z;
		
			p2e.x = m_pLineShape->m_pointArray[ i + 1 ].x;
			p2e.y = m_pLineShape->m_pointArray[ i + 1 ].z;

			if(i != 0)
			{
				if(CheckCross(p1s, p1e, p2s, p2e))
				{
					return false;
				}
			}

			p1s = fvReverse3;
			p1e = fvReverse2;

			if( i != (iPointCount - 2))
			{
				if(CheckCross(p1s, p1e, p2s, p2e))
				{
					return false;
				}
			}
		}
	}

	return true;
}


CRegionPoint CUnreachRegion::fvSub(CRegionPoint fv1, CRegionPoint fv2)
{
	CRegionPoint fvResult = CRegionPoint(fv1.x-fv2.x,fv1.y-fv2.y);
	return fvResult;
}

Ogre::Vector3 CUnreachRegion::fvSub(Ogre::Vector3& fv1, Ogre::Vector3& fv2)
{
	Ogre::Vector3 fvResult = Ogre::Vector3(fv1.x-fv2.x, 0, fv1.z-fv2.z);
	return fvResult;
}

float CUnreachRegion::fvMulti(CRegionPoint fv1, CRegionPoint fv2)
{
	return (fv1.x*fv2.y - fv1.y*fv2.x);
}

float CUnreachRegion::fvMulti(Ogre::Vector3& fv1, Ogre::Vector3& fv2)
{
	return (fv1.x*fv2.z - fv1.z*fv2.x);
}

// �����ֵ
float CUnreachRegion::max(float f1, float f2)
{
	if(f1 > f2)
	{
		return f1;
	}
	else 
	{
		return f2;
	}
}

// ����Сֵ
float CUnreachRegion::min(float f1, float f2)
{

	if(f1 < f2)
	{
		return f1;
	}
	else
	{
		return f2;
	}
}


bool CUnreachRegion::CanChangePointPos(int iIndex, Ogre::Vector3 position)
{
	
	// ������������
	CRegionPoint fvReverse3;
	// �����ڶ�����
	CRegionPoint fvReverse2;
	// ������һ����
	CRegionPoint fvReverse1;

	// �������.

	// ������һ����
	int iIndexReverse1 = 0;

	// �����ڶ�����
	int iIndexReverse2 = 0;

	// ������������
	int iIndexReverse3 = 0;

    if(m_pLineShape)
	{
		int iPointCount = m_pLineShape->m_pointArray.size();

		// �������б���ֻ��������, ��������µ�.
		if(iPointCount <= 3)
		{
			return true;
		}


		if(0 == iIndex)
		{
			iIndexReverse1 = iIndex + 1;
			iIndexReverse2 = iIndex;
			iIndexReverse3 = iPointCount - 1;
		}
		else if((iPointCount - 1) == iIndex)
		{
			iIndexReverse1 = 0;
			iIndexReverse2 = iIndex;
			iIndexReverse3 = iIndex - 1;
		}
		else
		{
			iIndexReverse1 = iIndex + 1;
			iIndexReverse2 = iIndex;
			iIndexReverse3 = iIndex - 1;
		}

		// ������һ������0
		fvReverse1.x = m_pLineShape->m_pointArray[iIndexReverse1].x;
		fvReverse1.y = m_pLineShape->m_pointArray[iIndexReverse1].z;

		// �����ڶ�����������ӵĵ㡣
		fvReverse2.x = position.x;
		fvReverse2.y = position.z;

		// ����������������һ����Ӻ�����һ���㡣
		fvReverse3.x = m_pLineShape->m_pointArray[iIndexReverse3].x;
		fvReverse3.y = m_pLineShape->m_pointArray[iIndexReverse3].z;
		
		// ��һ����ʼ�ĵ㡣
		CRegionPoint p1s;
		// ��һ�������ĵ㡣
		CRegionPoint p1e;

		// �ڶ�����ʼ�ĵ㡣
		CRegionPoint p2s;

		// �ڶ��������ĵ㡣
		CRegionPoint p2e;

		for(int i = 0; i < iPointCount; i++)
		{
			p1s = fvReverse2;
			p1e = fvReverse1;

			// 
			p2s.x = m_pLineShape->m_pointArray[i].x;
			p2s.y = m_pLineShape->m_pointArray[i].z;
		
			// ����д����Ϊ�˴���ĩβ���һ��Ԫ��.
			p2e.x = m_pLineShape->m_pointArray[ ( i + 1 ) % iPointCount].x;
			p2e.y = m_pLineShape->m_pointArray[ ( i + 1 ) % iPointCount].z;

			// ������ֱ�����ڵ�ֱ�߶������.
			if((i != iIndexReverse1)&&(i != iIndexReverse2)&&(i != iIndexReverse3))
			{
				if(CheckCross(p1s, p1e, p2s, p2e))
				{
					return false;
				}
			}

			p1s = fvReverse3;
			p1e = fvReverse2;

			// ������ֱ�����ڵ�ֱ�߶������.
			// ���һ��������Ҫ�Ǵ��� 0 �� fvReverse3 �����.
			if(( i != iIndexReverse2) && ( i != iIndexReverse3) && (i != (iIndexReverse3 + iPointCount - 1) % iPointCount ))
			{
				if(CheckCross(p1s, p1e, p2s, p2e))
				{
					return false;
				}
			}
		}
	}

	return true;

}


// ָ��idָ��λ�����һ��С��
void CUnreachRegion::AddNewPoint(unsigned long ulId, Ogre::Vector3 position)
{
	// ����ǰһ��С��Ĳ���
	SetPrePointMaterial();
			
	// ����һ���µ���
	
	CPointInfo* pPointInfo = new CPointInfo(
											position,
											m_pSceneNode,
											m_pSceneManipulator,
											ulId,
											m_ulId
											);

	if(pPointInfo)
	{
		
		m_pointMap[ulId] = pPointInfo;
		m_ulCurEditPointId = ulId;
		m_pLineShape->AddNewPoint(position);
		//ReShapeArea();
		
	}
}

void CUnreachRegion::AddNewPoint1(unsigned long ulId, Ogre::Vector3 position)
{
	// ����ǰһ��С��Ĳ���
	SetPrePointMaterial();

	// ����һ���µ���

	CPointInfo* pPointInfo = new CPointInfo(
		position,
		m_pSceneNode,
		m_pSceneManipulator,
		ulId,
		m_ulId
		);

	if(pPointInfo)
	{

		m_pointMap1[ulId] = pPointInfo;
		m_ulCurEditPointId = ulId;
		m_pLineShape1->AddNewPoint1(position);
	}
}

// ͨ��id ɾ��һ��С��
void CUnreachRegion::DelPointById(unsigned long ulId)
{
	// �õ���ǰ���ڱ༭���������Ϣ.
	CPointInfo*     pPoint  = NULL;
		
	pPoint = GetPointInfoById(ulId);
	if(pPoint)
	{	
		
		// �����������±�ֵ.
		int iIndex = -1;
		POINT_MAP::iterator it;
		POINT_MAP::iterator itEnd;
		itEnd = m_pointMap.end();
		for(it = m_pointMap.begin(); it != itEnd; it++)
		{
			iIndex++;
			if(ulId == it->first)
			{
				break;
			}
		}
		

		if(it != itEnd)
		{
			if(m_pLineShape)
			{
				// ����±����
				if((-1 != iIndex)&&(iIndex < (int)m_pLineShape->m_pointArray.size()))
				{
					POSITION_ARRAY::iterator itPoint;
					POSITION_ARRAY::iterator itPointEnd;

					itPointEnd = m_pLineShape->m_pointArray.end();
					itPoint = m_pLineShape->m_pointArray.begin();

					while(iIndex && ( itPoint != itPointEnd))
					{
						itPoint++;
						iIndex--;
					}
					
					m_pLineShape->m_pointArray.erase(itPoint);
					m_pLineShape->FillPointData();
					//ReShapeArea();


				}
			}
		}

		pPoint->DelSceneNode();
		delete pPoint;
		m_pointMap.erase(it);
	}
}
void CUnreachRegion::DelPointById1(unsigned long ulId)
{
	// �õ���ǰ���ڱ༭���������Ϣ.
	CPointInfo*     pPoint  = NULL;

	pPoint = GetPointInfoById1(ulId);
	if(pPoint)
	{	

		// �����������±�ֵ.
		int iIndex = -1;
		POINT_MAP::iterator it;
		POINT_MAP::iterator itEnd;
		itEnd = m_pointMap1.end();
		for(it = m_pointMap1.begin(); it != itEnd; it++)
		{
			iIndex++;
			if(ulId == it->first)
			{
				break;
			}
		}


		if(it != itEnd)
		{
			if(m_pLineShape1)
			{
				// ����±����
				if((-1 != iIndex)&&(iIndex < (int)m_pLineShape1->m_pointArray.size()))
				{
					POSITION_ARRAY::iterator itPoint;
					POSITION_ARRAY::iterator itPointEnd;

					itPointEnd = m_pLineShape1->m_pointArray.end();
					itPoint = m_pLineShape1->m_pointArray.begin();

					while(iIndex && ( itPoint != itPointEnd))
					{
						itPoint++;
						iIndex--;
					}

					m_pLineShape1->m_pointArray.erase(itPoint);
					m_pLineShape1->FillPointData1();
					//ReShapeArea();


				}
			}
		}

		pPoint->DelSceneNode();
		delete pPoint;
		m_pointMap1.erase(it);
	}
}

// ��β�� ɾ��һ��С��
void CUnreachRegion::DelPointFromTail(unsigned long ulId)
{
	// �õ���ǰ���ڱ༭���������Ϣ.
	CPointInfo*			pPoint  = NULL;
	POINT_MAP::iterator it;
	it = m_pointMap.find(ulId);

	if(it != m_pointMap.end())
	{
		
		
		if(m_pLineShape)
		{
			m_pLineShape->m_pointArray. pop_back();
			m_pLineShape->FillPointData();
			//ReShapeArea();
			
		}

		pPoint = it->second;
		if(pPoint)
		{
			pPoint->DelSceneNode();
			delete pPoint;
		}
		m_pointMap.erase(it);

	}
}

void CUnreachRegion::DelPointFromTail1(unsigned long ulId)
{
	// �õ���ǰ���ڱ༭���������Ϣ.
	CPointInfo*			pPoint  = NULL;
	POINT_MAP::iterator it;
	it = m_pointMap1.find(ulId);

	if(it != m_pointMap1.end())
	{


		if(m_pLineShape1)
		{
			m_pLineShape1->m_pointArray. pop_back();
			m_pLineShape1->FillPointData1();
			//ReShapeArea();

		}

		pPoint = it->second;
		if(pPoint)
		{
			pPoint->DelSceneNode();
			delete pPoint;
		}
		m_pointMap1.erase(it);

	}
}

bool CUnreachRegion::IsMouseSel(float x, float y)
{
	Ogre::Camera*	pCamera	  = NULL;
	Ogre::Viewport* pViewport = NULL;

	if(NULL == m_pSceneManipulator)
	{
		return false;
	}

	pCamera   = m_pSceneManipulator->getCamera();
	pViewport = m_pSceneManipulator->getViewport();

	if(pCamera)
	{
		const Ogre::Matrix4& matrixProj = pCamera->getProjectionMatrix();
		const Ogre::Matrix4& matrixView = pCamera->getViewMatrix();
		Ogre::Matrix4 matrixViewInvers;

		matrixViewInvers = matrixView.inverse();

		Ogre::Vector3 v;
		Ogre::Vector3 dir;
		Ogre::Vector3 orig;
		Ogre::Vector3 v1;
		Ogre::Vector3 v2;
		Ogre::Vector3 v3;

		int iViewWidth  = 0;
		int iViewHeight = 0;

		iViewWidth  = pViewport->getActualWidth();
		iViewHeight = pViewport->getActualHeight();

		

		v.x =  ( ( ( 2.0f * x + 2) / iViewWidth ) - 1) / matrixProj[0][0];
		v.y =  -( ( ( 2.0f * y + 2) / iViewHeight) - 1) / matrixProj[1][1];
		v.z =  -1.0f;

		// �����ߵķ����ԭ�㡣
		dir.x  = v.x * matrixViewInvers[0][0] + v.y * matrixViewInvers[0][1] + v.z * matrixViewInvers[0][2];
		dir.y  = v.x * matrixViewInvers[1][0] + v.y * matrixViewInvers[1][1] + v.z * matrixViewInvers[1][2];
		dir.z  = v.x * matrixViewInvers[2][0] + v.y * matrixViewInvers[2][1] + v.z * matrixViewInvers[2][2];
		orig.x = matrixViewInvers[0][3];
		orig.y = matrixViewInvers[1][3];
		orig.z = matrixViewInvers[2][3];

		//Ogre::Ray ray = m_pSceneManipulator->getWindowToViewportRay(x, y);
		if(m_pLineShape)
		{
			//const AxisAlignedBox& BindBox = m_pLineShape->getWorldBoundingBox(true);
			int iPointCount = m_pLineShape->m_pointArray.size();
			for(int i = 0; i < iPointCount - 2; i++)
			{
				// ���һ����
				v1.x = m_pLineShape->m_pointArray[0].x;
				v1.y = m_pLineShape->m_pointArray[0].y;
				v1.z = m_pLineShape->m_pointArray[0].z;

				// ��ڶ�����
				v2.x = m_pLineShape->m_pointArray[i+1].x;
				v2.y = m_pLineShape->m_pointArray[i+1].y;
				v2.z = m_pLineShape->m_pointArray[i+1].z;

				// ���������
				v3.x = m_pLineShape->m_pointArray[i+2].x;
				v3.y = m_pLineShape->m_pointArray[i+2].y;
				v3.z = m_pLineShape->m_pointArray[i+2].z;

				if(IsSelTri(dir, orig, v1, v2, v3))
				{
					return true;
				}

			}

		}//
		else
		{
			return false;
		}//
	}

	return false;
}

bool CUnreachRegion::IsSelTri(Ogre::Vector3 dir, Ogre::Vector3 orig, Ogre::Vector3 v1, Ogre::Vector3 v2, Ogre::Vector3 v3)
{
	
	float    fu = 0;
	float    fv = 0;
	float    fDist = 0;
	float    fDet  = 0;
	
	Ogre::Vector3 edge1 = v2 - v1;
	Ogre::Vector3 edge2 = v3 - v1;

	Ogre::Vector3 pvec;
	pvec = dir.crossProduct(edge2);
	fDet = edge1.dotProduct(pvec);
  
    Ogre::Vector3 tvec;
    if( fDet > 0 )
    {
        tvec = orig - v1;
    }
    else
    {
        tvec = v1 - orig;
        fDet = -fDet;
    }

    if( fDet < 0.0001f )
        return false;

    fu = tvec.dotProduct(pvec);
    if( fu < 0.0f || fu > fDet )
	{
        return false;
	}

    // Prepare to test V parameter
    Ogre::Vector3 qvec;
	qvec = tvec.crossProduct(edge1);

    // Calculate V parameter and test bounds
	fv = dir.dotProduct(qvec);
    if( fv < 0.0f || fu + fv > fDet )
	{
        return false;
	}

  return true;
}//


void CUnreachRegion::ReShapeArea()
{
	if((m_pLineShape)&&(m_pLineShape->m_pointArray.size()))
	{
		m_AreaShape.m_pointArray.clear();
		m_AreaShape.m_pointArray.assign(m_pLineShape->m_pointArray.begin(), m_pLineShape->m_pointArray.end());
		m_AreaShape.buildPlanePoint();
		m_AreaShape.FillAreaPointData();
		
		m_AreaShape.setMaterial("Region/AreaRegionNor");
	}
}

void CUnreachRegion::ReShapeArea1()
{
	if((m_pLineShape1)&&(m_pLineShape1->m_pointArray.size()))
	{
		m_AreaShape1.m_pointArray.clear();
		m_AreaShape1.m_pointArray.assign(m_pLineShape1->m_pointArray.begin(), m_pLineShape1->m_pointArray.end());
		m_AreaShape1.buildPlanePoint();
		//m_AreaShape1.FillAreaPointData();

		//m_AreaShape1.setMaterial("Region/AreaRegionNor");
	}
}

// ��������ڵ㡣
void CUnreachRegion::HideAreaNode(bool bHide)
{
	if(m_pAreaSceneNode)
	{
		m_pAreaSceneNode->setVisible(!bHide);
	}
}

void CUnreachRegion::HideAreaNode1(bool bHide)
{
	if(m_pAreaSceneNode1)
	{
		m_pAreaSceneNode1->setVisible(!bHide);
	}
}

// �����߽ڵ㡣
void CUnreachRegion::HideLineNode(bool bHide)
{
	if(m_pLineSceneNode)
	{
		m_pLineSceneNode->setVisible(!bHide);
		
	}
}

	// �ı�û����Ȼ������Ĳ���
void CUnreachRegion::ChangeNoDeepMaterial(bool bOpenDeepBuf)
{
	if(m_AreaShape.m_pointArray.size())
	{
		// ѡ�в��ʵ�����.
		Ogre::String strCloneName;

		Ogre::MaterialPtr pMaterial = m_AreaShape.getMaterial();
		
		if(pMaterial.isNull())
		{
			return;
		}//

		const Ogre::String& strName = pMaterial->getName();
		if("BaseWhite" == strName)
		{
			return;
		}

		Ogre::MaterialManager* pMaterialManager = (Ogre::MaterialManager*)(pMaterial->getCreator());
		if(NULL == pMaterialManager)
		{
			return;
		}

		//Ogre::Technique* pTechnique = pMaterial->getBestTechnique();
		//Ogre::Pass* pPass = pTechnique->getPass(0);
		//pPass->setDepthCheckEnabled(bOpenDeepBuf);//

		// 	
		pMaterial = pMaterialManager->getByName("Region/AreaRegionNor"); 
		if(!pMaterial.isNull())
		{
			Ogre::Technique* pTechnique = pMaterial->getBestTechnique();
			Ogre::Pass* pPass = pTechnique->getPass(0);
			pPass->setDepthCheckEnabled(bOpenDeepBuf);//
		}

		pMaterial = pMaterialManager->getByName("Region/AreaRegionSel"); 
		if(!pMaterial.isNull())
		{
			Ogre::Technique* pTechnique = pMaterial->getBestTechnique();
			Ogre::Pass* pPass = pTechnique->getPass(0);
			pPass->setDepthCheckEnabled(bOpenDeepBuf);//
		}
		
	}
}


// �����Ƿ�Ϸ�
bool CUnreachRegion::IsRegionValidate()
{

    if(m_pLineShape)
	{
		int iPointCount = m_pLineShape->m_pointArray.size();

		// �������б���ֻ��������, ��������µ�.
		if(iPointCount <= 2)
		{
			return true;
		}

		// ��һ����ʼ�ĵ㡣
		CRegionPoint p1s;
		// ��һ�������ĵ㡣
		CRegionPoint p1e;

		// �ڶ�����ʼ�ĵ㡣
		CRegionPoint p2s;

		// �ڶ��������ĵ㡣
		CRegionPoint p2e;

		for(int i = 0; i < iPointCount - 2; i++)
		{
			p1s.x = m_pLineShape->m_pointArray[i].x;
			p1s.y = m_pLineShape->m_pointArray[i].z;
			p1e.x = m_pLineShape->m_pointArray[i+1].x;
			p1e.y = m_pLineShape->m_pointArray[i+1].z;
			for(int j = i+2; j < iPointCount; j++)
			{
			
				if(j != iPointCount - 1)
				{
					p2s.x = m_pLineShape->m_pointArray[j].x;
					p2s.y = m_pLineShape->m_pointArray[j].z;
					p2e.x = m_pLineShape->m_pointArray[ j + 1 ].x;
					p2e.y = m_pLineShape->m_pointArray[ j + 1 ].z;
				}
				else
				{
					p2s.x = m_pLineShape->m_pointArray[j].x;
					p2s.y = m_pLineShape->m_pointArray[j].z;
					p2e.x = m_pLineShape->m_pointArray[0].x;
					p2e.y = m_pLineShape->m_pointArray[0].z;

					if(0 == i)
					{
						continue;
					}
				}

				if(CheckCross(p1s, p1e, p2s, p2e))
				{
					return false;
				}
				
			}
		}
	}

	return true;

}

}
	
