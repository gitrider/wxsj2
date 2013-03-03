
/** ObjEntityDummy.cpp
 */

#include "StdAfx.h"
#include "ObjEntityDummy.h"
#include "../../Engine/EngineInterface.h"
#include "Global.h"
#include "GIException.h"
#include "Core/FairySystem.h"
#include <OgreSceneManager.h>
#include "ObjEntityProjTex.h"
#include "EffectSystem/FairyEffectManager.h"
#include "EffectSystem/FairyEffect.h"
#include "EffectSystem/FairyEffectElement.h"
#include "EffectSystem/FairyProjectorElement.h"



CObjEntityDummy::CObjEntityDummy()
{
	// �����հ׽ڵ㣬���Ը�����Ч����ProjTexture
	m_pSceneNode = CEngineInterface::GetMe()->GetFairySystem()->getBaseSceneNode()->createChildSceneNode();

	m_pEffect = NULL;
}

CObjEntityDummy::~CObjEntityDummy()
{
	if(m_pSceneNode)
	{
		if( m_pEffect != NULL )
		{
			Fairy::EffectManager::getSingleton().removeEffect(m_pEffect);
			m_pEffect = NULL;
		}

		CEngineInterface::GetMe()->GetFairySystem()->getSceneManager()->destroySceneNode(m_pSceneNode->getName());
	}
}


VOID CObjEntityDummy::Destroy(VOID)
{
	// ɾ��ͶӰ
	for(INT i=0; i<(INT)m_vProjTex.size(); i++)
	{
		delete m_vProjTex[i];
		m_vProjTex[i] = NULL;
	}

	m_vProjTex.clear();

	// ɾ������
	CObjectEntity::Destroy();
}

//����λ�� (��Ϸ��������)
VOID CObjEntityDummy::SetPosition(const fVector3& vPos)
{
	if(!m_pSceneNode)
		return;

	//����ת��
	fVector3 fvGfx;
	CEngineInterface::GetMe()->Axis_Trans(CEngineInterface::AX_GAME, vPos, CEngineInterface::AX_GFX, fvGfx);

	m_pSceneNode->setPosition(fvGfx.x, fvGfx.y, fvGfx.z);
}

// ������ת
VOID CObjEntityDummy::SetOrientation(const fVector3& vRotate)
{
	//Todo ...	
	if(!m_pSceneNode)
		return;

	m_pSceneNode->setOrientation( Ogre::Quaternion(Ogre::Radian(vRotate.y), Ogre::Vector3::UNIT_Y) );
}

// �����Ƿ�ɼ�
VOID CObjEntityDummy::SetVisible(BOOL bVisible)
{
	KLThrow("Not support!");
}

// ����Ͷ������
VOID CObjEntityDummy::Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range, FLOAT fHeight)
{
	// ��λ���е�ProjTex
	CObjEntityProjTex* pShadowProj = NULL;
	for(INT i=0; i<(INT)m_vProjTex.size(); i++)
	{
		if(m_vProjTex[i]->GetType() == type)
		{
			pShadowProj = m_vProjTex[i];
			break;
		}
	}
	
	if(Ring_Range != 100.0f && m_pEffect)
	{
		for(unsigned short i=0; i < m_pEffect->getNumElements(); i++)
		{
			Fairy::EffectElement *eff_element = m_pEffect->getElement(i);

			if(eff_element->getType() == "Projector")
			{
				static_cast<Fairy::ProjectorElement*>(eff_element)->setProjectionSize(Ring_Range);
			}
		}
	}
	
	// �߶�ת�� GAME->GFX
	// ȡ�÷���ϵ��
	fVector3 fvScale = CEngineInterface::GetMe()->GetSacle();
	fHeight = fHeight*fvScale.y;

	if(bShow)
	{
		// �Ѿ�����
		if(pShadowProj)
		{
			pShadowProj->Show(bShow);
			pShadowProj->SetAddHeight(fHeight);
			return;
		}

		// ����
		pShadowProj = new CObjEntityProjTex(type, m_pSceneNode, fHeight);

		m_vProjTex.push_back(pShadowProj);
	}
	else
	{
		// �Ѿ�����
		if(pShadowProj)
		{
			pShadowProj->Show(bShow);
			pShadowProj->SetAddHeight(fHeight);
			return;
		}
	}

}

// ����Ͷ������
VOID CObjEntityDummy::Attach_ProjTexture(const char* effectName, BOOL bShow, FLOAT Ring_Range, FLOAT fHeight)
{
	KLAssert(effectName);
	// ��λ���е�ProjTex
	CObjEntityProjTex* pShadowProj = NULL;
	for(INT i=0; i<(INT)m_vProjTex.size(); i++)
	{
		if(m_vProjTex[i]->GetType() == AURA_RUNE)
		{
			pShadowProj = m_vProjTex[i];
			break;
		}
	}

	if(Ring_Range != 100.0f && m_pEffect)
	{
		for(unsigned short i=0; i < m_pEffect->getNumElements(); i++)
		{
			Fairy::EffectElement *eff_element = m_pEffect->getElement(i);

			if(eff_element->getType() == "Projector")
			{
				static_cast<Fairy::ProjectorElement*>(eff_element)->setProjectionSize(Ring_Range);
			}
		}
	}

	// �߶�ת�� GAME->GFX
	// ȡ�÷���ϵ��
	fVector3 fvScale = CEngineInterface::GetMe()->GetSacle();
	fHeight = fHeight*fvScale.y;

	if(bShow)
	{
		// �Ѿ�����
		if(pShadowProj)
		{
			pShadowProj->ChangeEffect(effectName, m_pSceneNode, fHeight);
			pShadowProj->Show(bShow);
			//pShadowProj->SetAddHeight(fHeight);
			return;
		}

		// ����
		pShadowProj = new CObjEntityProjTex(effectName, m_pSceneNode, fHeight);

		m_vProjTex.push_back(pShadowProj);
	}
	else
	{
		// �Ѿ�����
		if(pShadowProj)
		{
			pShadowProj->Show(bShow);
			pShadowProj->SetAddHeight(fHeight);
			return;
		}
	}

}

VOID CObjEntityDummy::Attach_Effect(LPCTSTR szEffectName)
{
	if( m_pEffect != NULL )
	{
		assert( m_pEffect == NULL && "CObj_Dummy::Attach_Effect" );
		Detach_Effect();
	}

	// �����µ���Ч
	m_pEffect = Fairy::EffectManager::getSingleton().createEffect(szEffectName);
	if( m_pEffect != NULL )
	{
		// �ҽӵ������ڵ�
		//m_pSceneNode->addChild(m_pEffect->createSceneNode());
		m_pEffect->createSceneNode(m_pSceneNode);
	}
}

VOID CObjEntityDummy::Detach_Effect(void)
{
	if( m_pSceneNode != NULL )
	{
		if( m_pEffect != NULL )
		{
			Fairy::EffectManager::getSingleton().removeEffect(m_pEffect);
			m_pEffect = NULL;
		}
	}
}
