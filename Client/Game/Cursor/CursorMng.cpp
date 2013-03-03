
#include "StdAfx.h"
#include "cursormng.h"
#include "resource.h"
#include "Network\NetManager.h"
#include "gamecommand.h"
#include "object\Logic\Object.h"
#include "Interface\GMGameInterface.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Logic\TripperObj\GMTripperObj_ItemBox.h"
#include "DB_Struct.h"
#include "Global.h"
#include "GIUtil.h"
#include "Event\GMEventSystem.h"
#include "GIActionSystem.h"
#include "GITimeSystem.h"
#include "Action\GMActionSystem.h"
#include "DataPool\GMDP_CharacterData.h"
#include "DataPool\GMDataPool.h"
#include "Action\GMActionSystem_Item.h"
#include "CGUseItem.h"
#include "Object\Logic\Item\Obj_Item_Medicine.h"
#include "Action\GMActionSystem_PetSkill.h"



KL_IMPLEMENT_DYNAMIC(CCursorMng, GETCLASS(tCursorSystem));
CCursorMng* CCursorMng::s_pMe = NULL;


CCursorMng::CCursorMng(VOID)
{
	s_pMe = this;

	m_bShow = TRUE;
	m_nCursorState	= CURSOR_NORMAL;
	m_hUICursor = NULL;
	m_cmdCurrent_Left.Cleanup();
	m_cmdCurrent_Right.Cleanup();

	m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_NULL;
	m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_NULL;
}

CCursorMng::~CCursorMng(VOID)
{
}


VOID CCursorMng::SetCursor(ENUM_CURSOR_TYPE nType )
{
	m_nCursorState		= nType;
	::PostMessage( g_hMainWnd, WM_SETCURSOR, 0, HTCLIENT );
}

INT CCursorMng::GetCursor_State(VOID)
{
	return m_nCursorState;
}

HCURSOR CCursorMng::GetCursor(VOID)
{
	if(!m_bShow) 
		return NULL;

	return m_hCursor[m_nCursorState];
}

// 响应WM_SETCURSOR
VOID CCursorMng::OnSetCursor(VOID)
{
	if(m_bShow)
	{
		if(m_hUICursor)
		{
			::SetCursor(m_hUICursor);
		}
		else
		{
			::SetCursor(GetCursor());
		}
	}
}

VOID CCursorMng::ShowCursor(BOOL bShow)
{
	if(bShow == m_bShow) 
		return;

	m_bShow = bShow;
	::ShowCursor(m_bShow);
}


