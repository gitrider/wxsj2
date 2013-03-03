
#include "StdAfx.h"

#include "ObjEntityActor.h"

#include "Global.h"
#include "../../Engine/EngineInterface.h"
#include "ObjEntityProjTex.h"

#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreLogmanager.h>

#include "Core/FairySystem.h"
#include "Core/FairyObjectFactoryManager.h"
#include "Core/FairyLogicModelObject.h"
#include "Core/FairyVariant.h"
#include "Core/FairyFlags.h"

#include "ModelSystem/FairyLogicModel.h"
#include "EffectSystem/FairyEffectBasic.h"

#include "GIException.h"
#include "GIDBC_Struct.h"
#include "GIDataBase.h"
#include "GIException.h"
#include "GIKernel.h"
#include "GIWorldSystem.h"
#include "AxTrace.h"



#define FOBJ_ACTOR_FILE _T("logic model name")			// ����ģ����Դ��


CObjEntityActor::CObjEntityActor() : CObjectEntity()
{
	Fairy::System* pFairySystem = CEngineInterface::GetMe()->GetFairySystem();
	// ��Fairy�д���ʵ��
	m_pFairyObj = pFairySystem->getObjectFactoryManager()->createInstance(Fairy::String("LogicModel"));
	// ��ogre�д�����Ⱦ����
	m_pFairyObj->createRenderInstance( pFairySystem );
	// ��thisָ�뱣����FairyData��
	m_pFairyObj->setData((Fairy::ulong)this);	
	// �ɱ����ѡ��
	m_bRayQuery = TRUE;

	mModel = NULL;
	m_bVisible = TRUE;
	m_rightWeaponModel = NULL;
	m_leftWeaponModel = NULL;
}

CObjEntityActor::~CObjEntityActor()
{
}


VOID CObjEntityActor::Destroy(VOID)
{
    CObjectEntity::Destroy();
}

VOID CObjEntityActor::Actor_SetFile(LPCTSTR szActorFile)
{
	KLAssert(szActorFile&& m_pFairyObj);

	m_pFairyObj->setProperty(FOBJ_ACTOR_FILE, Fairy::String(szActorFile));

	//�����ԴLoad���
	mModel = ((Fairy::LogicModelObject*)m_pFairyObj.get())->getLogicModel();

	KLAssert(mModel);
}

VOID CObjEntityActor::SetVisible(BOOL bVisible)
{
	//ȷ����Դ�Ѿ�Load
	if(!mModel) return;

    mModel->setVisible(bVisible==TRUE);
	
	m_bVisible = bVisible;
}

VOID CObjEntityActor::Actor_SetObjectProperty(LPCTSTR szName, LPCTSTR szValue)
{
	KLAssert(m_pFairyObj);

	//ȷ����Դ�Ѿ�Load
	if( !mModel )
		return;

	try
	{
		mModel->changeAttribute(szName, szValue);
	}
	catch(...)
	{
		int a=0;
	}
}

VOID CObjEntityActor::Actor_GetObjectProperty(LPCTSTR szName, STRING& szValue)
{
	KLAssert(m_pFairyObj);

	//ȷ����Դ�Ѿ�Load
	if(!mModel) return;

    mModel->getAttribute(szName, szValue);
}

VOID CObjEntityActor::Actor_GetLocator(LPCTSTR szName, fVector3& fvPosition)
{
	KLAssert(m_pFairyObj);

	//ȷ����Դ�Ѿ�Load
	if(!mModel) return;

	Ogre::Matrix4 mx;
	bool bResult = mModel->getLocatorWorldTransform(szName, mx);
	if(bResult)
	{
		fVector3 fvGfx(mx[0][3], mx[1][3], mx[2][3]);

		CEngineInterface::GetMe()->Axis_Trans(CEngineInterface::AX_GFX, fvGfx, CEngineInterface::AX_GAME, fvPosition);
	}
}


inline float SMOOTH(float f1, float f2, float fper, float max_f)
{
	if(abs(f1-f2) > max_f)
	{
		return (int)(f2+0.5f);
	}

	return (int)((float)f1+(f2-f1)*(float)fper+0.5f);
}


