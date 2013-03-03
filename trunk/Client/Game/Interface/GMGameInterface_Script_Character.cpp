
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"
#include "Procedure/GamePro_Login.h"
#include "NetWork/NetManager.h"
#include "LuaPlus.h"
#include "Global.h"
#include "Event/GMEventSystem.h"
#include "GIException.h"
#include "Action/GMActionSystem.h"
#include "DataPool/GMUIDataPool.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Object.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Input/GMInputSystem.h"
#include "world/worldmanager.h"
#include "GIDbc_struct.h"
#include "object/Logic/character/obj_playermyself.h"
#include "GIUtil.h"
#include "ScriptCommandStruct.h"
#include "CGEventRequest.h"
#include "CGTeamInvite.h"
#include "..\..\Common\database\WXSJ_DBC.h"
#include "..\..\Common\BuffImpactMgr.h"
#include "GIDBC_Struct.h"
#include "DataPool\GMDP_CharacterData.h"
#include "CGTeamApply.h"								// 发送申请加入队伍消息
#include "Object\Logic\Character\AI\GMAICommandDef.h"
#include "Object\Logic\Character\AI\GMAI_Base.h"
#include "CGAskDetailAttrib.h"
#include "CGAskDetailEquipList.h"
#include "dbc\GMDataBase.h"
#include "Procedure/GameProcedure.h"
#include "CGAskPrivateInfo.h"
#include "CGCharAllTitles.h"


namespace SCRIPT_SANDBOX
{
	
	//===============================================================
	Character Character::s_Target;
	INT Character::IsPresent(LuaPlus::LuaState* state)
	{
		if(this == &s_Target)
		{
			if(CObjectManager::GetMe()->GetMainTarget())
			{
				state->PushBoolean(true);
			}
			else
			{
				state->PushBoolean(FALSE);
			}
		}
		else
			KLThrow("Not a valid obj");

		return 1;
	}

	INT Character::GetName(LuaPlus::LuaState* state)
	{
		if(this == &s_Target)
		{
			CObject_Character* pCharObj = _GetTargetAsCharacter();

			STRING xx =pCharObj->GetCharacterData()->Get_Name();

			state->PushString(pCharObj->GetCharacterData()->Get_Name());
			
//			state->PushString(pCharObj->GetNameWithColor().c_str());
		}	
		else
			KLThrow("Not a valid obj");

		return 1;
	}

	
	INT Character::GetLevel(LuaPlus::LuaState* state)
	{
		if(this == &s_Target)
		{
			CObject_Character* pCharObj = _GetTargetAsCharacter();
			state->PushNumber(pCharObj->GetCharacterData()->Get_Level());
		}	
		else
			KLThrow("Not a valid obj");

		return 1;
	}

	INT Character::GetData(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:GetData param parameter error");
		}

