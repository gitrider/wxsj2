
/**	ObjectManager.cpp
*/

#include "StdAfx.h"
#include "Global.h"
#include "ObjectManager.h"
#include "..\Logic\Object.h"
#include "..\Logic\Character\Obj_Character.h"
#include "..\Logic\Character\Obj_PlayerOther.h"
#include "..\Logic\Character\Obj_PlayerMySelf.h"
#include "..\Logic\Bus\Obj_Bus.h"
#include "DataPool\GMDP_CharacterData.h"
#include "..\Logic\Surface\Obj_Surface.h"
#include "Procedure\GameProcedure.h"
#include "Procedure\GamePro_Main.h"
#include "DBC\GMDataBase.h"
#include "NetWork\NetManager.h"
#include "GIDBC_Struct.h"
#include "GIException.h"
#include "GIinputsystem.h"
#include "GIGfxSystem.h"
#include "Cursor\CursorMng.h"
#include "..\Logic\ProjTex\Obj_ProjTex.h"
#include "World\WorldManager.h"
#include "..\Logic\Item\Obj_Item.h"
#include "..\Logic\Item\Obj_Item_Equip.h"
#include "..\Logic\Item\Obj_Item_Medicine.h"
#include "..\Logic\Item\Obj_Item_Gem.h"
#include "..\Logic\Item\Obj_Item_StoreMap.h"
#include "CGLockTarget.h"
#include "GIEventDefine.h"
#include "GIUtil.h"
#include "Event\GMEventSystem.h"
#include "AxProfile.h"
#include "..\GMObjectLoadQueue.h"
#include "FakeObjectManager.h"
#include "GIGameInterface.h"
#include "DataPool\GMUIDataPool.h"
#include "../GMObjectDestoryQueue.h"


KL_IMPLEMENT_DYNAMIC(CObjectManager, GETCLASS(tObjectSystem));
CObjectManager* CObjectManager::s_pMe = NULL;





CObjectManager::CObjectManager()
{
	s_pMe				= this;
	m_pMySelf			= NULL;
	m_pObjectRoot		= NULL;
	m_pLogicalObject	= NULL;
	m_pPlayerOther		= NULL;
	m_pAlogicalObject	= NULL;

	m_pMainTarget		= NULL;
	m_pLoadQueue		= 0;
	m_pDestoryQueue		= 0;

	m_bIsSelTeam = false;
}

CObjectManager::~CObjectManager()
{
	if( s_pMe == this )
		s_pMe = NULL;
}


VOID CObjectManager::Initial( VOID* )
{
	m_pObjectRoot				= (CObject*)g_theKernel.NewNode(_T("CObject"), _T("scene"), _T("object"));
		m_pLogicalObject		= (CObject*)g_theKernel.NewNode(_T("CObject"), m_pObjectRoot, _T("logical"));
			m_pPlayerOther		= (CObject*)g_theKernel.NewNode(_T("CObject"), m_pLogicalObject, _T("otherplayer"));
			m_pNPC				= (CObject*)g_theKernel.NewNode(_T("CObject"), m_pLogicalObject, _T("npc"));
			m_pSurface			= (CObject*)g_theKernel.NewNode(_T("CObject"), m_pLogicalObject, _T("surface"));
		m_pAlogicalObject		= (CObject*)g_theKernel.NewNode(_T("CObject"), m_pObjectRoot, _T("alogical"));

	m_pLoadQueue = new CObjectLoadQueue();
	m_pDestoryQueue = new CObjectDestoryQueue();

	// 目标
	m_pMainTargetAvatar = (CObject_PlayerOther*)(CFakeObjSystem::GetMe()->NewFakeObject("CObject_PlayerOther", "Target", "Camera_Main"));
	m_pMainTargetAvatar->SetFaceDir(0);
	// 创建角色Avatar
	// 在UI脚本中调用显示
//	m_pAvatar[0] = (CObject_PlayerOther*)(CFakeObjSystem::GetMe()->NewFakeObject(_T("CObject_PlayerOther"), _T("CreateRole_Woman"), "Camera_Main"));
//	m_pAvatar[1] = (CObject_PlayerOther*)(CFakeObjSystem::GetMe()->NewFakeObject(_T("CObject_PlayerOther"), _T("CreateRole_Man"), "Camera_Main"));
}

VOID CObjectManager::Release(VOID)
{
	ClearAllObject();

	delete m_pLoadQueue; 
	m_pLoadQueue = 0;

	delete m_pDestoryQueue;
	m_pDestoryQueue = 0;
}