BOOL CObjEntityActor::Actor_GetInfoBoardPos(fVector2& fvPos, const fVector3 *pvObjPos,const float fObligeHeight)
{
	// ȷ����Դ�Ѿ�Load
	if(!mModel) 
		return FALSE;

	Ogre::Vector3 vPos;
	if ( pvObjPos != NULL )
	{
		// ����ת��
		fVector3 fvGfxPos;
		CEngineInterface::GetMe()->Axis_Trans(CEngineInterface::AX_GAME, *pvObjPos, CEngineInterface::AX_GFX, fvGfxPos);

		vPos.x = fvGfxPos.x;
		vPos.y = fvGfxPos.y;
		vPos.z = fvGfxPos.z;
	}
	else
	{
		vPos =  mModel->getSceneNode()->_getDerivedPosition();
	}
	
	// ������λ��
	FLOAT s_fAddOffset = 0.1 * CEngineInterface::GetMe()->GetSacle().y;
	FLOAT s_fAddHeight = abs(mModel->getBoundingBox().getMaximum().y - mModel->getBoundingBox().getMinimum().y );
	
//	if(fObligeHeight > 0)
//		vPos.y += (fObligeHeight + s_fAddOffset);


	// ʹ�����ݱ���ָ���߶�
	if(fObligeHeight > 0)
		vPos.y += fObligeHeight * CEngineInterface::GetMe()->GetSacle().y;
	else
	// ʹ��Boundbox�߶�
		vPos.y += abs(mModel->getBoundingBox().getMaximum().y - mModel->getBoundingBox().getMinimum().y );

	// ����0.1m
	vPos.y += 0.1 * CEngineInterface::GetMe()->GetSacle().y;
	
	fVector3 fvGfx(vPos.x, vPos.y, vPos.z);
	fVector3 fvScreen;
	BOOL bVisible = CEngineInterface::GetMe()->Axis_Trans(CEngineInterface::AX_GFX, fvGfx, CEngineInterface::AX_SCREEN, fvScreen);
	
	if(!bVisible) 
		return FALSE;

	//ƽ������
	fvPos.x = SMOOTH(fvPos.x, fvScreen.x, 0.8f, 3.f);
	fvPos.y = SMOOTH(fvPos.y, fvScreen.y, 0.8f, 3.f);

	return TRUE;	
}

VOID CObjEntityActor::StopCurrentAction()
{
	if(mModel)
		mModel->stopCurrentAction();
}

BOOL CObjEntityActor::Actor_EnterSkill(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop, FLOAT fFuseParam, INT nFlag, 
									   INT nObjType, INT nLogicTag)
{
	// ȷ����Դ�Ѿ�Load
	if( !mModel )
		return FALSE;

	// �·����� �����ں�ʹ��
	//if( 0 == nObjType )
	{
	//	mModel->createSkill(szSkillName,  bLoop==TRUE, bAnim==TRUE, fFuseParam);

		// �����±������by czg 200911-30
		// mModel->GetSkillHelper().createSkill( szSkillName, nFlag, (bool)bLoop, (bool)bAnim, 0 );	// , nLogicTag
	}
	// �ɷ����� ����ʹ��
	//else
	{
		mModel->createSkill(szSkillName, bLoop==TRUE, FALSE, fFuseParam);
		//char szBuff[256];//ע�͵����Skill����,��ʱ������δ���
		//sprintf(szBuff,"Skill %s\n",szSkillName);
		//OutputDebugStringA(szBuff);
	}	 

	return TRUE;
}

Fairy::Skill* CObjEntityActor::GetCurrentSkill()
{
	//return mModel->GetSkillHelper().getCurrentSkill();
	return mModel->getCurrentSkill();
}



// �л������Ĳ����ٶ�
// fRate		- ���ű���
VOID CObjEntityActor::Actor_ChangeActionRate(FLOAT fRate)
{
	if(!mModel) return;

	mModel->setGlobalAnimationRate(fRate);
}

// ����ȱʡ����
VOID CObjEntityActor::Actor_SetDefaultAnim(LPCTSTR szAnimName)
{
	// ȷ����Դ�Ѿ�Load
	if(!mModel) return;

	//mModel->SetDefaultAnimation(szAnimName);
}

VOID CObjEntityActor::Actor_SetTransparent(FLOAT fTransparency, FLOAT fTime)
{
	if(!mModel) return;

	mModel->setTransparent(fTime, fTransparency);
}

VOID CObjEntityActor::Actor_SetEffectExtraTransformInfos(UINT uEffect, fVector3& fvPosition)
{
	if(!mModel) return;

	Fairy::Effect *pEffect = mModel->getEffect(uEffect);
	if(pEffect != NULL)
	{
		Fairy::TransformInfos tempTransformInfos;
		fVector3 fvGfxPos;
		
		CEngineInterface::GetMe()->Axis_Trans(CEngineInterface::AX_GAME, fvPosition, CEngineInterface::AX_GFX, fvGfxPos);
		Fairy::TransformInfo tempTransformInfo(Ogre::Vector3(fvGfxPos.x, fvGfxPos.y, fvGfxPos.z));
		
		tempTransformInfos.push_back(tempTransformInfo);
		
		pEffect->setExtraTransformInfos(tempTransformInfos);
	}
}