		if(this == &s_Target)
		{
			CObject_Character* pCharObj = NULL;
			if( args[3].IsInteger() )
			{
				INT objServerID = args[3].GetInteger();
//				pCharObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject(objServerID));
				pCharObj = (CObject_Character*)(CDataPool::GetMe()->GetTargetEquip());
			}
			else
			{
				pCharObj = _GetTargetAsCharacter();
			}

			if(!pCharObj)
			{
				state->PushString("没得到Obj指针");
				return 1;
			}

			std::string  str = args[2].GetString();
			
			// 等级
			if( str == "LEVEL" )	
			{
				state->PushInteger(pCharObj->GetCharacterData()->Get_Level());
				return 1;
			}
			// 名字
			else if( str == "NAME" )	
			{
				state->PushString(pCharObj->GetCharacterData()->Get_Name());
				return 1;
			}
			// 名字
			else if( str == "MONEY" )	
			{
				state->PushInteger(pCharObj->GetCharacterData()->Get_Money());
				return 1;
			}
			
			// NPC ID
			else if( str == "NPCID" )	
			{
				state->PushInteger(pCharObj->GetID());
				return 1;
			}
			// 职业（门派）
			else if( str == "PROFESSION" )				
			{
				state->PushInteger(pCharObj->GetCharacterData()->Get_MenPai());
				return 1;								   
			}
			// 称号
			else if( str == "TITLE" )				
			{
				//080401
//				state->PushString(pCharObj->GetCharacterData()->Get_CurTitle());
			}
			// 相对关系
			else if( str == "RELATIVE" )			
			{
//				INT xx =pCharObj->GetCharacterData()->Get_Relative();
				state->PushInteger(pCharObj->GetCharacterData()->Get_Relative());
			}
			// 是否NPC
			else if( str == "ISNPC" )
			{
				if(pCharObj->GetCharacterType() == CObject_Character::CT_MONSTER)
				{
					state->PushInteger(1);
				}
				else
				{
					state->PushInteger(0);
				}
			}
			// 人物头像
			else if( str == "PORTRAIT" )			
			{
				if(pCharObj->GetCharacterType() == CObject_Character::CT_MONSTER)
				{
					state->PushString(((CObject_PlayerNPC*)pCharObj)->GetPortrait());
					return 1;
				}


				// 得到玩家和自己的头像信息
				LPCSTR pImageName = NULL;
				switch(pCharObj->GetCharacterType())
				{
				
				case CObject_Character::CT_PLAYERMYSELF:	// 如果是玩家
				case CObject_Character::CT_PLAYEROTHER:		// 如果是其他玩家
					{

						DBC_DEFINEHANDLE(s_pFaceImageDBC, DBC_CHARACTER_FACE)	// char_face.tab
						// 打开头像定义表
						const _DBC_CHAR_FACE* pFaceImage = NULL;

						pFaceImage = (const _DBC_CHAR_FACE*)(s_pFaceImageDBC->Search_Index_EQU(pCharObj->GetCharacterData()->Get_PortraitID()));

						// 如果没有获取到头像信息，则根据性别加载一个默认的头像
						if( pFaceImage == NULL )
						{
							int nRaceID = pCharObj->GetCharacterData()->Get_RaceID();

							nRaceID = nRaceID % 2;
							pFaceImage = (const _DBC_CHAR_FACE*)( s_pFaceImageDBC->Search_First_Column_Equ( 2,  nRaceID ) ); 
						}		

						if(pFaceImage)
						{
							pImageName = pFaceImage->pImageSetName;
						}
						//else // 默认状态不显示头像图标， 头像图标显示空
						//{
						//	pFaceImage = (const _DBC_CHAR_FACE*)(s_pFaceImageDBC->Search_LineNum_EQU(0));
						//	pImageName = pFaceImage->pImageSetName;
					
						//}//									
						
					}

				}//switch(pCharObj->GetCharacterType())

				if(pImageName)
				{
					state->PushString(pImageName);
				}
				else
				{
					state->PushString(_T(""));
				}
				return 1;
			}
			else if( str == "OCCUPATION")
			{
				GUID_t objGUID = pCharObj->GetCharacterData()->Get_OccupantGUID();
//				CObject* pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject( objGUID ) );
				if(objGUID == 0xFFFFFFFF)
				{
					state->PushInteger(-1);
					return 1;
				}
				
				if(CUIDataPool::GetMe()->IsInTeam()== FALSE)
				{
					if(objGUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
						state->PushInteger(1);
					else
						state->PushInteger(0);					
				}
				else
				{
					if(CUIDataPool::GetMe()->IsTeamMember(objGUID) == TRUE)
						state->PushInteger(1);
					else	
						state->PushInteger(0);
				}
			}
			
		}	
		else
			KLThrow("Not a valid obj");

		return 1;
	}

	INT Character::GetHPPercent(LuaPlus::LuaState* state)
	{
		if(this == &s_Target)
		{
			CObject_Character* pCharObj = _GetTargetAsCharacter();
			state->PushNumber(pCharObj->GetCharacterData()->Get_HPPercent());
		}
		else
			KLThrow("Not a valid obj");

		return 1;
	}


	INT Character::GetMPPercent(LuaPlus::LuaState* state)
	{
		if(this == &s_Target)
		{
			CObject_Character* pCharObj = _GetTargetAsCharacter();

			state->PushNumber(pCharObj->GetCharacterData()->Get_MPPercent());
		}
		else
			KLThrow("Not a valid obj");

		return 1;
	}