VOID CObjectManager::Tick(VOID)
{
	// 检查是否有物体不再被关心
	if(CGameProcedure::GetActiveProcedure() == CGameProcedure::s_pProcMain)
		m_pLogicalObject->Tick_CheckAlive();

	// 执行逻辑函数
	m_pLogicalObject->Tick();

	// 检查被UI关心的逻辑对象
	std::map< std::string, OBJECT_BECARED >::iterator it;
	for(it=m_mapCaredObject.begin(); it!=m_mapCaredObject.end(); it++)
	{
		OBJECT_BECARED& obj = it->second;
		// 物体是否存在
		CObject* pObject = (CObject*)FindObject(obj.id);
		if(!pObject)
		{
			KLAssert(false && "Careobject error find!");
			continue;
		}

		// 计算目前的距离
		FLOAT fDistance = KLU_GetDist(	fVector2(GetMySelf()->GetPosition().x,	GetMySelf()->GetPosition().z),
										fVector2(pObject->GetPosition().x,		pObject->GetPosition().z));

		float fStep = abs(fDistance-obj.fLastDistance);
		if(fStep > 0.001)
		{
			// 距离发生改变，产生事件
			std::vector< STRING > vParam;
			CHAR szTemp[MAX_PATH];

			_snprintf(szTemp, MAX_PATH, "%d", pObject->GetID());
			vParam.push_back(szTemp);

			vParam.push_back("distance");

			_snprintf(szTemp, MAX_PATH, "%.3f", fDistance);
			vParam.push_back(szTemp);

			CEventSystem::GetMe()->PushEvent(GE_OBJECT_CARED_EVENT, vParam);
		}

		obj.fLastDistance = fDistance;
	}

	// 加载队列工作
	if(m_pLoadQueue)
	{
		m_pLoadQueue->Tick();
	}

	// 删除队列
	if(m_pDestoryQueue)
	{
		m_pDestoryQueue->Tick();
	}
}

tObject* CObjectManager::NewObject(LPCTSTR szClass, INT idServer, tObject* pParent)
{
	KLAssert(szClass);

	// 查找是否已经有这样的玩家
	if(idServer != -1)
	{
		if(strcmp(szClass,"CTripperObject_ItemDrop"))
		{
			tObject* pFind = (tObject*)FindServerObject(idServer);
			// 如果已经有
			if(pFind != NULL)
			{
				return pFind;
			}
		}
	}

	// 通过数据Kernel创建节点
	INT id = IDFactory();
	TCHAR szName[MAX_PATH];
	_stprintf(szName, _T("%d"), id);

	CObject* pNewNode = (CObject*)g_theKernel.NewNode(szClass, m_pObjectRoot, szName);
	m_pObjectRoot->EraseChild(pNewNode);

	pNewNode->SetServerID( idServer );
	pNewNode->m_ID = id;
	AttachObject( pNewNode );

	pNewNode->Enable(OSF_VISIABLE);

	CGameProcedure::m_bWaitNeedFreshMinimap = true;

	return pNewNode;
}

BOOL CObjectManager::DestroyObject(tObject* pObject)
{
	CGameProcedure::m_bWaitNeedFreshMinimap = true;
	KLAssert(pObject);

	DetchObject( pObject );

	delete (CObject*)pObject;

	return true;
}

// 根据ID得到某物体
tObject* CObjectManager::FindObject( INT id )
{
	CObjectMap::iterator it = m_hmObject.find(id);

	if(it == m_hmObject.end()) 
		return 0;
	else 
		return it->second;
}

// 获取一个SERVER OBJ对象的指针
tObject* CObjectManager::FindServerObject( INT idServer )
{
	CObjectMap::iterator it = m_hmServerObject.find(idServer);

	if(it == m_hmServerObject.end()) 
		return 0;
	else 
		return it->second;
}

tObject* CObjectManager::FindBullObject( string& objName )
{
	CObjectStrMap::iterator iter = m_mBullObject.find(objName);
	if(iter == m_mBullObject.end())
		return NULL;
	else
		return iter->second;
}


const std::vector<INT>& CObjectManager::GetNpcObjectId(VOID)
{
	return m_vNpcObjectIndex;
}


CObject_Character* CObjectManager::FindCharacterByName(std::string szName)
{
	CObject_Character* pObject = NULL;

	if(!(m_hmServerObject.empty()))
	{
		std::map< INT, CObject* >::iterator it = m_hmServerObject.begin();
		for(;it != m_hmServerObject.end(); it++)
		{
			pObject = (CObject_Character*)(it->second);
			if(!g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_Character)))
			{
				pObject = NULL;
			}
			else
			{
				std::string szObjName = pObject->GetCharacterData()->Get_Name();
				if(szObjName != szName)
					pObject = NULL;
				else
					break;
			}				
		}
	}

	return pObject;
}

