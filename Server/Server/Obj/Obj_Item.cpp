#include "stdafx.h"
#include "Obj_Item.h"
#include "Scene.h"
#include "Log.h"
#include "Config.h"
#include "GCNewObjItem.h"
#include "TimeManager.h"
#include "GameTable.h"
#include "ItemOperator.h"
#include "LogDefine.h"
#include "SceneDropPosManager.h"

Obj_Item::Obj_Item()
{
}

Obj_Item::~Obj_Item()
{

}

BOOL Obj_Item::Init( const _OBJ_INIT *pInit )
{
	__ENTER_FUNCTION

		CleanUp();

	BOOL bResult = Obj_Static::Init( pInit );


	if ( !bResult )
		return FALSE;


	_OBJ_ITEM_INIT *pItemInit = (_OBJ_ITEM_INIT*)pInit;
	if ( pItemInit == NULL )
		return FALSE;

	m_OwnerCount	=	0;
	m_OwnerCount	=   0;
	m_DropObjID		=   INVALID_ID;
	m_CreateTime	=	pItemInit->m_uCreateTime;
	m_RecycleTime	=	pItemInit->m_uRecycleTime;
	m_Obj_ItemType	=	pItemInit->m_ItemType;
	m_PickOwnerTime =	g_Config.m_ConfigInfo.m_DropProtectTime;
	if(pItemInit->m_bRecycle)
	{
		m_RecycleTimer.BeginTimer( m_RecycleTime, m_CreateTime );
	}
	else
	{
		m_RecycleTimer.CleanUp();
	}

	__LEAVE_FUNCTION

		return TRUE;
}

BOOL	Obj_Item::HeartBeat(UINT uTime/* =0  */)
{
	__ENTER_FUNCTION

		BOOL bResult = Obj_Static::HeartBeat( uTime );
	if ( !bResult )
		return FALSE;

	UpdateZone();

	if( m_RecycleTimer.CountingTimer(uTime) )
	{
		//回收操作
		//
		Recycle();
		return TRUE;
	}

	return	TRUE;

	__LEAVE_FUNCTION

		return	FALSE;

}

Packet *Obj_Item::CreateNewObjPacket( VOID )
{
	__ENTER_FUNCTION

		if ( getScene() != NULL )
		{
			_ITEM* pItem = GetObj_Item();
			Assert(pItem);
			Assert(!pItem->IsNullType());
			GCNewObjItem *pPacket = (GCNewObjItem*)(getScene()->m_nPacket_NewObj_Item);
			pPacket->CleanUp();
			pPacket->setObjType(GetType());
			pPacket->setObjID(GetID());
			pPacket->setProtectTime(GetPickOwnerTime());
			pPacket->setItemIndex(pItem->m_ItemIndex);
			pPacket->setItemCount(pItem->GetItemCount());
			for (INT i=0; i<GetOwnerCount(); ++i)
			{
				pPacket->setOwner(GetOwner(i));
			}
			pPacket->setWorldPos(*(getWorldPos()));
			pPacket->setDropObjID(GetDropObjID());
			return pPacket;
		}
		else
		{
			Assert( getScene() != NULL && "Obj_Item::CreateNewObjPacket" );
			return NULL;
		}

		__LEAVE_FUNCTION

			return NULL;
}

VOID Obj_Item::DestroyNewObjPacket( Packet *pPacket )
{
}