	INT Character::GetRagePercent(LuaPlus::LuaState* state)
	{
		if(this == &s_Target)
		{
			CObject_Character* pCharObj = _GetTargetAsCharacter();

			state->PushNumber(((FLOAT)(pCharObj->GetCharacterData()->Get_Rage()))/BASE_MAX_RAGE);
		}
		else
			KLThrow("Not a valid obj");

		return 1;
	}

	INT	Character::GetScenePos( LuaPlus::LuaState* state)
	{
		if( this == &s_Target )
		{
			CObject_Character* pCharObj = _GetTargetAsCharacter();
			const _DBC_SCENE_DEFINE* pSceneDef = CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine();
			FLOAT x = pCharObj->GetPosition().x;
			FLOAT y = pCharObj->GetPosition().z;
			state->PushNumber( x );
			state->PushNumber( y );
		}
		return 0;
	}

	INT Character::Follow(LuaPlus::LuaState* state)
	{
		if( this == &s_Target )
		{
			CObject_Character* pCharObj = _GetTargetAsCharacter();

			CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

			// 发送消息
			SCommand_AI cmdTemp;
			cmdTemp.m_wID			= AIC_FOLLOW;
			cmdTemp.m_adwParam[0]	= pCharObj->GetServerID();

			pMySelfAI->PushCommand( &cmdTemp );
		}

		return 0;
	}

	INT Character::GetPos( LuaPlus::LuaState* state)
	{
		if( this == &s_Target )
		{
			CObject_Character* pCharObj = _GetTargetAsCharacter();
			const _DBC_SCENE_DEFINE* pSceneDef = CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine();
			FLOAT x = pCharObj->GetPosition().x * 256 / pSceneDef->nXSize;
			FLOAT y = pCharObj->GetPosition().z * 256 / pSceneDef->nZSize;
			state->PushNumber( x );
			state->PushNumber( y );
			state->PushNumber(pCharObj->GetFaceDir() );
		}
		return 3;
	}