tObject_Item* CObjectManager::FindItem(INT id)
{
	return CObject_Item::FindItem(id);
}

VOID CObjectManager::ClearAllObject(VOID)
{
	//m_pMySelf			= NULL;
	m_pAlogicalObject->EraseAllChild();
	m_pPlayerOther->EraseAllChild();
	m_pNPC->EraseAllChild();
	m_pSurface->EraseAllChild();
	m_pLogicalObject->EraseAllChild();
	m_pLogicalObject->AddChild( m_pPlayerOther );
	m_pLogicalObject->AddChild( m_pNPC );
	m_pLogicalObject->AddChild( m_pSurface );
	m_pLogicalObject->AddChild( m_pMySelf );

	CObject *pObject;
	CObjectMap::iterator itCur, itEnd;
	if ( !m_hmObject.empty() )
	{
		itEnd = m_hmObject.end();
		for ( itCur = m_hmObject.begin(); itCur != itEnd; itCur++ )
		{
			pObject = (CObject*)(itCur->second);
			if ( pObject != NULL && m_pMySelf != pObject )
			{
				pObject->Release();
				delete pObject;
			}
		}
		m_hmObject.clear();
	}

	m_hmServerObject.clear();
	m_vNpcObjectIndex.clear();
	m_mBullObject.clear();
}

BOOL CObjectManager::SetObjectServerID( INT idObj, INT idServer )
{
	if ( idObj != INVALID_ID )
	{
		CObject *pObject = (CObject*)(FindObject( idObj ));
		if ( pObject != NULL )
		{
			INT idOldServer = pObject->GetServerID();
			if ( idOldServer != INVALID_ID )
				m_hmServerObject.erase( idOldServer ); 

			if ( idServer != INVALID_ID )
				m_hmServerObject[idServer] = pObject; 

			pObject->SetServerID( idServer );
		}
	}
	return TRUE;
}

// 产生新物体,并加入数据链
CObject_PlayerOther* CObjectManager::NewPlayerOther( INT idServer )
{
	return (CObject_PlayerOther*)(NewObject( "CObject_PlayerOther", idServer, m_pPlayerOther ));
}

/// 产生新物体,并加入数据链
CObject_PlayerNPC* CObjectManager::NewPlayerNPC( INT idServer )
{
	return (CObject_PlayerNPC*)(NewObject( "CObject_PlayerNPC", idServer, m_pNPC ));
}

CObject_Surface* CObjectManager::NewSurfaceObj( string strClass, LPCTSTR ObjName )
{
	CObject_Surface* obj = (CObject_Surface*)NewObject( strClass.c_str(), INVALID_UID, m_pSurface );

	if(obj)
		m_mBullObject[ObjName] = obj;

	return obj;
}

VOID CObjectManager::DetchBullObject( string& objName )
{
	CObjectStrMap::iterator iter = m_mBullObject.find(objName);
	if(iter != m_mBullObject.end())
	{
		DetchObject( iter->second );
		m_mBullObject.erase( objName );
	}
}


VOID CObjectManager::AttachObject( tObject *pObject )
{
	KLAssert(pObject);
	
	// 根据是否有逻辑函数加入到节点中
	if(g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_PlayerMySelf)))
	{
		m_pLogicalObject->AddChild(pObject);
	}
	else if(g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_PlayerOther)))
	{
		m_pPlayerOther->AddChild(pObject);
	}
	else if(g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_PlayerNPC)))
	{
		m_pNPC->AddChild(pObject);

		// 保存NPC对象ID， 使用自动施放技能时查找周围怪物用
		std::vector<INT>::iterator iter = std::find(m_vNpcObjectIndex.begin(), m_vNpcObjectIndex.end(), pObject->GetID());
		if(iter == m_vNpcObjectIndex.end())
			m_vNpcObjectIndex.push_back(pObject->GetID());
	}
	else if(g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_Bus)))
	{
		m_pLogicalObject->AddChild(pObject);
	}
	else if(g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_Surface)))
	{
		m_pSurface->AddChild(pObject);
	}
	else if(g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_ProjTex)))
	{
		m_pLogicalObject->AddChild(pObject);
	}
	else
	{
		m_pAlogicalObject->AddChild(pObject);
	}

	// 加入hash_map
	m_hmObject[pObject->GetID()] = ((CObject*)pObject);

	INT idServer = ((CObject*)pObject)->GetServerID();
	if(idServer != INVALID_UID) 
	{
		m_hmServerObject[idServer] = ((CObject*)pObject);
	}


	//如果是自身，记录下指针
	if(pObject->GetClass() == GETCLASS(CObject_PlayerMySelf))
	{
		m_pMySelf = (CObject_PlayerMySelf*)pObject;
	}
}