BOOL	Obj_Item::CanPick(GUID_t nPickID, ObjID_t HumanID)
{
	__ENTER_FUNCTION

	Obj_Human*	pHuman = getScene()->GetHumanManager()->GetHuman(HumanID);
	Assert(pHuman);

	if(GetType()==ITYPE_MONSTERDROP_OBJ_ITEM)
	{
		for(UINT i=0; i<m_OwnerCount; ++i)
		{
			if(m_OwnerId[i] == nPickID)
				return true;
		}
		return  g_pTimeManager->CurrentTime()>(uint)m_FinalPickTime;
	}
	else if (GetType()==ITYPE_PERSONDROP_OBJ_ITEM)
	{
		return TRUE;
	}
	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL	Obj_Item::IsCanViewMe(const Obj *pObj )
{
	__ENTER_FUNCTION

		if(!pObj)
			return FALSE;

	//if(GetType()==ITYPE_DROPBOX)
	{
		if(pObj->GetObjType() == OBJ_TYPE_HUMAN)
		{
			Obj_Human* pHuman = (Obj_Human*)pObj;
			if(!pHuman)
				return FALSE;

			return	TRUE/*m_OwnerId == pHuman->GetGUID()||m_OwnerId==INVALID_ID*/;
		}
	}

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE ;
}



VOID Obj_Item::EnablePickOwnerTime()
{
	__ENTER_FUNCTION

		m_FinalPickTime	=	g_pTimeManager->CurrentTime()+m_PickOwnerTime;

	__LEAVE_FUNCTION
}

UINT	Obj_Item::GetPickOwnerTime()
{
	return m_PickOwnerTime;
}

VOID Obj_Item::SetPickOwnerTime(UINT uPickTime)
{
	m_PickOwnerTime	=	uPickTime;
}


//VOID	Obj_Item::SetMaxGrowTime(UINT uGrowTime)
//{
//	__ENTER_FUNCTION
//
//		m_MaxGrowTime	=	uGrowTime;
//
//	m_LifeTimer.BeginTimer(m_MaxGrowTime,g_pTimeManager->CurrentTime());
//
//	__LEAVE_FUNCTION
//}


VOID	Obj_Item::Recycle()
{
	__ENTER_FUNCTION
		//回收日志
	WORLD_POS worldPos;
	worldPos.m_fX  =	getWorldPos()->m_fX;
	worldPos.m_fZ  =	getWorldPos()->m_fZ;
	INT Type = GetType();
	Scene* pScene = getScene();
	Assert(pScene);
	SceneDropPosManager* pDropPosMag = pScene->GetSceneDropPosManager();
	Assert(pDropPosMag);

	ITEM_LOG_PARAM	ItemLogParam;
	ItemLogParam.OpType		=	ITEM_BOX_RECYCLE;
	ItemLogParam.SceneID	=	pScene->SceneID();
	ItemLogParam.XPos		=	getWorldPos()->m_fX;
	ItemLogParam.ZPos		=	getWorldPos()->m_fZ;
	ItemLogParam.ItemType	=	m_Obj_Item.m_ItemIndex;

	SaveItemLog(&ItemLogParam);


	//if(m_Obj_ItemType != ITYPE_DROPBOX) //非DropBox 都应该由生长点管理器释放
	//{
	//	BOOL bRet =
	//		getScene()->GetGrowPointManager()->DecGrowPointTypeCount(m_Obj_ItemType,getWorldPos()->m_fX,getWorldPos()->m_fZ);

	//	Assert(bRet);
	//}

	SetActiveFlag(FALSE);
	pScene->DeleteObject(this);
	
	if(Type==ITYPE_MONSTERDROP_OBJ_ITEM)
	{
		pDropPosMag->ClearDropPos(worldPos.m_fX, worldPos.m_fZ);
	}
	__LEAVE_FUNCTION
}


VOID	Obj_Item::CleanUp()
{
	__ENTER_FUNCTION

	m_CreateTime		=	0;
	m_RecycleTime		=	0;
	//m_MaxGrowTime		=	0;
	m_Obj_ItemType		=	-1;
	m_RecycleTimes		=	0;

	//m_LifeTimer.CleanUp();

	m_PickOwnerTime		=	0;

	m_FinalPickTime		=	0;
	for(INT i=0; i<MAX_TEAM_MEMBER; ++i)
	{
		m_OwnerId[i] = INVALID_ID;
	}
	m_DropObjID			=   INVALID_ID;

	__LEAVE_FUNCTION
}

//VOID	Obj_Item::SetObj_Item(UINT nItemIndex)
//{
//	__ENTER_FUNCTION
//		Assert(nItemIndex != INVALID_INDEX);
//	m_nItemIndex = nItemIndex;
//	__LEAVE_FUNCTION
//}
//
//UINT	Obj_Item::GetObj_Item()
//{
//	__ENTER_FUNCTION
//		return	m_nItemIndex;
//	__LEAVE_FUNCTION
//}


ScriptID_t	Obj_Item::GetScriptID() const
{
	__ENTER_FUNCTION

		if(m_Obj_ItemType == ITYPE_DROPBOX)
		{
			return INVALID_ID;
		}

		_GROW_POINT_INFO*	pGET = g_GrowPointInfoTbl.Get(m_Obj_ItemType);
		Assert(pGET);
		if(pGET->m_ScriptID>0)
		{
			return pGET->m_ScriptID;
		}

		return INVALID_ID;

		__LEAVE_FUNCTION

			return INVALID_ID;
}

VOID				Obj_Item::SaveObj_Item(_ITEM* pItem)			//设置物品数据
{
	Assert(pItem);
	memcpy(&m_Obj_Item, pItem, sizeof(_ITEM));
}

_ITEM*				Obj_Item::GetObj_Item()						//获取物品数据
{
	return &m_Obj_Item;
}