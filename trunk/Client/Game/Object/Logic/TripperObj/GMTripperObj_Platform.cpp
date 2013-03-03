
#include "StdAfx.h"
#include "GMTripperObj_Platform.h"
#include "Global.h"
#include "../../Manager/ObjectManager.h"
#include "Procedure/GameProcedure.h"
#include "GIDBC_Struct.h"
#include "DBC/GMDataBase.h"
#include "NetWork/NetManager.h"
#include "GIException.h"
#include "CGCharDefaultEvent.h"



KL_IMPLEMENT_DYNAMIC(CTripperObject_Platform, GETCLASS(CTripperObject));


CTripperObject_Platform::CTripperObject_Platform()
{
//	m_pRenderInterface2 = NULL;
}

CTripperObject_Platform::~CTripperObject_Platform()
{
}


//�ܷ�������
BOOL CTripperObject_Platform::Tripper_CanOperation(VOID) const
{
	return (m_pPlatformDef->nClickType>0);
}

VOID CTripperObject_Platform::Initial(VOID*)
{

	if ( m_pRenderInterface == NULL )
	{
		//������Ⱦ��ʵ��
		m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_MESH);
		m_pRenderInterface->StaObj_SetMeshFile(m_pPlatformDef->szMeshFile);

		m_pRenderInterface->SetData(GetID());

		//ѡ�����ȼ�
		m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_PLATFORM);
	}
}


VOID CTripperObject_Platform::SetPosition(const fVector3& fvPosition) 
{ 
	CObject_Static::SetPosition(fvPosition);
	//����Ⱦ��ͬ��λ��
//	if(m_pRenderInterface2)
//		m_pRenderInterface2->SetPosition( GetPosition() );
}

VOID CTripperObject_Platform::Release( VOID )
{
	if ( m_pRenderInterface != NULL )
	{
		m_pRenderInterface->Destroy();
		m_pRenderInterface = NULL;

//		m_pRenderInterface2->Destroy();
//		m_pRenderInterface2 = NULL;
	}
	CObject_Static::Release();
}

BOOL CTripperObject_Platform::SetPlatformID(INT nID)
{
	DBC_DEFINEHANDLE(pDBC, DBC_LIFEABILITY_PLATFORM);
	m_pPlatformDef = (const _DBC_LIFEABILITY_PLATFORM*)pDBC->Search_Index_EQU(nID);

	if(!m_pPlatformDef) return FALSE;

	return true;

}
INT CTripperObject_Platform::GetPlatformID(VOID)
{
	return m_pPlatformDef->nType;
}

//���뼤��
VOID CTripperObject_Platform::Tripper_Active(VOID)
{
	Packets::CGCharDefaultEvent Msg;
	Msg.setObjID( GetServerID() );

	CNetManager::GetMe()->SendPacket(&Msg);
}