VOID CObjectManager::DetchObject( tObject *pObject )
{
	KLAssert(pObject);

	INT id = pObject->GetID();
	INT idServer = ((CObject*)(pObject))->GetServerID();

	// 从数据链表中删除
	if(! pObject->GetParent()->EraseChild(pObject) ) 
		return ;

	// 从HashMap中删除
	m_hmObject.erase(id);

	if( idServer != INVALID_ID )
		m_hmServerObject.erase( idServer ); 

	m_vNpcObjectIndex.erase(remove(m_vNpcObjectIndex.begin(), m_vNpcObjectIndex.end(), id), m_vNpcObjectIndex.end());


	// 如果是自身，记录下指针
	if(pObject == m_pMySelf )
	{
		m_pMySelf = NULL;
	}

	// 如果是主目标,取消选择
	if(pObject == m_pMainTarget)
	{
		m_pMainTarget = NULL;
	}

	// 如果被ui关心，发出事件并清除
	std::map< std::string, OBJECT_BECARED >::iterator it;
	for(it=m_mapCaredObject.begin(); it!=m_mapCaredObject.end(); it++)
	{
		OBJECT_BECARED& obj = it->second;
		if(obj.id == id)
		{
			// 产生事件
			std::vector< STRING > vParam;
			CHAR szTemp[MAX_PATH];

			_snprintf(szTemp, MAX_PATH, "%d", id);
			vParam.push_back(szTemp);

			vParam.push_back("destroy");

			CEventSystem::GetMe()->PushEvent(GE_OBJECT_CARED_EVENT, vParam);

			m_mapCaredObject.erase(it);
			break;
		}
	}

	// 删除队列中其他同ID的物体
	std::map< std::string, OBJECT_BECARED >::iterator itNext;
	for(it=m_mapCaredObject.begin(); it!=m_mapCaredObject.end(); it=itNext)
	{
		itNext = it;
		itNext++;

		OBJECT_BECARED& obj = it->second;
		if(obj.id == id)
		{
			m_mapCaredObject.erase(it);
		}
	}
}