// 激活当前鼠标上挂接的命令
VOID CCursorMng::MouseCommand_Active(SCommand_Mouse& cmd)
{
	CGameProcedure::s_pGameInterface->SetAutoPathNPC(-1,"");
	switch(cmd.m_typeMouse)
	{
	case SCommand_Mouse::MCT_PLAYER_MOVETO:
		{
			static UINT uPrevMoveTime = 0;
			if(CGameProcedure::s_pTimeSystem->GetTimeNow() > uPrevMoveTime + 100)
			{
				uPrevMoveTime = CGameProcedure::s_pTimeSystem->GetTimeNow();
				CGameProcedure::s_pGameInterface->Player_MoveTo( fVector2( cmd.m_afParam[0], cmd.m_afParam[1] ) );
			}
		}
		break;

	case SCommand_Mouse::MCT_PLAYER_SELECT:
		{
			// 
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0], 1);

			if(	m_cmdCurrent_Left.m_typeMouse == SCommand_Mouse::MCT_SKILL_DIR ||
				m_cmdCurrent_Left.m_typeMouse == SCommand_Mouse::MCT_SKILL_AREA)
			{
				m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_NULL;
				m_cmdCurrent_Left.m_apParam[0] = NULL;
				m_cmdCurrent_Left.m_afParam[1] = NULL;
			
				// 恢复激活技能
				CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
			}
		}
		break;

	// 对单一对象使用技能
	case SCommand_Mouse::MCT_SKILL_OBJ:
		{
			// 首先选择目标
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[1], 2);

			tActionItem* pItem = (tActionItem*)cmd.m_apParam[0];
			if( !pItem ) 
				break;

			// 宠物攻击
			//if( pItem->GetType() == AOT_PET_SKILL )
			//{
			//	int nPetSkillID = ((PET_SKILL*)(((CActionItem_PetSkill*)pItem)->GetImpl()))->m_pDefine->m_nID;
			//	CDataPool::GetMe()->Pet_SendUseSkillMessage(nPetSkillID, INVALID_ID, cmd.m_afParam[1], cmd.m_afParam[2]);
			//}
			//// 玩家攻击
			//else
			{
				CGameProcedure::s_pGameInterface->Player_UseSkill(((tActionItem*)cmd.m_apParam[0])->GetID(), (INT)cmd.m_adwParam[1]);
			}
		}
		break;

	// 对某个范围使用技能
	case SCommand_Mouse::MCT_SKILL_AREA:
		{
			tActionItem* pItem = (tActionItem*)cmd.m_apParam[0];
			if( !pItem )
				break;

			if( pItem->GetType() == AOT_PET_SKILL )
			{
				int nPetSkillID = ((PET_SKILL*)(((CActionItem_PetSkill*)pItem)->GetImpl()))->m_pDefine->m_nID;
				CDataPool::GetMe()->Pet_SendUseSkillMessage(nPetSkillID, INVALID_ID, cmd.m_afParam[1], cmd.m_afParam[2]);
			}
			else
			{
				CGameProcedure::s_pGameInterface->Player_UseSkill(((tActionItem*)cmd.m_apParam[0])->GetID(), fVector2(cmd.m_afParam[1], cmd.m_afParam[2]));
			}

			// 恢复激活Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	// 对某个方向使用技能（扇形攻击）
	case SCommand_Mouse::MCT_SKILL_DIR:
		{
			tActionItem* pItem = (tActionItem*)cmd.m_apParam[0];
			if(!pItem) 
				break;

			// 忽略宠物的技能使用

			// 人物的方向技能使用
			CGameProcedure::s_pGameInterface->Player_UseSkill(((tActionItem*)cmd.m_apParam[0])->GetID(), cmd.m_afParam[1]);

			// 恢复激活Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	// 掉落包之类的
	case SCommand_Mouse::MCT_HIT_TRIPPEROBJ:	
		{
			CGameProcedure::s_pGameInterface->TripperObj_Active(cmd.m_adwParam[0]);
		}
		break;

	// NPC对话
	case SCommand_Mouse::MCT_SPEAK:		
		{
			// 首先选择目标
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0], 3);

			// 谈话
			CGameProcedure::s_pGameInterface->Player_Speak(cmd.m_adwParam[0]);
		}
		break;

	// 右键关联菜单
	case SCommand_Mouse::MCT_CONTEXMENU:
		{
			// 首先选择
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0], 0);
			
			// 显示右键菜单
			CGameProcedure::s_pGameInterface->Object_ShowContexMenu(cmd.m_adwParam[0]);
		}
		break;

	case SCommand_Mouse::MCT_REPAIR:
		{
			//do nothing...
		}
		break;

	case SCommand_Mouse::MCT_CANCLE_REPAIR:
		{
			// 取消修理
			CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
		}
		break;

    
	case SCommand_Mouse::MCT_UI_USE_IDENTIFY:
		{
			// 使用鉴定卷轴
			
			break;
		}
	case SCommand_Mouse::MCT_UI_USE_CANCEL_IDENTIFY:
		{
			// 取消使用鉴定卷轴
			CDataPool::GetMe()->Identify_UnLock();
			break;
		}

	case SCommand_Mouse::MCT_USE_ITEM:
		{
			// 使用物品
			CGameProcedure::s_pGameInterface->PacketItem_UserItem((	tActionItem*)cmd.m_apParam[0], 
																	cmd.m_adwParam[1], fVector2(cmd.m_afParam[2], cmd.m_afParam[3]));

			// 恢复激活Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SCommand_Mouse::MCT_CANCEL_USE_ITEM:
		{
			// 恢复激活Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;
	/*case SCommand_Mouse::MCT_PLAYER_JUMP:
		{
			CObjectManager::GetMe()->GetMySelf()->Jump();
		}
		break;*/

	default:
		AxTrace(0, 0, "Active: ERROR!(%d)", cmd.m_typeMouse);
		break;
	}
}