VOID CObjEntityActor::Actor_SetHairColor(UINT HairColor)
{
	// ȷ����Դ�Ѿ�Load
	if(!mModel)
		return;

	Ogre::ColourValue color;
	color.r = (HairColor >> 24)/ 255.0;
	color.g = ((HairColor >> 16) & 0xFF)/255.0;
	color.b = ((HairColor >> 8) & 0xFF)/255.0;
	color.a = (HairColor & 0xFF)/255.0;

	mModel->setHairColour(color);
}

//���ý�ɫ��������֪ͨ
void CObjEntityActor::SetAnimationEndEvent(FUNC_ONANIMATIONENDEX pFunc, DWORD dwParam, INT n)
{
	// ȷ����Դ�Ѿ�Load
	if(!mModel) 
		return;

	mModel->SetAnimationFinishListener( pFunc, dwParam );
}

void CObjEntityActor::SetAnimationCanBreakEvent(FUNC_ONSKILLBREAKTIME pFunc, DWORD dwParam)
{
	// ȷ����Դ�Ѿ�Load
	if(!mModel) 
		return;

	mModel->SetSkillBreakTimeListener( pFunc, dwParam );
}

void CObjEntityActor::SetAnimationHitEvent(FUNC_ONSKILLHITTIME pFunc, DWORD dwParam)
{
	// ȷ����Դ�Ѿ�Load
	if(!mModel) return;

	mModel->SetSkillHitTimeListener( pFunc, dwParam );
}

void CObjEntityActor::SetAnimationShakeEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam)
{
	// ȷ����Դ�Ѿ�Load
	if(!mModel) return;

	mModel->SetSkillShakeTimeListener(pFunc, dwParam );
}


//��Ч���
UINT CObjEntityActor::AddEffect( LPCSTR effectName, LPCSTR locatorName )
{
	if(!mModel)
		return NULL;

	return mModel->addEffect( Ogre::String(effectName), Ogre::String(locatorName) );
}

VOID CObjEntityActor::DelEffect( UINT effect )
{
	if(!mModel)
		return ;

	mModel->delEffect( effect );
}

VOID CObjEntityActor::DelAllEffect( VOID )
{
	if(!mModel)
		return ;

	mModel->delAllEffect( );
}

VOID CObjEntityActor::Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range, FLOAT fHeight )
{
    if( !mModel )
		return;

	bool show = ( (bShow == TRUE) && (Ring_Range >= 0.0f) );
	
    switch (type)
    {
    case SELECT_RING:
        {
            mModel->addProjector( Fairy::MPT_SELECTED, "selected_projector", show, Ring_Range );
            break;

			// ѡ�񻷵����ࣺ NPC�� �֣� �������
//			switch(  )
//			{
//			case NPC:
//				break;
//			case ��:
//				break;
//			case �������:
//				break;
//			};
        }

    case SHADOW_BLOB:
        {
			mModel->addProjector( Fairy::MPT_SHADOW, "shadow_projector", show, Ring_Range );
            break;
        }
    }
}

void CObjEntityActor::Attach_Object(tEntityNode *pObject, LPCTSTR szAttachLocator)
{
	if ( mModel != NULL && pObject!= NULL && pObject->GetType() == ETYPE_ACTOR && szAttachLocator != NULL )
	{
		CObjEntityActor *pRFObject = (CObjEntityActor*)pObject;
		Fairy::LogicModel* pActorImpl = pRFObject->GetActorImpl();
		if ( pActorImpl != NULL )
		{
			mModel->attachModel( szAttachLocator, pActorImpl );
		}
	}
}

void CObjEntityActor::Detach_Object(tEntityNode *pObject)
{
	if ( mModel != NULL && pObject!= NULL && pObject->GetType() == ETYPE_ACTOR )
	{
		CObjEntityActor *pRFObject = (CObjEntityActor*)pObject;
		Fairy::LogicModel* pActorImpl = pRFObject->GetActorImpl();
		if ( pActorImpl != NULL )
		{
			mModel->detachModel( pActorImpl );
		}
	}
}