tObject* CObjectManager::GetMouseOverObject( INT nX, INT nY, fVector3& fvMouseHitPlan ) 
{
	CObject* pHitObject = NULL;
	
	// 取得鼠标射线
	fRay rayScreen;
	CGameProcedure::s_pGfxSystem->Camera_GetWindowToViewportRay(nX, nY, rayScreen);

	// 根据屏幕上的坐标取得3D世界中的坐标
	const POINT pt = CGameProcedure::s_pInputSystem->MouseGetPos();
	if(!CGameProcedure::s_pGfxSystem->Axis_Trans( tGfxSystem::AX_SCREEN, fVector3((FLOAT)pt.x, (FLOAT)pt.y, 0.0f), 
		tGfxSystem::AX_PLAN, fvMouseHitPlan))
	{
		// 与世界没有相交的物体
		pHitObject = NULL;
		return pHitObject;
	}

	// 和行走面相交
	//fVector3 fvBuilding;
	//BOOL bInBuilding = ((CScene*)CWorldManager::GetMe()->GetActiveScene())->m_WalkCollisionMng.SelBuilding(
	//						rayScreen.mOrigin.x, rayScreen.mOrigin.y, rayScreen.mOrigin.z, 
	//						rayScreen.mDirection.x, rayScreen.mDirection.y, rayScreen.mDirection.z, 
	//						(INT)fvMouseHitPlan.x, (INT)fvMouseHitPlan.z,
	//						fvBuilding.x, fvBuilding.y, fvBuilding.z);

	fVector3 fvBuilding;
	BOOL bInBuilding = ((CScene*)CWorldManager::GetMe()->GetActiveScene())->collisionRay(rayScreen,
		fvBuilding);

	if(bInBuilding)
	{
		fvMouseHitPlan = fvBuilding;
	}

	if(bInBuilding)
	{
		fvMouseHitPlan = fvBuilding;
	}


	// 查找鼠标接触物体
	static INT s_nLastHitObj = -1;

	tEntityNode* pEntityHitObj = CGameProcedure::s_pGfxSystem->Find_HitFairyObject(pt.x, pt.y);
	if(pEntityHitObj)
	{
		pHitObject = (CObject*)FindObject((INT)(pEntityHitObj->GetData()));
		if(pHitObject)
		{
			CGameProcedure::s_pGfxSystem->SetHitObjText( pHitObject->GetDebugDesc().c_str());
		}
		else 
		{
			CGameProcedure::s_pGfxSystem->SetHitObjText( CGameProcedure::s_pUISystem->GetDebugString().c_str());
		}
	}
	else
	{
		CGameProcedure::s_pGfxSystem->SetHitObjText( CGameProcedure::s_pUISystem->GetDebugString().c_str());
	}

	INT nHitObjID = pHitObject ? pHitObject->GetID() : -1;
	if(s_nLastHitObj != nHitObjID)
	{
		// 设定选择
		if( pHitObject )	
		{
			pHitObject->GetRenderInterface()->Actor_SetMouseHover( TRUE );

			if( g_theKernel.IsKindOf( pHitObject->GetClass(), GETCLASS(CObject_PlayerMySelf) ) ||
				g_theKernel.IsKindOf( pHitObject->GetClass(), GETCLASS(CObject_PlayerOther)  ) ||
				g_theKernel.IsKindOf( pHitObject->GetClass(), GETCLASS(CObject_PlayerNPC)    ))
				((CObject_Character*)pHitObject)->MouseHoverState( TRUE );
		}

		// 取消选择
		CObject* pLastHitObj = (CObject*)FindObject( s_nLastHitObj );
		if( pLastHitObj )
		{
			pLastHitObj->GetRenderInterface()->Actor_SetMouseHover( FALSE );

			if( g_theKernel.IsKindOf( pLastHitObj->GetClass(), GETCLASS(CObject_PlayerMySelf) ) ||
				g_theKernel.IsKindOf( pLastHitObj->GetClass(), GETCLASS(CObject_PlayerOther)  ) ||
				g_theKernel.IsKindOf( pLastHitObj->GetClass(), GETCLASS(CObject_PlayerNPC)    ))
				((CObject_Character*)pLastHitObj)->MouseHoverState( FALSE );
		}

		s_nLastHitObj = nHitObjID;
	}

	return pHitObject;
}

INT CObjectManager::IDFactory(VOID)
{
	static INT nLastUID = 100;

	return ++nLastUID; 
}

VOID CObjectManager::SetMainTarget(INT id, DESTROY_MAIN_TARGET_TYPE delType)
{
	CObject* pFindObj = NULL;
	
	if (id != INVALID_ID)
	{
		pFindObj = (CObject*)FindServerObject(id);
	}

	
	// 不考虑无法选择的物体
	// 如果没选中物体或者选种的物体是不CanbeSelect的，告诉服务器取消当前选种对象。
	if( !pFindObj || ( pFindObj && !(pFindObj->CanbeSelect()) ) ) 
	{
		if( m_pMainTarget )
		{
			// 关闭选择环
			m_pMainTarget->GetRenderInterface()->Attach_ProjTexture( tEntityNode::SELECT_RING, FALSE );
			m_pMainTarget = NULL;
			
			if( (delType != DEL_OBJECT)||(pFindObj && !(pFindObj->CanbeSelect())) )
			{
				// 如果是鼠标右键销毁， target 窗口.
				// 如果当前选择的不是队友返回。
				
				// 设置当前没有选中队友
				SetCurSelTeam(-1);

				CGameProcedure::s_pEventSystem->PushEvent(GE_MAINTARGET_CHANGED, -1);
			}
			else
			{
				if( !IsSelTeam() )
				{
					// 如果当前选择的不是队友返回。
					CGameProcedure::s_pEventSystem->PushEvent(GE_MAINTARGET_CHANGED, -1);
				}
			}

		}
		else
		{
			// 设置当前没有选中队友
			SetCurSelTeam(-1);

			CGameProcedure::s_pEventSystem->PushEvent(GE_MAINTARGET_CHANGED, -1);
		}
		return;
	}

	if( pFindObj && (pFindObj != m_pMainTarget) )
	{
		// 关闭选择环
		if(m_pMainTarget)
		{
			m_pMainTarget->GetRenderInterface()->Attach_ProjTexture(tEntityNode::SELECT_RING, FALSE, 0);
		}

		// 设置选择对象
		m_pMainTarget = pFindObj;


		// 告诉server选中的对象

		// 发往服务器
		CGLockTarget msg;
		msg.setTargetID(id);
		CNetManager::GetMe()->SendPacket( &msg );


		// 打开新的选择环
		if(m_pMainTarget && m_pMainTarget->GetRenderInterface())
		{
			FLOAT Select_Ring_Range = ((CObject_PlayerNPC*)(CObject_Character*)pFindObj)->GetProjtexRange();

			//616======	根据不同的角色类型来处理不同的选择圈
			m_pMainTarget->GetRenderInterface()->Attach_ProjTexture(tEntityNode::SELECT_RING, TRUE, Select_Ring_Range);
		}

		// 产生一个选择对象更改的事件
		CGameProcedure::s_pEventSystem->PushEvent(GE_MAINTARGET_CHANGED, id);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 设置选中队友
		//

		TeamMemberInfo* pInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByServerId(m_pMainTarget->GetServerID());
		if(pInfo)
		{
			GUID_t GUID = pInfo->m_GUID;

			// 设置当前选中队友
			SetCurSelTeam(TRUE);
			SetCurSelTeam(GUID);
		}

	}
}