VOID CCursorMng::Initial( VOID * pHandle)
{
	HINSTANCE hInstance = *( ( HINSTANCE*) pHandle );

	m_hCursor[ CURSOR_WINBASE ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));				// 系统标准的
	m_hCursor[ CURSOR_NORMAL ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_NORMAL));		// 普通
	m_hCursor[ CURSOR_ATTACK ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_ATTACK));		// 攻击
	m_hCursor[ CURSOR_AUTORUN ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_AUTORUN ));	// 自动行走中
	m_hCursor[ CURSOR_PICKUP ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_PICKUP ));		// 拾取物品
	m_hCursor[ CURSOR_UNREACHABLE ] = ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_UNREACHABLE ));// 无法到达的区域
	m_hCursor[ CURSOR_MINE ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_MINE ));		// 采矿
	m_hCursor[ CURSOR_HERBS ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_HERBS ));		// 采药
	m_hCursor[ CURSOR_SPEAK ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_SPEAK ));		// 谈话
	m_hCursor[ CURSOR_INTERACT ]	= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_INTERACT ));	// 齿轮
	m_hCursor[ CURSOR_REPAIR ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_REPAIR ));		// 修理
	m_hCursor[ CURSOR_HOVER ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_HOVER));		// 鼠标激活(挂接物品...)
	m_hCursor[ CURSOR_IDENTIFY ]	= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_IDENFITY ));	// 鼠标鉴定
	m_hCursor[ CURSOR_RING ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_RING ));		// 技能环
	m_hCursor[ CURSOR_DIRECTION ]	= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_DIRECTION ));	// 方向技能
}