//����VisibleFlag
VOID CObjEntityActor::Actor_SetUIVisibleFlag(VOID)
{
	//ȷ����Դ�Ѿ�Load
	if(!mModel) return;

	mModel->setVisibleFlag((unsigned int)Fairy::OVF_GUI_ELEMENTS);
	mModel->changeAttribute("MovementType", "Normal");
}

VOID CObjEntityActor::SetPosition(const fVector3& vPos)
{
	KLAssert(m_pFairyObj);

	// ȷ����Դ�Ѿ�Load
	if(!mModel) 
		return;

	// ����ת��
	fVector3 fvGfxPos;
	CEngineInterface::GetMe()->Axis_Trans(CEngineInterface::AX_GAME, vPos, CEngineInterface::AX_GFX, fvGfxPos);

    mModel->setPosition(Ogre::Vector3(fvGfxPos.x, fvGfxPos.y, fvGfxPos.z));
}

VOID CObjEntityActor::SetOrientation(const fVector3& vRotate)
{
	KLAssert(m_pFairyObj);

	if(!mModel) 
		return;

	Ogre::Quaternion qu(Ogre::Radian(vRotate.y), Ogre::Vector3::UNIT_Y);
	mModel->setOrientation(qu);
}

VOID CObjEntityActor::Actor_SetMouseHover(BOOL bHover)
{
	KLAssert(m_pFairyObj);

	if(!mModel) 
		return;

	mModel->setSelected(bHover==TRUE);
}

VOID CObjEntityActor::SetLeftWeaponEffect(LPCTSTR szEffectName, LPCSTR szLocatorName, UINT color)
{
	Ogre::ColourValue colour;

	colour.a = ((FLOAT)((color & 0xFF)))/255.0f;
	colour.r = (((FLOAT)(color >> 24))/255.0f) * colour.a;
	colour.g = (((FLOAT)(( color >> 16 ) & 0xFF))/255.0f) *colour.a;
	colour.b = (((FLOAT)((color >> 8) & 0xFF))/255.0f) * colour.a;
	
 
	m_leftWeaponModel = Fairy::VariantCast<Fairy::LogicModel*>(mModel->getAttribute("LeftWeaponObj"));
	if( m_leftWeaponModel )
	m_leftWeaponModel->delAllEffect();
	

    if( m_leftWeaponModel )
    {
		if( (NULL == szEffectName)  || (0 == strcmp(szEffectName, "" )) ||
			(NULL == szLocatorName) || (0 == strcmp(szLocatorName, "")) )
			return ;

		if( color != 0 )
		{
			UINT leftWeaponEffect = m_leftWeaponModel->addEffect( szEffectName, szLocatorName, Fairy::LogicModel::GTIT_ALL );	// "������Ч��"
			Fairy::Effect* effect = m_leftWeaponModel->getEffect(leftWeaponEffect);

			if( effect )
			{
//				effect->setColour(colour);
			}
		}
	}
}

VOID CObjEntityActor::SetRightWeaponEffect(LPCTSTR szEffectName, LPCSTR szLocatorName, UINT color)
{
	Ogre::ColourValue colour;

	colour.a = ((FLOAT)((color & 0xFF)))/255.0f;
	colour.r = (((FLOAT)(color >> 24))/255.0f)*colour.a;
	colour.g = (((FLOAT)(( color >> 16 ) & 0xFF))/255.0f)*colour.a;
	colour.b = (((FLOAT)((color >> 8) & 0xFF))/255.0f)*colour.a;
	
  
	m_rightWeaponModel = Fairy::VariantCast<Fairy::LogicModel*>(mModel->getAttribute("RightWeaponObj"));
	 if( m_rightWeaponModel )
		m_rightWeaponModel->delAllEffect();

    if( m_rightWeaponModel )
    {
		if( (NULL == szEffectName)  || (0 == strcmp(szEffectName, "" )) ||
			(NULL == szLocatorName) || (0 == strcmp(szLocatorName, "")) )
			return ;

		if( color != 0 )
		{
			UINT rightWeaponEffect = m_rightWeaponModel->addEffect( szEffectName, szLocatorName, Fairy::LogicModel::GTIT_ALL );	// "������Ч��"
			Fairy::Effect* effect = m_rightWeaponModel->getEffect( rightWeaponEffect );

			if( effect )
			{
//				effect->setColour(colour);
			}
		}
	}
}