tObject* CObjectManager::GetMainTarget(VOID)
{
	return m_pMainTarget;
}

// 关心某物体的指定事件 id-物体ID,	szSign:关心标志（谁关心）		bCare-关心或者取消关心
// 修改说明：添加一个“是谁关心”的内容
VOID CObjectManager::CareObject(INT id, BOOL bCare, std::string szSign)
{
	//AxTrace(0,0,"C++ id =%d,bCare =%d,szSign =%s",id,(INT)bCare,szSign.c_str());
	// 搜索物体
	CObject* pObject = (CObject*)FindObject(id);
	if(!pObject) return;

	// 是否已经加入
	std::map< std::string, OBJECT_BECARED >::iterator it;
	it = m_mapCaredObject.find(szSign);

	// 在表中已经有
	if(it != m_mapCaredObject.end())// && !bCare)
	{
		if(bCare)
		{
			// 这个界面已经有关心的NPC，去关心新的 Npc 就可以了，
			it->second.id = id;
		}
		else
		{
			// 取消关心的操作
			m_mapCaredObject.erase(it);
		}
	}

	// 在表中没有
	if(it == m_mapCaredObject.end() && bCare)
	{
		OBJECT_BECARED objNewCared;
		objNewCared.id      = id;
		objNewCared.fLastDistance = KLU_GetDist(fVector2(GetMySelf()->GetPosition().x, GetMySelf()->GetPosition().z),
												fVector2(pObject->GetPosition().x, pObject->GetPosition().z));

		m_mapCaredObject.insert(std::make_pair(szSign, objNewCared));
	}
}

LPCTSTR CObjectManager::ItemNameByTBIndex(UINT idTable)
{
	BYTE    nItemClass = GetSerialClass(idTable);
	switch( nItemClass )
	{
		//武器WEAPON、防具DEFENCE、饰物ADORN   1
	case ICLASS_EQUIP: 
		{
			//if (ISCommonEquip(idTable))
			//{  
			//	// 打开数据表
			//	DBC_DEFINEHANDLE(s_pItem_EquipVisual, DBC_ITEM_COMMON_EQUIP_VISUAL);
			//	// 搜索纪录

			//	const _DBC_ITEM_COMMON_EQUIP_VISUAL* pEquipVisual = \
			//		(const _DBC_ITEM_COMMON_EQUIP_VISUAL*)s_pItem_EquipVisual->Search_Index_EQU((UINT)idTable);
			//	if(!pEquipVisual) return NULL;
			//	return pEquipVisual->szName;
			//}
			//else
			//{
			//	 // 打开数据表
			//	 DBC_DEFINEHANDLE(s_pItem_EquipVisual, DBC_ITEM_ADVANCED_EQUIP_VISUAL);
			//	 // 搜索纪录
			//	 const _DBC_ITEM_ADVANCED_EQUIP_VISUAL* pEquipVisual = \
			//		 (const _DBC_ITEM_ADVANCED_EQUIP_VISUAL*)s_pItem_EquipVisual->Search_Index_EQU((UINT)idTable);
			//	 if(!pEquipVisual) return NULL;
			//	 return pEquipVisual->szName;
			//}


			// 打开数据表
			DBC_DEFINEHANDLE(s_pItem_EquipVisual, DBC_ITEM_EQUIP_VISUAL);
			// 搜索纪录
			const _DBC_ITEM_EQUIP_VISUAL* pEquipVisual = \
				(const _DBC_ITEM_EQUIP_VISUAL*)s_pItem_EquipVisual->Search_Index_EQU((UINT)idTable);
			if(!pEquipVisual) return NULL;
			return pEquipVisual->szName;

		}
	
		break;

	case ICLASS_MATERIAL: //原料 2

	case ICLASS_COMITEM:  //药品 3 

	case ICLASS_TASKITEM: //任务物品 4
		{
			// 打开数据表
			DBC_DEFINEHANDLE(s_pItem_Medic, DBC_ITEM_MEDIC);
			// 搜索纪录
			const _DBC_ITEM_MEDIC* pMedic = (const _DBC_ITEM_MEDIC*)s_pItem_Medic->Search_Index_EQU((UINT)idTable);
			if(!pMedic) return NULL;
			return pMedic->szName;

		}

		break;
	case ICLASS_GEM: //宝石 5
	    {
			// 打开数据表
			DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
			// 搜索纪录
			const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU((UINT)idTable);
			if(!pGem) return NULL;
			return pGem->szName;
		}
		
		break;

	case ICLASS_STOREMAP:   //藏宝图  6
		{
			// 打开数据表
			DBC_DEFINEHANDLE(s_pItem_StoreMap, DBC_ITEM_STOREMAP);
			// 搜索纪录
			const _DBC_ITEM_STOREMAP* pStoreMap = (const _DBC_ITEM_STOREMAP*)s_pItem_StoreMap->Search_Index_EQU((UINT)idTable);
			if(!pStoreMap) return NULL;
			return pStoreMap->szName;
		}
		break;

	case ICLASS_TALISMAN : //法宝  7
		return NULL;
		break;

	case ICLASS_GUILDITEM : //帮会物品 8
		return NULL;
		break;

	default:
		{
			return NULL;
			KLThrow("Invalid ItemID:%d", idTable);

		}
		break;
	}	

}

