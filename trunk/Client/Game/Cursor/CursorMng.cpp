
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

// ��ӦWM_SETCURSOR
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


// ���ǰ����Ϲҽӵ�����
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
			
				// �ָ������
				CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
			}
		}
		break;

	// �Ե�һ����ʹ�ü���
	case SCommand_Mouse::MCT_SKILL_OBJ:
		{
			// ����ѡ��Ŀ��
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[1], 2);

			tActionItem* pItem = (tActionItem*)cmd.m_apParam[0];
			if( !pItem ) 
				break;

			// ���﹥��
			//if( pItem->GetType() == AOT_PET_SKILL )
			//{
			//	int nPetSkillID = ((PET_SKILL*)(((CActionItem_PetSkill*)pItem)->GetImpl()))->m_pDefine->m_nID;
			//	CDataPool::GetMe()->Pet_SendUseSkillMessage(nPetSkillID, INVALID_ID, cmd.m_afParam[1], cmd.m_afParam[2]);
			//}
			//// ��ҹ���
			//else
			{
				CGameProcedure::s_pGameInterface->Player_UseSkill(((tActionItem*)cmd.m_apParam[0])->GetID(), (INT)cmd.m_adwParam[1]);
			}
		}
		break;

	// ��ĳ����Χʹ�ü���
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

			// �ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	// ��ĳ������ʹ�ü��ܣ����ι�����
	case SCommand_Mouse::MCT_SKILL_DIR:
		{
			tActionItem* pItem = (tActionItem*)cmd.m_apParam[0];
			if(!pItem) 
				break;

			// ���Գ���ļ���ʹ��

			// ����ķ�����ʹ��
			CGameProcedure::s_pGameInterface->Player_UseSkill(((tActionItem*)cmd.m_apParam[0])->GetID(), cmd.m_afParam[1]);

			// �ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	// �����֮���
	case SCommand_Mouse::MCT_HIT_TRIPPEROBJ:	
		{
			CGameProcedure::s_pGameInterface->TripperObj_Active(cmd.m_adwParam[0]);
		}
		break;

	// NPC�Ի�
	case SCommand_Mouse::MCT_SPEAK:		
		{
			// ����ѡ��Ŀ��
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0], 3);

			// ̸��
			CGameProcedure::s_pGameInterface->Player_Speak(cmd.m_adwParam[0]);
		}
		break;

	// �Ҽ������˵�
	case SCommand_Mouse::MCT_CONTEXMENU:
		{
			// ����ѡ��
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0], 0);
			
			// ��ʾ�Ҽ��˵�
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
			// ȡ������
			CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
		}
		break;

    
	case SCommand_Mouse::MCT_UI_USE_IDENTIFY:
		{
			// ʹ�ü�������
			
			break;
		}
	case SCommand_Mouse::MCT_UI_USE_CANCEL_IDENTIFY:
		{
			// ȡ��ʹ�ü�������
			CDataPool::GetMe()->Identify_UnLock();
			break;
		}

	case SCommand_Mouse::MCT_USE_ITEM:
		{
			// ʹ����Ʒ
			CGameProcedure::s_pGameInterface->PacketItem_UserItem((	tActionItem*)cmd.m_apParam[0], 
																	cmd.m_adwParam[1], fVector2(cmd.m_afParam[2], cmd.m_afParam[3]));

			// �ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SCommand_Mouse::MCT_CANCEL_USE_ITEM:
		{
			// �ָ�����Action
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

	m_hCursor[ CURSOR_WINBASE ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));				// ϵͳ��׼��
	m_hCursor[ CURSOR_NORMAL ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_NORMAL));		// ��ͨ
	m_hCursor[ CURSOR_ATTACK ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_ATTACK));		// ����
	m_hCursor[ CURSOR_AUTORUN ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_AUTORUN ));	// �Զ�������
	m_hCursor[ CURSOR_PICKUP ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_PICKUP ));		// ʰȡ��Ʒ
	m_hCursor[ CURSOR_UNREACHABLE ] = ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_UNREACHABLE ));// �޷����������
	m_hCursor[ CURSOR_MINE ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_MINE ));		// �ɿ�
	m_hCursor[ CURSOR_HERBS ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_HERBS ));		// ��ҩ
	m_hCursor[ CURSOR_SPEAK ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_SPEAK ));		// ̸��
	m_hCursor[ CURSOR_INTERACT ]	= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_INTERACT ));	// ����
	m_hCursor[ CURSOR_REPAIR ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_REPAIR ));		// ����
	m_hCursor[ CURSOR_HOVER ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_HOVER));		// ��꼤��(�ҽ���Ʒ...)
	m_hCursor[ CURSOR_IDENTIFY ]	= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_IDENFITY ));	// ������
	m_hCursor[ CURSOR_RING ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_RING ));		// ���ܻ�
	m_hCursor[ CURSOR_DIRECTION ]	= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_DIRECTION ));	// ������
}

