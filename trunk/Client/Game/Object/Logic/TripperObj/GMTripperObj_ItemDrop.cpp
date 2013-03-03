
#include "StdAfx.h"
#include "GMTripperObj_ItemDrop.h"
#include "Global.h"
#include "NetWork/NetManager.h"
#include "../../Manager/ObjectManager.h"
#include "../Character/Obj_PlayerMySelf.h"
#include "Procedure/GameProcedure.h"
#include  "CGPickDropItem.h"
#include "..\..\..\DataPool\GMDataPool.h"
#include " ..\..\..\Item\Obj_Item.h"
#include "..\Game\Engine\EngineInterface.h"
//#include "..\..\Entity\ObjEntityActor.h"
#include "GIUtil.h"
 
KL_IMPLEMENT_DYNAMIC(CTripperObject_ItemDrop, GETCLASS(CTripperObject));

extern float SMOOTH(float f1, float f2, float fper, float max_f);

CTripperObject_ItemDrop::CTripperObject_ItemDrop()
{
	m_bCanPick = false;
	m_bCanEveryOnePick = false;
	m_pInfoBoard		= NULL;
	m_bUpdataBoard		= false;
}

CTripperObject_ItemDrop::~CTripperObject_ItemDrop()
{
   Release();
}

VOID	CTripperObject_ItemDrop::SetOwnerGUID(ObjID_t* nID)
{

	if(!nID)

		return ;

	for (int i = 0 ;i < MAX_TEAM_MEMBER ;++i)
	{
		m_idOwner[i] = nID[i];
	}
}


VOID CTripperObject_ItemDrop::Initial(VOID*)
{

	if ( !CDataPool::GetMe()->ItemDrop_GetItem(this->GetServerID()))
	{
		return ;
	} 

	if( m_pRenderInterface == NULL )
	{
		// ������Ⱦ��ʵ��
		m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_ACTOR);

	
		//m_sModel = NOCOLORMSGFUNC("Obj_ItemBox_Info_MuXiang"); //"0��Ʒ/����.obj"
 
		//  m_pRenderInterface->Actor_SetFile(m_sModel.c_str());
         if ( !CDataPool::GetMe()->ItemDrop_GetItem(this->GetServerID()))
			 return ;

		 LPCSTR  vs =CDataPool::GetMe()->ItemDrop_GetItem(this->GetServerID())->GetDropVisualID();

		 if ( !vs )
		 {
			  return ;
		 }
		 m_pRenderInterface->Actor_SetFile(vs);
	 
		

		// ����ѡ�����ȼ�
		m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_ITEMBOX);

		m_pRenderInterface->SetData(GetID());
		if ( m_bCanPick   )
		{
			//m_pRenderInterface->AddEffect("wq_huoyandao","�����󶨵�_04");
		}

		if(!GetFakeObjectFlag()&& CGameProcedure::s_pUISystem)
		{

			m_pInfoBoard = CGameProcedure::s_pUISystem->CreateCreatureBoard();
			m_pInfoBoard->SetElement_ObjId( this->GetServerID() );


			//��ɫ
			STRING strColor =  CDataPool::GetMe()->ItemDrop_GetItem(this->GetServerID())->GetDropVisColor();
			STRING	strName = CDataPool::GetMe()->ItemDrop_GetItem(this->GetServerID())->GetName();

			// ������ɫ
			if(strColor.size() == 6)
		 {
			 strColor = "#c" + strColor;
			 strName  = strColor + strName;
		 }

		 m_pInfoBoard->SetElement_Name(strName.c_str());
		 m_pInfoBoard->Show(TRUE);


		 // ����λ��
		 fVector2 fvPos = m_pInfoBoard ? m_pInfoBoard->GetPosition() : fVector2(0.f, 0.f);
		 fVector3 fvCurPos = GetPosition();		// �������λ��

		 fVector3 fvGfxPos;
		 fVector3 fvScreen;

		 CEngineInterface::GetMe()->Axis_Trans(CEngineInterface::AX_GAME, fvCurPos, CEngineInterface::AX_GFX, fvGfxPos);

		 CEngineInterface::GetMe()->Axis_Trans(CEngineInterface::AX_GFX, fvGfxPos, CEngineInterface::AX_SCREEN, fvScreen);

		 //ƽ������
		 fvPos.x = SMOOTH(fvPos.x, fvScreen.x, 0.8f, 3.f);
		 fvPos.y = SMOOTH(fvPos.y, fvScreen.y, 0.8f, 3.f)-45;
		 m_fvInfoBoardPos = fvPos;  

		 m_pInfoBoard->SetPosition( m_fvInfoBoardPos.x,m_fvInfoBoardPos.y );
		 m_pInfoBoard->ShowCharAttr( false );//
		}
	}

}


VOID   CTripperObject_ItemDrop::SetStartTime( UINT time)
{ 
	m_StartTime = time;

}