VOID CCursorMng::MouseCommand_Set( BOOL bHoverInUI, CObject* pSelectObj, const fVector3& fvPos, tActionItem* pActiveSkill )
{
	m_cmdCurrent_Left.m_typeMouse	= SCommand_Mouse::MCT_NULL;
	m_cmdCurrent_Right.m_typeMouse	= SCommand_Mouse::MCT_NULL;


	// 在UI上空
	if(bHoverInUI)
	{
		if(!pActiveSkill)
			return;

		switch(pActiveSkill->GetType())
		{
		// 修理装备
		case AOT_MOUSECMD_REPARE:
			{
				//左键挂锤子
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_REPAIR;
				//右键取消
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCLE_REPAIR;		
			}
			break;

		//鉴定装备
		case AOT_MOUSECMD_IDENTIFY:
			{
				//左键鉴定
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_IDENTIFY;
				//右键取消
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_IDENTIFY;		
			}
			break;
		}
	}
	else
	{
		// 判断鼠标下有无选择对象来计算相应obj鼠标指令
		if( pSelectObj != NULL )
		{
			pSelectObj->FillMouseCommand_Left( &m_cmdCurrent_Left, pActiveSkill );
			pSelectObj->FillMouseCommand_Right( &m_cmdCurrent_Right, pActiveSkill );
		}
		else
		{
			if( !pActiveSkill )
			{
				// 如果鼠标下没有相应的对象， 就当作移动来处理， 改掉了必须要有个默认技能才能走路得问题
				m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_PLAYER_MOVETO;
				m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
				m_cmdCurrent_Left.m_afParam[1] = fvPos.z;
			}

		}

		if( m_cmdCurrent_Left.m_typeMouse == SCommand_Mouse::MCT_NULL )
		{
			// 当前存在激活的技能
			if( pActiveSkill != NULL )
			{
				// 根据技能的类型设置鼠标行为
				switch( pActiveSkill->GetType() )
				{
				// 技能 和 宠物技能
				case AOT_SKILL:
				case AOT_PET_SKILL:
					{
						const void* pSkillImpl = pActiveSkill->GetImpl();

						// 不存在内部数据就设为行走状态
						if( !pSkillImpl )
						{
							m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_PLAYER_MOVETO;
							m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
							m_cmdCurrent_Left.m_afParam[1] = fvPos.z;

							return;
						}

						// 根据是否为战斗技能处理是人物（或宠物）的技能选择类型
						ENUM_SELECT_TYPE typeSel = (ENUM_SELECT_TYPE)(	pActiveSkill->GetType() == AOT_SKILL ? 
																		((SCLIENT_SKILL*)pSkillImpl)->m_pDefine->m_nSelectType :	// 人物技能
																		((PET_SKILL*)pSkillImpl)->m_pDefine->m_nSelectType );		// 宠物技能

						// 技能使用的鼠标选择逻辑
						switch( typeSel )
						{
						// 点选角色
						case SELECT_TYPE_CHARACTER:
							{
								m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_PLAYER_MOVETO;
								m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
								m_cmdCurrent_Left.m_afParam[1] = fvPos.z;
							}
							break;

						// 点选范围
						case SELECT_TYPE_POS:
							{
								// 储存技能
								m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_SKILL_AREA;		// 在自动技能时被改为 SCommand_Mouse::MCT_PLAYER_MOVETO;
								m_cmdCurrent_Left.m_apParam[0] = (VOID*)pActiveSkill;
								m_cmdCurrent_Left.m_afParam[1] = fvPos.x;
								m_cmdCurrent_Left.m_afParam[2] = fvPos.z;
							}
							break;

						// 方向
						case SELECT_TYPE_DIR:
							{
								// 储存技能
								m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_SKILL_DIR;	// 在自动技能时被改为 SCommand_Mouse::MCT_PLAYER_MOVETO;
								m_cmdCurrent_Left.m_apParam[0] = (VOID*)pActiveSkill;

								//====== 计算方向

								// 主角位置
								fVector3 fvPlayer = CObjectManager::GetMe()->GetMySelf()->GetPosition();
								// 鼠标位置
								fVector3 fvTarget = fvPos;

								m_cmdCurrent_Left.m_afParam[1] = KLU_GetYAngle(fVector2(fvPlayer.x, fvPlayer.z), fVector2(fvTarget.x, fvTarget.z));
							}
							break;

						case SELECT_TYPE_SELF:
							// ERROR...
							break;
						}
					}
					break;

				// 物品
				case AOT_ITEM:
					{
						if(!pActiveSkill || pActiveSkill->GetType() != AOT_ITEM) 
							break;

						CObject_Item* pItem = (CObject_Item*)(((CActionItem_Item*)pActiveSkill)->GetItemImpl());
						if(!pItem)
							break;

						// 必须是能够使用的物品
						if(pItem->GetItemClass() != ICLASS_COMITEM) 
							break;

						BOOL bAreaItem = ((CObject_Item_Medicine*)pItem)->IsAreaTargetType();

						m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_USE_ITEM;
						m_cmdCurrent_Left.m_apParam[0] = (VOID*)pActiveSkill;
						m_cmdCurrent_Left.m_adwParam[1] = -1;
						m_cmdCurrent_Left.m_afParam[2] = fvPos.x;
						m_cmdCurrent_Left.m_afParam[3] = fvPos.z;
						m_cmdCurrent_Left.m_adwParam[4] = bAreaItem;

						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCEL_USE_ITEM;
					}
					break;

				// 修理单独处理
				case AOT_MOUSECMD_REPARE:
					{
						//左键挂锤子
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_REPAIR;
						//右键取消
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCLE_REPAIR;		
					}
					break;

				// 鉴定装备
				case AOT_MOUSECMD_IDENTIFY:
					{
						//左键鉴定
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_IDENTIFY;
						//右键取消
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_IDENTIFY;		
					}
					break;

				// 添加好友
				case AOT_MOUSECMD_ADDFRIEND:
					{
						m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_ADD_FRIEND;
					}
					break;

				default:
					// 移动
					{
						m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_PLAYER_MOVETO;
						m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
						m_cmdCurrent_Left.m_afParam[1] = fvPos.z;
					}
					break;
				}
			}
		}

		// 跳跃 //好像都不太喜欢右键跳跃,忍痛割爱了
		/*if( m_cmdCurrent_Right.m_typeMouse == SCommand_Mouse::MCT_NULL )
		{
			m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_PLAYER_JUMP;
		}*/
	}
}

