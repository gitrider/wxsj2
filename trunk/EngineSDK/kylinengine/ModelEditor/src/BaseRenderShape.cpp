#include "BaseRenderShape.h"
#include "SceneManipulator.h"

#include "Core/FairyUtils.h"
#include "Core/TerrainData.h"

#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>

namespace Fairy 
{

	using namespace Ogre;
	/////////////////////////////////////////////////////////////////////////////////
	//
	// ����������
	//

	// ���ø������ڵ�.
	void CBaseRenderShape::SetParentSceneNode(Ogre::SceneNode* pSceneNode)
	{
		m_pParentSceneNode = pSceneNode;
	}

	// ����SceneManipulator���
	void CBaseRenderShape::SetSceneManipulator(SceneManipulator*  SceneManipulator)
	{
		m_pSceneManipulator = SceneManipulator;
	}

	// ���������ڵ�ģ��.
	unsigned long CBaseRenderShape::CreateSceneNode()
	{
		if(m_pSceneNode)
		{
			return 0;
		}

		if(m_pParentSceneNode)
		{
			 m_pSceneNode = (SceneNode*)m_pParentSceneNode->createChildSceneNode();
			 if(NULL == m_pSceneNode)
			 {
				 // ����ʧ��.
				 return 0xffffffff;
			 }
		}

		// �����ɹ�.
		return 0;
	}


	CBaseRenderShape::CBaseRenderShape(void)
	{
		m_pParentSceneNode = NULL;
		m_pSceneNode       = NULL;		
		m_pRectRenderShape = NULL;
		m_pSceneManipulator= NULL;
	}

	CBaseRenderShape::~CBaseRenderShape(void)
	{
		ReleaseRenderData();

        if (m_pRectRenderShape)
        {
            delete m_pRectRenderShape;
            m_pRectRenderShape = NULL;
        }
	}

	// �����Ⱦ����
	void CBaseRenderShape::ReleaseRender()
	{
		//m_pParentSceneNode  = NULL;		// ������ʾ����༭���ܵĽڵ�.
		//m_pSceneNode        = NULL;		// ��ĳ����ڵ�.
		//m_pRectRenderShape  = NULL;		// ��ʾ��Ϣ.
		//m_pSceneManipulator = NULL;		// ������ogre�����ľ����

	}

	// ���һ����
	void CBaseRenderShape::AddNewPoint(Ogre::Vector3 point)
	{
		if(m_pRectRenderShape)
		{
			m_pRectRenderShape->AddNewPoint(point, false);
		}
	}

	// ������ʵ��
	unsigned long CBaseRenderShape::CreateRenderShape()
	{
		if(m_pRectRenderShape)
		{
			delete m_pRectRenderShape;
			m_pRectRenderShape = NULL;
		}

		m_pRectRenderShape = new CCustomShape;
		if(m_pRectRenderShape)
		{
			m_pRectRenderShape->setQueryFlags(0);
			if(m_pSceneNode)
			{
				m_pSceneNode->attachObject(m_pRectRenderShape);
			}

			//m_pRectRenderShape->FillPointData();
		}

		//m_pLineShape->setMaterial(_createIntersectGridMaterial()->getName()); 
		//m_AreaShape.setMaterial(_createIntersectGridMaterial()->getName()); 
		//m_AreaShape.setMaterial("Region/AreaRegionSel");

		return 0;
	}

	// �����ʾ����״.
	void CBaseRenderShape::ClearRenderShape()
	{
		if(m_pRectRenderShape)
		{
			m_pRectRenderShape->m_pointArray.clear();
		}
	}

	void CBaseRenderShape::ReleaseEntity()
	{
		if((m_pRectRenderShape)&&(m_pSceneNode))
		{
			m_pSceneNode->detachObject(m_pRectRenderShape->getName());
	
		}
	}

	void CBaseRenderShape::ReleaseSceneNode()
	{
		if((m_pParentSceneNode)&&(m_pSceneNode))
		{
			m_pParentSceneNode->removeAndDestroyChild(m_pSceneNode->getName());
		}
	}

	void CBaseRenderShape::ReleaseRenderData()
	{

		ClearRenderShape();

		ReleaseEntity();
		ReleaseSceneNode();
		if(m_pRectRenderShape)
		{
			delete m_pRectRenderShape;
			m_pRectRenderShape = NULL;
		}
	}

			

}
