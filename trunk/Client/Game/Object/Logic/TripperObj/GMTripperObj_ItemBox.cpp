
#include "StdAfx.h"
#include "GMTripperObj_ItemBox.h"
#include "Global.h"
#include "NetWork/NetManager.h"
#include "../../Manager/ObjectManager.h"
#include "../Character/Obj_PlayerMySelf.h"
#include "Procedure/GameProcedure.h"
#include "CGOpenItemBox.h"



KL_IMPLEMENT_DYNAMIC(CTripperObject_ItemBox, GETCLASS(CTripperObject));


CTripperObject_ItemBox::CTripperObject_ItemBox()
{
}

CTripperObject_ItemBox::~CTripperObject_ItemBox()
{
}


VOID CTripperObject_ItemBox::Initial(VOID*)
{
	if( m_pRenderInterface == NULL )
	{
		// ������Ⱦ��ʵ��
		m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_ACTOR);

		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("Obj_ItemBox_Info_MuXiang");
		m_pRenderInterface->Actor_SetFile(strTemp.c_str());

		// ����ѡ�����ȼ�
		m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_ITEMBOX);

		m_pRenderInterface->SetData(GetID());
	}
}

BOOL CTripperObject_ItemBox::Tripper_CanOperation(VOID) const
{ 
	if(CObjectManager::GetMe()->GetMySelf()->GetServerGUID() == m_idOwner)
		return TRUE;
	else
		return FALSE;
}

VOID CTripperObject_ItemBox::Release( VOID )
{
	if( m_pRenderInterface != NULL )
	{
		m_pRenderInterface->Destroy();
		m_pRenderInterface = NULL;
	}
	CObject_Static::Release();
}

VOID CTripperObject_ItemBox::Tripper_Active(VOID)
{
	Packets::CGOpenItemBox msg;
	msg.setObjID(this->GetServerID());

	CNetManager::GetMe()->SendPacket(&msg);
}