VOID CCursorMng::MouseCommand_Set( BOOL bHoverInUI, CObject* pSelectObj, const fVector3& fvPos, tActionItem* pActiveSkill )
{
	m_cmdCurrent_Left.m_typeMouse	= SCommand_Mouse::MCT_NULL;
	m_cmdCurrent_Right.m_typeMouse	= SCommand_Mouse::MCT_NULL;


	// ��UI�Ͽ�
	if(bHoverInUI)
	{
		if(!pActiveSkill)
			return;

		switch(pActiveSkill->GetType())
		{
		// ����װ��
		case AOT_MOUSECMD_REPARE:
			{
				//����Ҵ���
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_REPAIR;
				//�Ҽ�ȡ��
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCLE_REPAIR;		
			}
			break;

		//����װ��
		case AOT_MOUSECMD_IDENTIFY:
			{
				//�������
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_IDENTIFY;
				//�Ҽ�ȡ��
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_IDENTIFY;		
			}
			break;
		}
	}
	else
	{
		// �ж����������ѡ�������������Ӧobj���ָ��
		if( pSelectObj != NULL )
		{
			pSelectObj->FillMouseCommand_Left( &m_cmdCurrent_Left, pActiveSkill );
			pSelectObj->FillMouseCommand_Right( &m_cmdCurrent_Right, pActiveSkill );
		}
		else
		{
			if( !pActiveSkill )
			{
				// ��������û����Ӧ�Ķ��� �͵����ƶ������� �ĵ��˱���Ҫ�и�Ĭ�ϼ��ܲ�����·������
				m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_PLAYER_MOVETO;
				m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
				m_cmdCurrent_Left.m_afParam[1] = fvPos.z;
			}

		}

		if( m_cmdCurrent_Left.m_typeMouse == SCommand_Mouse::MCT_NULL )
		{
			// ��ǰ���ڼ���ļ���
			if( pActiveSkill != NULL )
			{
				// ���ݼ��ܵ��������������Ϊ
				switch( pActiveSkill->GetType() )
				{
				// ���� �� ���＼��
				case AOT_SKILL:
				case AOT_PET_SKILL:
					{
						const void* pSkillImpl = pActiveSkill->GetImpl();

						// �������ڲ����ݾ���Ϊ����״̬
						if( !pSkillImpl )
						{
							m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_PLAYER_MOVETO;
							m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
							m_cmdCurrent_Left.m_afParam[1] = fvPos.z;

							return;
						}

						// �����Ƿ�Ϊս�����ܴ�������������ļ���ѡ������
						ENUM_SELECT_TYPE typeSel = (ENUM_SELECT_TYPE)(	pActiveSkill->GetType() == AOT_SKILL ? 
																		((SCLIENT_SKILL*)pSkillImpl)->m_pDefine->m_nSelectType :	// ���＼��
																		((PET_SKILL*)pSkillImpl)->m_pDefine->m_nSelectType );		// ���＼��

						// ����ʹ�õ����ѡ���߼�
						switch( typeSel )
						{
						// ��ѡ��ɫ
						case SELECT_TYPE_CHARACTER:
							{
								m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_PLAYER_MOVETO;
								m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
								m_cmdCurrent_Left.m_afParam[1] = fvPos.z;
							}
							break;

						// ��ѡ��Χ
						case SELECT_TYPE_POS:
							{
								// ���漼��
								m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_SKILL_AREA;		// ���Զ�����ʱ����Ϊ SCommand_Mouse::MCT_PLAYER_MOVETO;
								m_cmdCurrent_Left.m_apParam[0] = (VOID*)pActiveSkill;
								m_cmdCurrent_Left.m_afParam[1] = fvPos.x;
								m_cmdCurrent_Left.m_afParam[2] = fvPos.z;
							}
							break;

						// ����
						case SELECT_TYPE_DIR:
							{
								// ���漼��
								m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_SKILL_DIR;	// ���Զ�����ʱ����Ϊ SCommand_Mouse::MCT_PLAYER_MOVETO;
								m_cmdCurrent_Left.m_apParam[0] = (VOID*)pActiveSkill;

								//====== ���㷽��

								// ����λ��
								fVector3 fvPlayer = CObjectManager::GetMe()->GetMySelf()->GetPosition();
								// ���λ��
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

				// ��Ʒ
				case AOT_ITEM:
					{
						if(!pActiveSkill || pActiveSkill->GetType() != AOT_ITEM) 
							break;

						CObject_Item* pItem = (CObject_Item*)(((CActionItem_Item*)pActiveSkill)->GetItemImpl());
						if(!pItem)
							break;

						// �������ܹ�ʹ�õ���Ʒ
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

				// ����������
				case AOT_MOUSECMD_REPARE:
					{
						//����Ҵ���
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_REPAIR;
						//�Ҽ�ȡ��
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCLE_REPAIR;		
					}
					break;

				// ����װ��
				case AOT_MOUSECMD_IDENTIFY:
					{
						//�������
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_IDENTIFY;
						//�Ҽ�ȡ��
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_IDENTIFY;		
					}
					break;

				// ��Ӻ���
				case AOT_MOUSECMD_ADDFRIEND:
					{
						m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_ADD_FRIEND;
					}
					break;

				default:
					// �ƶ�
					{
						m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_PLAYER_MOVETO;
						m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
						m_cmdCurrent_Left.m_afParam[1] = fvPos.z;
					}
					break;
				}
			}
		}

		// ��Ծ //���񶼲�̫ϲ���Ҽ���Ծ,��ʹ���
		/*if( m_cmdCurrent_Right.m_typeMouse == SCommand_Mouse::MCT_NULL )
		{
			m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_PLAYER_JUMP;
		}*/
	}
}