void CObjectManager::Debug_LockNearestTarget(void)
{
	CObjectMap::iterator it;

	CObject* pNearestTarget = 0;
	float fNearestDist = 20.0f;		//最大允许距离(m)

	for(it=m_hmServerObject.begin(); it!=m_hmServerObject.end(); it++)
	{
		CObject* pObj = it->second;

		// 非角色
		if(!g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
			continue;

		// 友好阵营
		CObject_Character* pChar = (CObject_Character*)pObj;
		if(RELATION_ENEMY != CGameProcedure::s_pGameInterface->GetCampType(
									GetMySelf(), pChar))
		{
			continue;
		}

		// 死亡
		if( CObject_Character::CHARACTER_LOGIC_DEAD == pChar->CharacterLogic_Get(CObject_Character::LOGIC_BASE) )
			continue;

		// 距离太远
		float fDistance = ::KLU_GetDist(fVector2(GetMySelf()->GetPosition().x, GetMySelf()->GetPosition().z), 
										fVector2(pObj->GetPosition().x, pObj->GetPosition().z));

		if(fDistance >= fNearestDist) 
			continue;


		pNearestTarget = pObj;
		fNearestDist = fDistance;
	}

	// 锁定该目标
	if(pNearestTarget && pNearestTarget != GetMainTarget())
	{
		SetMainTarget(pNearestTarget->GetServerID());
	}
}


// 设置当前选择的是队友
void CObjectManager::SetCurSelTeam(GUID_t id)
{
	if(-1 == id)
	{
		m_bIsSelTeam = FALSE;
	}
	else
	{
		m_bIsSelTeam = TRUE;
	}

	m_GUID = id;
}

// 设置队友是否在当前可视范围
void CObjectManager::SetPresent(BOOL  bIsPresent)
{
	m_bIsSelTeamInPresent = bIsPresent;
}

// 当前选择的是否是对友 
BOOL CObjectManager::IsSelTeam()
{
	return m_bIsSelTeam;

}

	// 选择的队友是否在眼前 
BOOL CObjectManager::IsSelTeamInPresent()
{
	return m_bIsSelTeamInPresent;
}

GUID_t CObjectManager::GetCurSelTeam()
{
	return m_GUID;
}



VOID CObjectManager::SetTargetAvatarAsTarget( VOID )
{
	CObject* pObj = CDataPool::GetMe()->GetTargetEquip();
	
	if(!pObj)
		pObj = m_pMainTarget;

	if(pObj)
	{
		if( g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_PlayerOther)) )
		{

			m_pMainTargetAvatar->GetCharacterData()->Set_RaceID( ((CObject_Character*)pObj)->GetCharacterData()->Get_RaceID() );

			m_pMainTargetAvatar->GetCharacterData()->Set_HairColor( ((CObject_Character*)pObj)->GetCharacterData()->Get_HairColor() );
			m_pMainTargetAvatar->GetCharacterData()->Set_HairMesh( ((CObject_Character*)pObj)->GetCharacterData()->Get_HairMesh() );
			

			for(INT i=0; i<HEQUIP_NUMBER; i++)
			{
				m_pMainTargetAvatar->GetCharacterData()->Set_Equip((HUMAN_EQUIP)i,\
					((CObject_Character*)pObj)->GetCharacterData()->Get_Equip((HUMAN_EQUIP)i));
			}

			m_pMainTargetAvatar->SetFaceDir(0.0f);
		}
	}
}