VOID	CTripperObject_ItemDrop::Tripper_AddEffect()
{
	if ( m_bCanEveryOnePick )
	{
		// m_pRenderInterface->AddEffect("wq_huoyandao","�����󶨵�_04");
		 //������Ʒ��Ч�󶨵�

		return ;
	}

}

VOID	CTripperObject_ItemDrop::Tripper_RealseEffect()
{
}

void	CTripperObject_ItemDrop::Tick_UpdateInfoBoard()
{

	// ͷ����Ϣ���Ӧ��UI�ӿ�
	if( !m_pInfoBoard ) 
		return;

	// ����λ��
	fVector2 fvPos = m_pInfoBoard ? m_pInfoBoard->GetPosition() : fVector2(0.f, 0.f);
	fVector3 fvCurPos = GetPosition();		// �������λ��

	/////////////////////////////////////
	BOOL bVisible;
	bVisible = GetRenderInterface()->Actor_GetInfoBoardPos( fvPos, &fvCurPos );//�ж�װ���Ƿ�ɼ�

	if( !bVisible )
	{
		m_pInfoBoard->Show(FALSE);
		return;
	}

	// ���þ������ǵľ���
	fVector3 fMyselfPos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
	FLOAT fDistance = KLU_GetDistSq( fMyselfPos, GetPosition() );
	
	// ��������Լ��趨�����û����ʹ�õ��ǵ�ǰ��ң��������NPC������ҵľ���
	m_pInfoBoard->SetInfoDistance( fDistance );

	fVector3 fvGfxPos;
	fVector3 fvScreen;

	CEngineInterface::GetMe()->Axis_Trans(CEngineInterface::AX_GAME, fvCurPos, CEngineInterface::AX_GFX, fvGfxPos);



	CEngineInterface::GetMe()->Axis_Trans(CEngineInterface::AX_GFX, fvGfxPos, CEngineInterface::AX_SCREEN, fvScreen);

	//ƽ������
	fvPos.x = SMOOTH(fvPos.x, fvScreen.x, 0.8f, 3.f);
	fvPos.y = SMOOTH(fvPos.y, fvScreen.y, 0.8f, 3.f) -45;

	m_fvInfoBoardPos = fvPos;  

	m_pInfoBoard->SetPosition( m_fvInfoBoardPos.x,m_fvInfoBoardPos.y );

    m_pInfoBoard->Show(TRUE);

	 
}

void	CTripperObject_ItemDrop::Tick()
{
	SetRotation(m_fvRotation);
	m_fvRotation.y = m_fvRotation.y+0.1;
}

BOOL CTripperObject_ItemDrop::Tripper_CanOperation(VOID) const
{ 

	//if(CObjectManager::GetMe()->GetMySelf()->GetServerID() == m_idOwner)   /*GetServerGUID()*/


	//	return TRUE;
	//else
	//	return FALSE;
	if (m_bCanPick)
	{
		return TRUE;
	}

	if (m_bCanEveryOnePick)
	{
		return TRUE;
	}
	  
	for (int i = 0 ;i < MAX_TEAM_MEMBER ;++i)
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetServerID() == m_idOwner[i])   /*GetServerGUID()*/

			return TRUE;
	}

	return FALSE;

}

VOID CTripperObject_ItemDrop::Release( VOID )
{
	if( m_pInfoBoard /*&& CGameProcedure::s_pUISystem*/ )
	{
		m_pInfoBoard->Destroy();
		m_pInfoBoard = NULL;
	}

	CDataPool::GetMe()->ItemDrop_Destroy(this->GetServerID());

	if( m_pRenderInterface != NULL )
	{
		m_pRenderInterface->Destroy();
		m_pRenderInterface = NULL;
	}


	CObject_Static::Release();
}

VOID CTripperObject_ItemDrop::Tripper_Active(VOID)
{
 
	 Packets::CGPickDropItem msg;
	 msg.setObj_ItemId(this->GetServerID());  //obj id 
	 STRING	strName = CDataPool::GetMe()->ItemDrop_GetItem(this->GetServerID())->GetName();
	 msg.setObj_ItemName(strName.c_str());

	//_ITEM_GUID  sDataGuid;
	// WORD wServer, wWorld;
	// GUID_t dwSerial;

	//if ( !CDataPool::GetMe()->ItemDrop_GetItem(this->GetServerID()))
	//{
	//	return ;
	//}
	// CDataPool::GetMe()->ItemDrop_GetItem(this->GetServerID())->GetGUID(wWorld, wServer, dwSerial);
	//
	// sDataGuid.m_Serial	= dwSerial;
	// sDataGuid.m_Server	= (BYTE)wServer;
	// sDataGuid.m_World	= (BYTE)wWorld;

	// msg.setItemId( sDataGuid );     // GUID

 
	 CNetManager::GetMe()->SendPacket(&msg);
	 
}