VOID CObjEntityActor::ClearWeaponEffect( eWEAPATTR eWeaponAttr )
{
	switch( eWeaponAttr )
	{
	case WEAP_RIGHT:
		m_rightWeaponModel =Fairy::VariantCast<Fairy::LogicModel*>( mModel->getAttribute("RightWeaponObj") );
		if( m_rightWeaponModel )
		{
			m_rightWeaponModel->delAllEffect();
		}
		break;

	case WEAP_LEFT:
		m_leftWeaponModel = Fairy::VariantCast<Fairy::LogicModel*>( mModel->getAttribute("LeftWeaponObj") );
		if( m_leftWeaponModel )
		{
			m_leftWeaponModel->delAllEffect();
		}
		break;

	case WEAP_LEFT_SHIELD:
		break;
	};

}



// �ı������Ķ���
VOID CObjEntityActor::ChangeWeaponAction( eWEAPATTR szWeaponAttrType, LPCSTR lpszActionName, BOOL bLoop, FLOAT fFuseParam )	
{
	if( !mModel )
	{
		return;
	}
		
	Fairy::LogicModel* pModel = NULL;

	// ��⶯������Ч��
	if( NULL == lpszActionName )
	{
		return ;
	}

	// �������ͻ�ȡ��Ӧ������ģ��ʵ��
	switch( szWeaponAttrType )
	{
	case WEAP_RIGHT:
		pModel = Fairy::VariantCast<Fairy::LogicModel*>( mModel->getAttribute("RightWeaponObj") );
		break;
	case WEAP_LEFT:
		pModel = Fairy::VariantCast<Fairy::LogicModel*>( mModel->getAttribute("LeftWeaponObj") );
		break;
	case WEAP_LEFT_SHIELD:
		pModel = Fairy::VariantCast<Fairy::LogicModel*>( mModel->getAttribute("LeftShieldObj") );
		break;
	case WEAP_NONE:
		break;
	};	// weitch end
	
	// �������ģ�ͣ������䶯��
	if( pModel )
	{
		pModel->createSkill( lpszActionName, bLoop==TRUE, TRUE, fFuseParam );
	}

}

VOID CObjEntityActor::ChangeWeaponActionSpeed( eWEAPATTR szWeaponAttrType, FLOAT fRate )
{
	if( !mModel )
		return;

	Fairy::LogicModel* pModel = NULL;

	switch( szWeaponAttrType )
	{
	case WEAP_RIGHT:
		pModel = Fairy::VariantCast<Fairy::LogicModel*>( mModel->getAttribute("RightWeaponObj") );
		break;
	case WEAP_LEFT:
		pModel = Fairy::VariantCast<Fairy::LogicModel*>( mModel->getAttribute("LeftWeaponObj") );
		break;
	case WEAP_LEFT_SHIELD:
		pModel = Fairy::VariantCast<Fairy::LogicModel*>( mModel->getAttribute("LeftShieldObj") );
		break;
	case WEAP_NONE:
		break;
	};	// weitch end

	if( pModel )
		pModel->setGlobalAnimationRate( fRate );
}


int CObjEntityActor::GetTerrainSoundType()
{
	if(mModel != NULL)
	{
		return mModel->_getTerrainSoundType();
	}

	return -1;
}

void CObjEntityActor::SetScaleFactor(float scaleFactorX, float scaleFactorY, float scaleFactorZ)
{
	if(mModel != NULL)
	{
		return mModel->setScaleFactor(Ogre::Vector3(scaleFactorX, scaleFactorY, scaleFactorZ));
	}

}

VOID CObjEntityActor::SetGfxPosition(const fVector3& vPos)
{
	KLAssert(m_pFairyObj);
	//ȷ����Դ�Ѿ�Load
	if(!mModel) return;

	mModel->setPosition(Ogre::Vector3(vPos.x, vPos.y, vPos.z));
}

VOID CObjEntityActor::Actor_PlayAnimation(LPCTSTR szAnimationName, FLOAT fStartTime, FLOAT fWeight)
{
	if(!mModel) return;

	mModel->playAnimation(szAnimationName, fStartTime, fWeight);
}

VOID CObjEntityActor::Actor_ChangeActionLoop(bool bLoop)
{
	if(!mModel) return;

	mModel->setAnimationLoop(bLoop);
}

VOID	CObjEntityActor::AddAreaEffect(LPCSTR templateName)
{
	if(!mModel) return;

	mModel->addAreaEffect(templateName);
}
bool	CObjEntityActor::RemoveAreaEffect(LPCSTR templateName)
{
	if(!mModel) return false;

	return mModel->removeAreaEffect(templateName);
}