	// 请求组队
	INT	Character::SendTeamRequest(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( this == &s_Target )
		{
			// 判断是否已经入队
			if( FALSE == CUIDataPool::GetMe()->IsInTeam() )
			{
				// 没有要先创建队伍
				CGTeamInvite msg;
				msg.SetSourObjID( CObjectManager::GetMe()->GetMySelf()->GetServerID() );
				msg.SetDestName ( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name() );
				
				CNetManager::GetMe()->SendPacket( &msg );
			}

			CObject* pChar = NULL;
			if(!(args[2].IsString()))
			{
				pChar = (CObject*) CObjectManager::GetMe()->GetMainTarget();
				if(pChar && g_theKernel.IsKindOf(pChar->GetClass(), GETCLASS(CObject_Character)))
				{
					//do...
					CObject_Character* pTarget = (CObject_Character*)pChar;

					// 发送邀请某人加入队伍的消息
					//LuaStack args(state);
					//args[ 1 ].GetInteger();
					//args[ 2 ].GetString();
					if (pTarget->GetCharacterData()->IsDie())
					{
						CHAR *pContex= new CHAR[MAX_PATH];
						STRING strTemp = "对方处于死亡状态，邀请失败。";
						strncpy( pContex, strTemp.c_str(), MAX_PATH - 1 );
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, pContex );
						return 0;
					}
					
					CGTeamInvite msg;
					msg.SetSourObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
					msg.SetDestName(pTarget->GetCharacterData()->Get_Name());

					CNetManager::GetMe()->SendPacket(&msg);
				}
			}
			else
			{
				pChar = (CObject*) CObjectManager::GetMe()->GetMainTarget();
				if(pChar && g_theKernel.IsKindOf(pChar->GetClass(), GETCLASS(CObject_Character)))
				{
					CObject_Character* pTarget = (CObject_Character*)pChar;
					if (pTarget->GetCharacterData()->IsDie())
					{
						CHAR *pContex= new CHAR[MAX_PATH];
						STRING strTemp = "对方处于死亡状态，邀请失败。";
						strncpy( pContex, strTemp.c_str(), MAX_PATH - 1 );
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, pContex );
						return 0;
					}
				}
				CGTeamInvite msg;
				msg.SetSourObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
				msg.SetDestName(args[2].GetString());

				CNetManager::GetMe()->SendPacket(&msg);
			}
		}

		return 0;
	}


	// 请求加入队伍
	INT	Character::SendTeamApply(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(this == &s_Target)
		{
			CObject* pChar = NULL;

			// 通过主目标
			if(!(args[2].IsString()))
			{
				pChar = (CObject*) CObjectManager::GetMe()->GetMainTarget();
				if( pChar && g_theKernel.IsKindOf(pChar->GetClass(), GETCLASS(CObject_Character)) )
				{
					//do...
					CObject_Character* pTarget = (CObject_Character*)pChar;

					CGTeamApply msg;
					msg.SetSourGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
					msg.SetDestName(pTarget->GetCharacterData()->Get_Name());

					CNetManager::GetMe()->SendPacket(&msg);
				}
			}
			// 通过名字申请
			else
			{
				CGTeamApply msg;
				msg.SetSourGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
				msg.SetDestName(args[2].GetString());

				CNetManager::GetMe()->SendPacket(&msg);
			}
		}

		return 0;
	}

	CObject_Character* Character::_GetTargetAsCharacter(VOID)
	{
		CObject* pObj = (CObject*)CObjectManager::GetMe()->GetMainTarget();
		if(!pObj || !g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		{
			KLThrow("None target or it isn't character target!");
		}
		return (CObject_Character*)pObj;
	}


	// 得到角色身上的buff的数量
	INT Character::GetBuffNumber( LuaPlus::LuaState* state )
	{
		if(this == &s_Target)
		{
			CObject_Character* pCharObj = _GetTargetAsCharacter();

			state->PushInteger( pCharObj->GetImpactEffectMap()->size() );
		}
		else
		{
			state->PushInteger( 0 );
			//KLThrow("Not a valid obj");
		}

		return 1;
	}

	// Buff图标
	INT Character::GetBuffIconNameByIndex( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		INT nIndex = args[2].GetInteger();
		if(this == &s_Target)
		{
			CObject_Character* pCharObj = _GetTargetAsCharacter();
			CImpactEffectMap::const_iterator itFind, itEnd;

			const CImpactEffectMap *m_ImpactEffect = pCharObj->GetImpactEffectMap();

			// 20100423 ModifyCodeBegin 修正 根据target下方的buff的索引号1-10，获取buff技能图标名称及tooltip说明的一些错误
			itEnd  = m_ImpactEffect->end();	
			itFind = itEnd; // 初始化itFind为未找到状态，在下面的代码如果找到会重新赋值为找到结点的iterator

			if ( (INT)(m_ImpactEffect->size()) >= nIndex  ) // 如果技能效果队列最大索引，大于所比较的1-10内的索引号
			{
				CImpactEffectMap::const_iterator	itCur; 
				INT nCurIndex;
				itCur		= m_ImpactEffect->begin();
				nCurIndex	= 1;
				while ( itCur != itEnd ) // 从效果队列的第一个记录开始找，直到找到指定索引的记录，索引表示效果队列10个记录的先后位置
				{
					if ( nCurIndex == nIndex )
					{
						itFind =  itCur;
						break;
					}

					nCurIndex++;
					itCur++;
				}
			}

			if ( itFind != itEnd )
			{

				if( !itFind->second )
				{
					state->PushString("");
					state->PushString("");
					state->PushInteger( -1 );
					return 3;
				}

		
				_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( itFind->second->GetImpactID() );

				if ( pBuffImpact != NULL )
				{	
					if (  pBuffImpact->m_szIconName != "" && pBuffImpact->m_pszInfo != "" )
					{
						state->PushString( pBuffImpact->m_szIconName );						
						state->PushString( pBuffImpact->m_pszInfo );
						state->PushInteger( itFind->second->GetContinuance() );
						return 3;	
					}					
				}					
			}
			//else
				//KLThrow("Not found the impact.");
		}
		//else
				//KLThrow("Not a valid obj");
		
		state->PushString("");
		state->PushString("");
		state->PushInteger( -1 );
		return 3;

		// 20100423 ModifyCodeEnd
		
	}

	INT Character::GetDialogNpcName(LuaPlus::LuaState* state)
	{
		INT nDialogNpcId = CUIDataPool::GetMe()->GetCurDialogNpcId();

		CObject_Character *pObj = (CObject_Character*)CObjectManager::GetMe()->FindServerObject(nDialogNpcId);
		if(pObj)
		{
			state->PushString(pObj->GetCharacterData()->Get_Name());
		}
		else
		{
			state->PushString("");		
			//KLThrow("Not a valid obj");
		}

		return 1;
	}

	INT Character::GetDialogNpcSeverID(LuaPlus::LuaState* state)
	{
		INT nDialogNpcId = CUIDataPool::GetMe()->GetCurDialogNpcId();
		CObject_Character *pObj = (CObject_Character*)CObjectManager::GetMe()->FindServerObject(nDialogNpcId);
		if(pObj)
		{
			state->PushInteger(nDialogNpcId);
		}
		else
		{
			state->PushInteger(-1);				
		}

		return 1;
	}

	INT Character::GetShopNpcName(LuaPlus::LuaState* state)
	{
		INT nShopNpcId = CUIDataPool::GetMe()->GetCurShopNpcId();

		CObject_Character *pObj = (CObject_Character*)CObjectManager::GetMe()->FindServerObject(nShopNpcId);
		if(pObj)
		{
			state->PushString(pObj->GetCharacterData()->Get_Name());
		}
		else
			KLThrow("Not a valid obj");

		return 1;
	}

	INT Character::GetXinfaNpcName(LuaPlus::LuaState* state)
	{
		/*INT nXinfaNpcId = CDataPool::GetMe()->StudyXinfa_GetXinfaTeacherId();
		CObject_Character *pObj = (CObject_Character*)CObjectManager::GetMe()->FindServerObject(nXinfaNpcId);
		if(pObj)
		{
			state->PushString(pObj->GetCharacterData()->Get_Name());
		}
		else
			KLThrow("Not a valid obj");*/

		return 1;
	}

	INT Character::Close_Before_TargetEquip_UI(LuaPlus::LuaState* state)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_TARGET_EQUIP );
		return 0;
	}

	INT Character::TargetEquip_ChangeModel(LuaPlus::LuaState* state)
	{
		CObjectManager::GetMe()->SetTargetAvatarAsTarget();
		return 0;
	}

	INT Character::SendAskDetail(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		CObject_Character* pCharObj = NULL;

		if(!(args[2].IsString()))
		{
			pCharObj = _GetTargetAsCharacter();
		}
		else
		{
			pCharObj = CObjectManager::GetMe()->FindCharacterByName(args[2].GetString());
			if(pCharObj && g_theKernel.IsKindOf(pCharObj->GetClass(), GETCLASS(CObject_Character)))
			{
			}
			else
			{
				pCharObj = NULL;
			}
		}

		if( pCharObj )
		{
			// 查询详细属性
			CGAskDetailAttrib	msgMyAttrib;
			msgMyAttrib.setTargetID( (ObjID_t)pCharObj->GetServerID() );
			CNetManager::GetMe()->SendPacket(&msgMyAttrib);			


			//// 查询玩家信息
			//CGAskPrivateInfo msg;
			//msg.SetPlayerID( (ObjID_t)pCharObj->GetServerID() );
			//CDataPool::GetMe()->PI_SetOtName(pCharObj->GetCharacterData()->Get_Name());
			//CDataPool::GetMe()->PI_SetObjId(pCharObj->GetID());
			//CNetManager::GetMe()->SendPacket(&msg);


			// 查询玩家所有称号信息
			CGCharAllTitles   msgAllTitles;
			msgAllTitles.setTargetID((ObjID_t)pCharObj->GetServerID());
			CNetManager::GetMe()->SendPacket(&msgAllTitles);


			// 查询详细装备
			CGAskDetailEquipList msgAskMyDetailEquip;
			msgAskMyDetailEquip.setObjID( (ObjID_t)pCharObj->GetServerID() );
			msgAskMyDetailEquip.SetAskMode(ASK_EQUIP_ALL);
			CNetManager::GetMe()->SendPacket(&msgAskMyDetailEquip);
		}
		return 0;
	}


	INT Character::Set_To_Private(LuaPlus::LuaState* state)
	{
//		CObject_Character* pCharObj = _GetTargetAsCharacter();
		LuaStack args(state);

		LPCTSTR szName = "";


		CObject_Character* pCharObj = NULL;
		if(!(args[2].IsString()))
		{
			pCharObj = _GetTargetAsCharacter();
			szName = pCharObj->GetCharacterData()->Get_Name();
		}
		else
		{
			szName = args[2].GetString();
		}

		CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_CHANGE_PRIVATENAME, szName );

		return 0;
	}

	// 当前的 target 是否是队友
	INT	Character::IsTargetTeamMember(LuaPlus::LuaState* state)
	{
		BOOL bIsSelTeamMember = CObjectManager::GetMe()->IsSelTeam();
		state->PushNumber( bIsSelTeamMember );
		return 1;
	}

	// 当前的 target 是否合法 
	INT	Character::IsTargetValide(LuaPlus::LuaState* state)
	{
		if(NULL == CObjectManager::GetMe()->GetMainTarget())
		{
			state->PushNumber(0);
		}
		else
		{
			state->PushNumber( 1 );
		}

		
		return 1;
	}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main target 窗口显示队友信息。 

	// 显示名字
	INT Character::TargetFrame_Update_Name_Team(LuaPlus::LuaState* state)
	{
		LPCSTR pName = CUIDataPool::GetMe()->GetTeamNameByGUID( CObjectManager::GetMe()->GetCurSelTeam() );
		
		if(NULL == pName)
		{
			state->PushString("");
		}
		else
		{
			state->PushString(pName);
		}
		return 1;
	}

	// 显示hp
	INT Character::TargetFrame_Update_HP_Team(LuaPlus::LuaState* state)
	{
		//int iHP    = CUIDataPool::GetMe()->GetTeamHPByGUID(CObjectManager::GetMe()->m_GUID);

		float fHPPercent = CUIDataPool::GetMe()->GetTeamHPPercentByGUID( CObjectManager::GetMe()->GetCurSelTeam() );
		
		state->PushNumber( fHPPercent );
		return 1;
	}

	// 显示mp
	INT	Character::TargetFrame_Update_MP_Team(LuaPlus::LuaState* state)
	{
		//int iMP = CUIDataPool::GetMe()->GetTeamMPByGUID(CObjectManager::GetMe()->m_GUID);
		//state->PushNumber( iMP );//

		float fMPPercent = CUIDataPool::GetMe()->GetTeamMPPercentByGUID( CObjectManager::GetMe()->GetCurSelTeam() );
		state->PushNumber( fMPPercent );//
		return 1;
	}

	// 显示怒气
	INT Character::TargetFrame_Update_Rage_Team(LuaPlus::LuaState* state)
	{
		int iRange = CUIDataPool::GetMe()->GetTeamRangeByGUID( CObjectManager::GetMe()->GetCurSelTeam() );
		state->PushNumber( iRange );
		return 1;
	}

	// 显示等级
	INT Character::TargetFrame_Update_Level_Team(LuaPlus::LuaState* state)
	{
		int iLevel = CUIDataPool::GetMe()->GetTeamLevelByGUID( CObjectManager::GetMe()->GetCurSelTeam() );
		state->PushNumber( iLevel );
		return 1;
	}

	// 显示头像名字。
	INT Character::TargetFrame_Update_Icon_Team(LuaPlus::LuaState* state)
	{
		LPCSTR strIcon = CUIDataPool::GetMe()->GetTeamIconByGUID( CObjectManager::GetMe()->GetCurSelTeam() );
		if(NULL == strIcon)
		{
			state->PushString( "" );
		}
		else
		{
			state->PushString( strIcon );
		}
		return 1;
	}

	// 显示目标头像的右键菜单
	INT	Character::ShowTargetContexMenu(LuaPlus::LuaState* state)
	{
		INT targetId = CObjectManager::GetMe()->GetMainTarget()->GetServerID();
		CGameInterface::GetMe()->Object_ShowContexMenu( targetId, FALSE );
		return 0;
	}


}	// namespace 