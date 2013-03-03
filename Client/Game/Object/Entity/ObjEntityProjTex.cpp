
/** 
 */

#include "StdAfx.h"
#include "ObjEntityProjTex.h"
#include "../../Engine/EngineInterface.h"
#include "GIException.h"
#include "EffectSystem/FairyEffect.h"
#include "EffectSystem/FairyEffectManager.h"



CObjEntityProjTex::CObjEntityProjTex(tEntityNode::PROJTEX_TYPE type, Ogre::SceneNode* pParentNode, FLOAT fAddHeight)
{
    KLAssert(pParentNode);
    mProjectorEffect = NULL;

    Ogre::String effectName;
	switch(type)
	{
	case tEntityNode::REACHABLE:
        effectName = "reachable_projector";
		break;

	case tEntityNode::UNREACHABLE:
        effectName = "unreachable_projector";
		break;

	case tEntityNode::AURA_RUNE:
        effectName = "aura_rune_projector";
		break;

	case tEntityNode::BLOOD_STAIN_1:
		effectName = "blood_stain_1_projector";
		break;

	case tEntityNode::BLOOD_STAIN_2:
		effectName = "blood_stain_2_projector";
		break;

	case tEntityNode::BLOOD_STAIN_3:
		effectName = "blood_stain_3_projector";
		break;

	case tEntityNode::BLOOD_STAIN_4:
		effectName = "blood_stain_4_projector";
		break;

	default:
		KLThrow("Invalid ProjTextType!");
		break;
	}

    mProjectorEffect = Fairy::EffectManager::getSingleton().createEffect(effectName);
    mProjectorEffect->createSceneNode(pParentNode);

	m_theType = type;

	m_effectName = effectName;
	
	// 设置高度
	SetAddHeight(fAddHeight);
}

//直接通过一个特效构造
CObjEntityProjTex::CObjEntityProjTex(const CHAR* effectName, Ogre::SceneNode* pParentNode, FLOAT fAddHeight/*=170.0f*/)
{
	KLAssert(pParentNode);
	KLAssert(effectName);
	mProjectorEffect = NULL;

	mProjectorEffect = Fairy::EffectManager::getSingleton().createEffect(effectName);
	mProjectorEffect->createSceneNode(pParentNode);

	m_theType = tEntityNode::AURA_RUNE;

	m_effectName = effectName;

	// 设置高度
	SetAddHeight(fAddHeight);
}

CObjEntityProjTex::~CObjEntityProjTex()
{
    Fairy::EffectManager::getSingleton().removeEffect(mProjectorEffect);
    mProjectorEffect = NULL;
}


VOID CObjEntityProjTex::SetAddHeight(FLOAT fAddHeight)
{
	m_fAddHeight = fAddHeight;
}

VOID CObjEntityProjTex::Show(BOOL bShow)
{
    if( mProjectorEffect )
	{
        mProjectorEffect->setVisible(bShow==TRUE);
	}
}

void	CObjEntityProjTex::ChangeEffect(const char* effectName, Ogre::SceneNode* pParentNode, FLOAT fAddHeight)
{
	if (effectName && m_effectName != effectName)
	{
		if (mProjectorEffect)
		{
			Fairy::EffectManager::getSingleton().removeEffect(mProjectorEffect);
			mProjectorEffect = NULL;
		}
		mProjectorEffect = Fairy::EffectManager::getSingleton().createEffect(effectName);
		mProjectorEffect->createSceneNode(pParentNode);

		m_theType = tEntityNode::AURA_RUNE;

		m_effectName = effectName;
		// 设置高度
		SetAddHeight(fAddHeight);
	}
}