// 取得当前选中物体
tObject* CObjectManager::GetMainTarget(GUID_t& guid)
{
	if(IsSelTeam())
		guid = m_GUID;
	else
		guid = (GUID_t) - 1;

	return m_pMainTarget;

}

void CObjectManager::OnScreenSizeChanged(void)
{
	CObjectMap::iterator it;
	for(it=m_hmServerObject.begin(); it!=m_hmServerObject.end(); it++)
	{
		CObject* pObj = it->second;
		if(g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		{
			((CObject_Character*)pObj)->Tick_UpdateInfoBoard();
		}
	}
}

void CObjectManager::OnMySelfLevelUp(void)
{
	// 重新根据等级刷新周围怪物的名称颜色
	CObjectMap::iterator it;
	for(it=m_hmServerObject.begin(); it!=m_hmServerObject.end(); it++)
	{
		if(g_theKernel.IsKindOf(it->second->GetClass(), GETCLASS(CObject_Character)))
		{
			CObject_Character* pChar = (CObject_Character*)(it->second);

			pChar->GetCharacterData()->Set_Name(pChar->GetCharacterData()->Get_Name());
		}
	}
}

void CObjectManager::UnSelMainTarget(void)
{
	if(m_pMainTarget)
	{
		// 关闭选择环
		m_pMainTarget->GetRenderInterface()->Attach_ProjTexture( tEntityNode::SELECT_RING, FALSE );
		m_pMainTarget	= NULL;
	}
}


// 设置属性
void CObjectManager::InitActorAvatar(void)
{
	// 女模型
//	m_pAvatar[0]->SetFaceDir(0);
//	m_pAvatar[0]->GetCharacterData()->Set_RaceID(0);
//	m_pAvatar[0]->GetCharacterData()->Set_HairColor(0);		// 设置缺省头发颜色
//
//	// 男模型 
//	m_pAvatar[1]->SetFaceDir(0);
//	m_pAvatar[1]->GetCharacterData()->Set_RaceID(1);
//	m_pAvatar[1]->GetCharacterData()->Set_HairColor(0);		// 设置缺省头发颜色
}
VOID CObjectManager::DestoryActorAvatar(VOID)
{
	return ;

//	if(m_pAvatar[0])
//	{
//		DetchObject(m_pAvatar[0]);
//		m_pAvatar[0] = NULL;
//	}
//
//	if(m_pAvatar[1])
//	{
//		DetchObject(m_pAvatar[1]);
//		m_pAvatar[1] = NULL;
//	}
}

void CObjectManager::FaceToTarget(INT sObj, INT tObj)
{
	CObject_Character* psObj = (CObject_Character*)(FindServerObject( sObj ));
	if(!psObj)
		return ; 
	CObject_Character* ptObj = (CObject_Character*)(FindServerObject( tObj ));
	if(!ptObj)
		return ;

	fVector2 fvs;
	fVector2 fvt;

	fvt.x = ptObj->GetPosition().x;
	fvt.y = ptObj->GetPosition().z;

	fvs.x = psObj->GetPosition().x;
	fvs.y = psObj->GetPosition().z;

	FLOAT fDir = KLU_GetYAngle( fvs, fvt );
	psObj->SetFaceDir( fDir );
}

LPCTSTR CObjectManager::GetMySelfOrTargetName(BOOL bTarget)
{
	if(!GetMySelf()) return "";
	if(!GetMySelf()->GetCharacterData()) return "";

	if(bTarget)
	{
		if(!GetMainTarget()) return "";
		if(!g_theKernel.IsKindOf(GetMainTarget()->GetClass(), GETCLASS(CObject_Character))) return "";

		return ((CObject_Character*)GetMainTarget())->GetCharacterData()->Get_Name();
	}
	else
	{
		return GetMySelf()->GetCharacterData()->Get_Name();
	}
}

