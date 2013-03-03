#pragma once

#include "LogicDataStructDef.h"
#include "Core/FairyPrerequisites.h"
#include <map>
#include <OgreMaterial.h>
#include <OgreVector3.h>
#include "CustomShape.h"

namespace Fairy 
{
	class SceneManipulator;
	
	class CBaseRenderShape
	{

	public:
		CBaseRenderShape(void);
		~CBaseRenderShape(void);

	public:

			Ogre::SceneNode*	m_pParentSceneNode;		// ������ʾ����༭���ܵĽڵ�.
			Ogre::SceneNode*	m_pSceneNode;			// ��ĳ����ڵ�.
			
			CCustomShape*		m_pRectRenderShape;		// ��ʾ��Ϣ.
			SceneManipulator*   m_pSceneManipulator;	// ������ogre�����ľ����

			
	public:
			
			/////////////////////////////////////////////////////////////////////////////////
			//
			// ����������
			//

			// ���ø������ڵ�.
			void SetParentSceneNode(Ogre::SceneNode* pSceneNode);

			// ����SceneManipulator���
			void SetSceneManipulator(SceneManipulator*  SceneManipulator);

			// ���������ڵ�ģ��.
			unsigned long CreateSceneNode();

			// �����Ⱦ����
			void ReleaseRender();

			// ���һ����
			void AddNewPoint(Ogre::Vector3 point);

			// ������ʵ��
			unsigned long CreateRenderShape();

			// �����ʾ����״.
			void ClearRenderShape();

			void ReleaseEntity();

			void ReleaseSceneNode();

			void ReleaseRenderData();
			
	};


}
